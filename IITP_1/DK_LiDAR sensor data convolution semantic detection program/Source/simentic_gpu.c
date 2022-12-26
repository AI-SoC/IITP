#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>
#include <math.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>

#define move 10
#define threashold 250

#define CHECK_ERROR(err)\
        if(err!=CL_SUCCESS){\
                printf("[%s:%d] OpenCL error %d\n", __FILE__, __LINE__, err);\
        exit(EXIT_FAILURE);\
        }


char* day[3];
char* month[3];
char* date[2];
char* tim[8];
char* year[4];

int counts = 198;

float alltime = 0;

double get_time() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return (double)tv.tv_sec + (double)1e-6 * tv.tv_usec;
}

char *get_source_code(const char *file_name, size_t *len)
{
	char *source_code;
	size_t length;
	FILE *file = fopen(file_name, "r");
	if (file == NULL) {
		printf("[%s:%d] OpenCL error %d\n", __FILE__, __LINE__, file_name);
		exit(EXIT_FAILURE);
	}

	fseek(file, 0, SEEK_END);
	length = (size_t) ftell(file);
	rewind(file);

	source_code = (char *) malloc(length + 1);
	fread(source_code, length, 1, file);
	source_code[length] = '\0';

	fclose(file);
	*len = length;
	return source_code;
}

int main()
{
    FILE* file = fopen("output.txt","r");
    int buff;
    int* data = (int*) malloc(sizeof(int) * 580 * 100);
    int* window = (int*) malloc(sizeof(int) * 50 * 50);
    int* conv = (int*) malloc(sizeof(int) * 53 * 5);

    cl_platform_id platform;
	cl_device_id device;
	cl_context context;
	cl_command_queue queue;
	cl_program program;
	char *kernel_source;
	size_t kernel_source_size;
	cl_kernel kernel;
	cl_int err;

	err = clGetPlatformIDs(1, &platform, NULL);
	CHECK_ERROR(err);

	err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);
	CHECK_ERROR(err);

	context = clCreateContext(NULL, 1, &device, NULL, NULL, &err);
	CHECK_ERROR(err);

	queue = clCreateCommandQueue(context, device, 0, &err);
	CHECK_ERROR(err);

	kernel_source = get_source_code("con_open.cl", &kernel_source_size);
	program = clCreateProgramWithSource(context, 1,
			(const char**) &kernel_source, &kernel_source_size, &err);
	CHECK_ERROR(err);

	err = clBuildProgram(program, 1, &device, "", NULL, NULL);

	if (err == CL_BUILD_PROGRAM_FAILURE) {
		size_t log_size;
		char *log;

		err = clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, 0,
		NULL, &log_size);
		CHECK_ERROR(err);

		log = (char*) malloc(log_size + 1);
		err = clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG,
				log_size, log, NULL);
		CHECK_ERROR(err);

		log[log_size] = '\0';
		printf("comfiler error : \n%s\n", log);
		free(log);
		exit(0);
	}
	CHECK_ERROR(err);

	kernel = clCreateKernel(program, "convolution", &err);
	CHECK_ERROR(err);

    cl_mem A1, B1, C1;
	A1 = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(int) * 580 * 100, NULL, &err);
	CHECK_ERROR(err);
	B1 = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(int) * 50 * 50, NULL, &err);
	CHECK_ERROR(err);
	C1 = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(int) * 53 * 5, NULL, &err);
	CHECK_ERROR(err);

    for(int i=0;i<50;i++)
    {
        for(int j=0;j<50;j++)
        {
            window[i*50+j] = 1;
        }
    }

    #if 1
    for(int i=10;i<40;i++)
    {
        for(int j=20;j<30;j++)
        {
            window[i*50+j] += 1;
        }
    }
    #endif

    #if 1
    for(int i=20;i<30;i++)
    {
        for(int j=20;j<30;j++)
        {
            window[i*50+j] += 1;
        }
    }
    #endif

    fscanf(file,"%s %s %s %s %s",day,month,date,tim,year);

    for(int i=0;i<5800;i++)
    {
        for(int j=0;j<100;j++)
        {
            fscanf(file,"%d",&buff);
            data[i*100+j] = buff;
        }
    }

    while(counts--)
    {
        double start_time = get_time();
        fscanf(file,"%s %s %s %s %s",day,month,date,tim,year);

        for(int i=0;i<5800;i++)
        {
            for(int j=0;j<100;j++)
            {
                fscanf(file,"%d",&buff);
                data[i*100+j] = buff;
            }
        }

//////////////////////////
        err = clEnqueueWriteBuffer(queue, A1, CL_FALSE, 0,
                sizeof(int) * 580 * 100, data, 0, NULL, NULL);
        CHECK_ERROR(err);

        err = clEnqueueWriteBuffer(queue, B1, CL_FALSE, 0,
                sizeof(int) * 50 * 50, window, 0, NULL, NULL);
        CHECK_ERROR(err);

        err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &A1);
        CHECK_ERROR(err);
        err = clSetKernelArg(kernel, 1, sizeof(cl_mem), &B1);
        CHECK_ERROR(err);
        err = clSetKernelArg(kernel, 2, sizeof(cl_mem), &C1);
        CHECK_ERROR(err);

        size_t global_size[2] = { 53, 5 };
        size_t local_size[2] = { 16, 16 };
        global_size[0] = (global_size[0] + local_size[0] - 1) / local_size[0] * local_size[0];
        global_size[1] = (global_size[1] + local_size[1] - 1) / local_size[1] * local_size[1];

        err = clEnqueueNDRangeKernel(queue, kernel, 2, NULL, global_size, local_size, 0, NULL, NULL);
        CHECK_ERROR(err);

        err = clEnqueueReadBuffer(queue, C1, CL_TRUE, 0, sizeof(int) * 53 * 5, conv, 0, NULL, NULL);
        CHECK_ERROR(err);
//////////////////////////

        printf("%s %s %s %s %s\n",day,month,date,tim,year);
        for(int i=0;i<53*5;i++)
        {
            if(conv[i]/100>threashold)
            {
                printf("Movement Detected at [%d/%d] : %d\n",i/53,i%53,conv[i]/100);
            }
        }
        double end_time = get_time();
        alltime += (end_time - start_time);
    }
    printf("Final lapsed time : %f sec\n", alltime);

    return 0;
}


/*
file size : 580*100
window size : 50*50
window move : 10
convolution size : 53*5
*/
