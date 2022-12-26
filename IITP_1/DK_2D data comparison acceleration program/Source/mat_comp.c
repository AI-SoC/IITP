#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>
#include <math.h>
#include <sys/time.h>
#include <unistd.h>

#define CHECK_ERROR(err)\
        if(err!=CL_SUCCESS){\
                printf("[%s:%d] OpenCL error %d\n", __FILE__, __LINE__, err);\
        exit(EXIT_FAILURE);\
        }

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

static int CHAN = 15;
static int POINT = 370;
static int DIFF = 1;

static int case1 = 2;
static int case2 = 6;
static int case3 = 11;
static int case4 = 21;
static int case5 = 51;

int main(int argc, char *argv[]) {
	float *IMG1a = (float*) malloc(sizeof(float) * CHAN * POINT * case1);
	float *IMG2a = (float*) malloc(sizeof(float) * CHAN * POINT * case2);
	float *IMG3a = (float*) malloc(sizeof(float) * CHAN * POINT * case3);
	float *IMG4a = (float*) malloc(sizeof(float) * CHAN * POINT * case4);
	float *IMG5a = (float*) malloc(sizeof(float) * CHAN * POINT * case5);
    float *IMG1b = (float*) malloc(sizeof(float) * CHAN * POINT * case1);
	float *IMG2b = (float*) malloc(sizeof(float) * CHAN * POINT * case2);
	float *IMG3b = (float*) malloc(sizeof(float) * CHAN * POINT * case3);
	float *IMG4b = (float*) malloc(sizeof(float) * CHAN * POINT * case4);
	float *IMG5b = (float*) malloc(sizeof(float) * CHAN * POINT * case5);

	int i;

	for (i = 0; i < CHAN*POINT*case1; i++)
    {
        IMG1a[i] = (float) (rand() % 1000) / 10.0f;
	}
    for (i = 0; i < CHAN*POINT*case2; i++)
    {
        IMG2a[i] = (float) (rand() % 1000) / 10.0f;
	}
    for (i = 0; i < CHAN*POINT*case3; i++)
    {
        IMG3a[i] = (float) (rand() % 1000) / 10.0f;
	}
    for (i = 0; i < CHAN*POINT*case4; i++)
    {
        IMG4a[i] = (float) (rand() % 1000) / 10.0f;
	}
    for (i = 0; i < CHAN*POINT*case5; i++)
    {
        IMG5a[i] = (float) (rand() % 1000) / 10.0f;
	}

	printf("Matrix Compare - OpenCL\n");

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

	kernel_source = get_source_code("mat_comp.cl", &kernel_source_size);
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

	kernel = clCreateKernel(program, "mat_com", &err);
	CHECK_ERROR(err);

	cl_mem buf1a, buf1b, buf2a, buf2b, buf3a, buf3b, buf4a, buf4b, buf5a, buf5b, buf6a, buf6b, buf7a, buf7b, buf8a, buf8b, buf9a, buf9b, buf10a, buf10b;
	buf1a = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(float) * CHAN * POINT * case1, NULL, &err);
	CHECK_ERROR(err);
    buf2a = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(float) * CHAN * POINT * case2, NULL, &err);
	CHECK_ERROR(err);
    buf3a = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(float) * CHAN * POINT * case3, NULL, &err);
	CHECK_ERROR(err);
    buf4a = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(float) * CHAN * POINT * case4, NULL, &err);
	CHECK_ERROR(err);
    buf5a = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(float) * CHAN * POINT * case5, NULL, &err);
	CHECK_ERROR(err);

	buf1b = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(float) * CHAN * POINT * case1, NULL, &err);
	CHECK_ERROR(err);
    buf2b = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(float) * CHAN * POINT * case2, NULL, &err);
	CHECK_ERROR(err);
    buf3b = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(float) * CHAN * POINT * case3, NULL, &err);
	CHECK_ERROR(err);
    buf4b = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(float) * CHAN * POINT * case4, NULL, &err);
	CHECK_ERROR(err);
	buf5b = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(float) * CHAN * POINT * case5, NULL, &err);
	CHECK_ERROR(err);

	/////////////////////////////////////////////////////////////
    printf("Case 1 : 1 compare - 100 times\n");
	double start_time = get_time();

	for(i=0;i<100;i++)
    {
        err = clEnqueueWriteBuffer(queue, buf1a, CL_FALSE, 0, sizeof(float) * CHAN * POINT * case1, IMG1a, 0, NULL, NULL);
        CHECK_ERROR(err);

        err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &buf1a);
        CHECK_ERROR(err);
        err = clSetKernelArg(kernel, 1, sizeof(cl_mem), &buf1b);
        CHECK_ERROR(err);
        err = clSetKernelArg(kernel, 2, sizeof(cl_int), &case1);
        CHECK_ERROR(err);
        err = clSetKernelArg(kernel, 3, sizeof(cl_int), &DIFF);
        CHECK_ERROR(err);

        size_t global_size[2] = { CHAN, POINT };
        size_t local_size[2] = { 16, 16 };
        global_size[0] = (global_size[0] + local_size[0] - 1) / local_size[0] * local_size[0];
        global_size[1] = (global_size[1] + local_size[1] - 1) / local_size[1] * local_size[1];

        err = clEnqueueNDRangeKernel(queue, kernel, 2, NULL, global_size, local_size, 0, NULL, NULL);
        CHECK_ERROR(err);

        err = clEnqueueReadBuffer(queue, buf1b, CL_TRUE, 0, sizeof(float) * CHAN * POINT * case1, IMG1b, 0, NULL, NULL);
        CHECK_ERROR(err);
    }

	double end_time = get_time();
	printf("Elapsed time : %f sec\n", end_time - start_time);
    /////////////////////////////////////////////////////////

    /////////////////////////////////////////////////////////////
    printf("Case 2 : 5 compare - 20 times\n");
	start_time = get_time();

	for(i=0;i<20;i++)
    {
        err = clEnqueueWriteBuffer(queue, buf2a, CL_FALSE, 0, sizeof(float) * CHAN * POINT * case2, IMG2a, 0, NULL, NULL);
        CHECK_ERROR(err);

        err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &buf2a);
        CHECK_ERROR(err);
        err = clSetKernelArg(kernel, 1, sizeof(cl_mem), &buf2b);
        CHECK_ERROR(err);
        err = clSetKernelArg(kernel, 2, sizeof(cl_int), &case2);
        CHECK_ERROR(err);
        err = clSetKernelArg(kernel, 3, sizeof(cl_int), &DIFF);
        CHECK_ERROR(err);

        size_t global_size[2] = { CHAN, POINT };
        size_t local_size[2] = { 16, 16 };
        global_size[0] = (global_size[0] + local_size[0] - 1) / local_size[0] * local_size[0];
        global_size[1] = (global_size[1] + local_size[1] - 1) / local_size[1] * local_size[1];

        err = clEnqueueNDRangeKernel(queue, kernel, 2, NULL, global_size, local_size, 0, NULL, NULL);
        CHECK_ERROR(err);

        err = clEnqueueReadBuffer(queue, buf2b, CL_TRUE, 0, sizeof(float) * CHAN * POINT * case2, IMG2b, 0, NULL, NULL);
        CHECK_ERROR(err);
    }

	end_time = get_time();
	printf("Elapsed time : %f sec\n", end_time - start_time);
    /////////////////////////////////////////////////////////

    /////////////////////////////////////////////////////////////
    printf("Case 3 : 10 compare - 10 times\n");
	start_time = get_time();

	for(i=0;i<10;i++)
    {
        err = clEnqueueWriteBuffer(queue, buf3a, CL_FALSE, 0, sizeof(float) * CHAN * POINT * case3, IMG3a, 0, NULL, NULL);
        CHECK_ERROR(err);

        err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &buf3a);
        CHECK_ERROR(err);
        err = clSetKernelArg(kernel, 1, sizeof(cl_mem), &buf3b);
        CHECK_ERROR(err);
        err = clSetKernelArg(kernel, 2, sizeof(cl_int), &case3);
        CHECK_ERROR(err);
        err = clSetKernelArg(kernel, 3, sizeof(cl_int), &DIFF);
        CHECK_ERROR(err);

        size_t global_size[2] = { CHAN, POINT };
        size_t local_size[2] = { 16, 16 };
        global_size[0] = (global_size[0] + local_size[0] - 1) / local_size[0] * local_size[0];
        global_size[1] = (global_size[1] + local_size[1] - 1) / local_size[1] * local_size[1];

        err = clEnqueueNDRangeKernel(queue, kernel, 2, NULL, global_size, local_size, 0, NULL, NULL);
        CHECK_ERROR(err);

        err = clEnqueueReadBuffer(queue, buf3b, CL_TRUE, 0, sizeof(float) * CHAN * POINT * case3, IMG3b, 0, NULL, NULL);
        CHECK_ERROR(err);
    }

	end_time = get_time();
	printf("Elapsed time : %f sec\n", end_time - start_time);
    /////////////////////////////////////////////////////////

    /////////////////////////////////////////////////////////////
    printf("Case 4 : 20 compare - 5 times\n");
	start_time = get_time();

	for(i=0;i<5;i++)
    {
        err = clEnqueueWriteBuffer(queue, buf4a, CL_FALSE, 0, sizeof(float) * CHAN * POINT * case4, IMG4a, 0, NULL, NULL);
        CHECK_ERROR(err);

        err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &buf4a);
        CHECK_ERROR(err);
        err = clSetKernelArg(kernel, 1, sizeof(cl_mem), &buf4b);
        CHECK_ERROR(err);
        err = clSetKernelArg(kernel, 2, sizeof(cl_int), &case4);
        CHECK_ERROR(err);
        err = clSetKernelArg(kernel, 3, sizeof(cl_int), &DIFF);
        CHECK_ERROR(err);

        size_t global_size[2] = { CHAN, POINT };
        size_t local_size[2] = { 16, 16 };
        global_size[0] = (global_size[0] + local_size[0] - 1) / local_size[0] * local_size[0];
        global_size[1] = (global_size[1] + local_size[1] - 1) / local_size[1] * local_size[1];

        err = clEnqueueNDRangeKernel(queue, kernel, 2, NULL, global_size, local_size, 0, NULL, NULL);
        CHECK_ERROR(err);

        err = clEnqueueReadBuffer(queue, buf4b, CL_TRUE, 0, sizeof(float) * CHAN * POINT * case4, IMG4b, 0, NULL, NULL);
        CHECK_ERROR(err);
    }

	end_time = get_time();
	printf("Elapsed time : %f sec\n", end_time - start_time);
    /////////////////////////////////////////////////////////

    /////////////////////////////////////////////////////////////
    printf("Case 5 : 50 compare - 2 times\n");
	start_time = get_time();

	for(i=0;i<2;i++)
    {
        err = clEnqueueWriteBuffer(queue, buf5a, CL_FALSE, 0, sizeof(float) * CHAN * POINT * case5, IMG5a, 0, NULL, NULL);
        CHECK_ERROR(err);

        err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &buf5a);
        CHECK_ERROR(err);
        err = clSetKernelArg(kernel, 1, sizeof(cl_mem), &buf5b);
        CHECK_ERROR(err);
        err = clSetKernelArg(kernel, 2, sizeof(cl_int), &case5);
        CHECK_ERROR(err);
        err = clSetKernelArg(kernel, 3, sizeof(cl_int), &DIFF);
        CHECK_ERROR(err);

        size_t global_size[2] = { CHAN, POINT };
        size_t local_size[2] = { 16, 16 };
        global_size[0] = (global_size[0] + local_size[0] - 1) / local_size[0] * local_size[0];
        global_size[1] = (global_size[1] + local_size[1] - 1) / local_size[1] * local_size[1];

        err = clEnqueueNDRangeKernel(queue, kernel, 2, NULL, global_size, local_size, 0, NULL, NULL);
        CHECK_ERROR(err);

        err = clEnqueueReadBuffer(queue, buf5b, CL_TRUE, 0, sizeof(float) * CHAN * POINT * case5, IMG5b, 0, NULL, NULL);
        CHECK_ERROR(err);
    }

	end_time = get_time();
	printf("Elapsed time : %f sec\n", end_time - start_time);
    /////////////////////////////////////////////////////////

	clReleaseMemObject(buf1a);
    clReleaseMemObject(buf1b);
	clReleaseMemObject(buf2a);
    clReleaseMemObject(buf2b);
	clReleaseMemObject(buf3a);
    clReleaseMemObject(buf3b);
	clReleaseMemObject(buf4a);
    clReleaseMemObject(buf4b);
	clReleaseMemObject(buf5a);
    clReleaseMemObject(buf5b);

	clReleaseKernel(kernel);
	clReleaseProgram(program);
	clReleaseCommandQueue(queue);
	clReleaseContext(context);

	free(IMG1a);
	free(IMG1b);
    free(IMG2a);
	free(IMG2b);
    free(IMG3a);
	free(IMG3b);
    free(IMG4a);
	free(IMG4b);
	free(IMG5a);
	free(IMG5b);

	return 0;
}
