#define CL_HPP_CL_1_2_DEFAULT_BUILD
#define CL_HPP_TARGET_OPENCL_VERSION 120
#define CL_HPP_MINIMUM_OPENCL_VERSION 120
#define CL_HPP_ENABLE_PROGRAM_CONSTRUCTION_FROM_ARRAY_COMPATIBILITY 1
#define CL_USE_DEPRECATED_OPENCL_1_2_APIS

//////////////////////////////////////////////////
// OpenCL header
#include <iostream>
#include <fstream>
#include <CL/cl2.hpp>
#include <CL/cl_ext_xilinx.h>
#include <vector>
#include <unistd.h>

#include "LLIST_ECG.h"

#define PAR 5

using namespace std;

template <typename T>
struct aligned_allocator {
    using value_type = T;
    T* allocate(int num) {
        void* ptr = nullptr;
        if( posix_memalign(&ptr, 4096, num*sizeof(T)) )
            throw bad_alloc();

        return reinterpret_cast<T*>(ptr);
    }

    void deallocate( T* p, int num ) {
        free(p);
    }
};

#define OCL_CHECK(error,call)                                       \
    call;                                                           \
    if (error != CL_SUCCESS) {                                      \
      printf("%s:%d Error calling " #call ", error code is: %d\n",  \
              __FILE__,__LINE__, error);                            \
      exit(EXIT_FAILURE);                                           \
    }  

namespace xcl {
    vector<cl::Device> get_devices(const string& vendor_name) {
        unsigned int            i;
        cl_int                  err;
        vector<cl::Platform>    platforms;

        OCL_CHECK(err, err = cl::Platform::get(&platforms));

        cl::Platform platform;
        for( i = 0; i < platforms.size(); i++ ) {
            platform    = platforms[i];
            OCL_CHECK( err, string platformName =
                platform.getInfo<CL_PLATFORM_NAME>(&err) );

            if( platformName == vendor_name ) {
                printf( "Found Platform\n" );
                printf( "Platform name: %s\n", platformName.c_str() );
                break;
            }
        }

        // Get device and select 1st device
        vector<cl::Device> devices;
        OCL_CHECK( err, err = platform.getDevices(
            CL_DEVICE_TYPE_ACCELERATOR, &devices) );

        return devices;
    }

    vector<cl::Device> get_xil_devices() {
        return get_devices( "Xilinx" );
    }

    char* read_binary_file(
        const string&   xclbin_file_name,
        unsigned int&   nb )
    {
        cout << "INFO: Reading " << xclbin_file_name << endl;

        if( access(xclbin_file_name.c_str(), R_OK) != 0 ) {
            cout << "ERROR: " << xclbin_file_name.c_str() 
                << " xclbin not available. Please build it" << endl;
            exit(EXIT_SUCCESS);
        }

        cout << "Loading: '" << xclbin_file_name.c_str() << "'\n";
        ifstream    bin_file(xclbin_file_name.c_str(), ifstream::binary);

        bin_file.seekg( 0, bin_file.end );
        nb = bin_file.tellg();
        bin_file.seekg( 0, bin_file.beg );

        char*   buf = new char[nb];
        bin_file.read( buf, nb );

        cout << "Success to read " << xclbin_file_name.c_str() << endl;

        return buf;
    }
};

int dynamic_programming( LLIST* list, DLIST* dp );
void    get_dp_angle( DLIST* list );
float   get_sum_err( DLIST* list, float* data );

int main( int argc, char* argv[] ) {
    // Check arguments
    if( argc != 4 ) {
        printf( "Usage: %s <XCLBIN> <DB> <PEAK>\n", argv[0] );
        return -1;
    }

    FILE* fDB = fopen( argv[2], "r" );
    if( fDB == NULL ) {
        printf( "Cannot open %s\n", argv[1] );
        return -1;
    }


    FILE* fPeak = fopen( argv[3], "r" );
    if( fPeak == NULL ) {
        printf( "Cannot open %s\n", argv[2] );
        return -1;
    }
    /////////////////////////////////////////////////////////////
    // Data insertion
    int     line = 0;
    float   temp = 0;
    while( 1 ) {
        fscanf( fDB, "%f", &temp );
        if( feof(fDB) ) break;
        line++;
    }

    float*  array   = (float*)malloc(sizeof(float)*line);
    if( !array ) {
        printf( "malloc failed\n" );
        return -1;
    }

    int     index = 0;
    fseek( fDB, 0, SEEK_SET );
    while( 1 ) {
        fscanf( fDB, "%f", &array[index] );
        if( feof(fDB) ) break;
        array[index] -= BASE;
        array[index] /= GAIN;
        index++;
    }

    fclose( fDB );
    /////////////////////////////////////////////////////////////

    int     i;
    int     peak    = 0;
    float   p_max   = 0;
    int     p_loc   = 0;
    float   data[Sl+Sr];
    FILE*   fo = fopen( "t.txt", "w" );

    LLIST*  list    = create_list();
    if( !list ) {
        printf( "Create list failed\n" );
        return -1;
    }
    // discard first peak
    fscanf( fPeak, "%d", &peak );
    while(1) {
        fscanf( fPeak, "%d", &peak );
        if( feof(fPeak) ) break;

        // Peak correction
        p_max   = 0;
        p_loc   = 0;
        for( i = peak-TS; i < peak+TS; i++ ) {
            if( p_max < array[i] ) {
                p_max   = array[i];
                p_loc   = i;
            }
        }
        peak    = p_loc;

        // get data
        index = peak-Sl;
        for( i = 0; i < Sl+Sr; i++ ) {
            data[i] = array[index++];
        }

        // Add data to list
        if( !add_data( list, data, fo ) ) {
            printf( "Add data failed\n" );
            return -1;
        }
    }

    printf( "list %d\n", list->count );
    // Divide N/A tpl
    LLIST*  normal  = create_list();

    if( !divide_tpl( list, normal ) ) {
        printf( "TPL divide error\n" );
        return -1;
    }

    printf( "list %d\n", list->count );
    printf( "normal %d\n", normal->count );

#ifdef DEBUG
    //printf( "List count = %d\n", list->count );
    //print_tpl_count( list );
    //print_tpl_count( normal );
    FILE* fout = fopen( "normal.txt", "w" );
    print_tpl_data( fout, normal, 0 );
    fclose( fout );
    FILE* fout2 = fopen( "Abnormal.txt", "w" );
    print_tpl_data( fout2, list, 0 );
    fclose( fout2 );
#endif

    // Dynamic programming
    DLIST*  d_norm  = create_dplist();
    DLIST*  d_abno  = create_dplist();

    dynamic_programming( normal, d_norm );
    delete_list( normal );

    dynamic_programming( list, d_abno );
    delete_list( list );

#ifdef DEBUG
//    for( i = 0; i < N; i++ )
//        printf( "%d %f\n", d_norm->front->index[i], d_norm->front->data[i] );
    FILE* fout3 = fopen( "n_dp1.txt", "w" );
    print_dp_data( fout3, d_norm->front );
    fclose(fout3);
    FILE* fout4 = fopen( "a_dp1.txt", "w" );
    print_dp_data( fout4, d_abno->front );
    fclose(fout4);
    print_dp_count( d_abno );
    printf( "\n" );
#endif

    // Sorting
    bubble_sort_dp( d_norm );
    bubble_sort_dp( d_abno );

    /////////////////////////////////////
    // Diagnosis
    fseek( fPeak, 0, SEEK_SET );
    fscanf( fPeak, "%d", &peak );


    // Set hardware platform
    cl_int              err;
    vector<cl::Device>  devices = xcl::get_xil_devices();
    cl::Device          device  = devices[0];

    // Create context
    OCL_CHECK( err, cl::Context context(device, NULL, NULL, NULL, &err) );
    OCL_CHECK( err, cl::CommandQueue q(context, device,
        CL_QUEUE_PROFILING_ENABLE, &err) );
    OCL_CHECK( err, string device_name = device.getInfo<CL_DEVICE_NAME>(&err) );

    unsigned int    fileBufSize;
    char*   fileBuf = xcl::read_binary_file( argv[1], fileBufSize );
    cl::Program::Binaries bins{{fileBuf, fileBufSize}};
    devices.resize(1);

    //cout << "Binary success" << endl;

    OCL_CHECK( err, cl::Program program(context, devices, bins, NULL, &err) );
    OCL_CHECK( err, cl::Kernel krnl_systolic_array(program, "geterr", &err) );


    get_dp_angle( d_norm );
    get_dp_angle( d_abno );

    // Set kernel memory
    int mat_size    = N*PAR;
    vector<int,   aligned_allocator<int  >> tpl_index   (mat_size);    
    vector<float, aligned_allocator<float>> tpl_data    (mat_size);
    vector<float, aligned_allocator<float>> tpl_angle   (mat_size);
    vector<float, aligned_allocator<float>> src_data    (mat_size);
    vector<float, aligned_allocator<float>> result      (PAR);

    OCL_CHECK( err, cl::Buffer buffer_index (context, CL_MEM_USE_HOST_PTR
        | CL_MEM_READ_ONLY, sizeof(int)*mat_size, tpl_index.data(), &err) );
    OCL_CHECK( err, cl::Buffer buffer_t_data(context, CL_MEM_USE_HOST_PTR
        | CL_MEM_READ_ONLY, sizeof(float)*mat_size, tpl_data.data(), &err) );
    OCL_CHECK( err, cl::Buffer buffer_angle (context, CL_MEM_USE_HOST_PTR
        | CL_MEM_READ_ONLY, sizeof(float)*mat_size, tpl_angle.data(), &err) );
    OCL_CHECK( err, cl::Buffer buffer_s_data(context, CL_MEM_USE_HOST_PTR
        | CL_MEM_READ_ONLY, sizeof(float)*mat_size, src_data.data(), &err) );
    OCL_CHECK( err, cl::Buffer buffer_result(context, CL_MEM_USE_HOST_PTR
        | CL_MEM_WRITE_ONLY, sizeof(float)*PAR, result.data(), &err) );

    OCL_CHECK( err, err = krnl_systolic_array.setArg(0, buffer_index ) );
    OCL_CHECK( err, err = krnl_systolic_array.setArg(1, buffer_t_data) );
    OCL_CHECK( err, err = krnl_systolic_array.setArg(2, buffer_angle ) );
    OCL_CHECK( err, err = krnl_systolic_array.setArg(3, buffer_s_data) );
    OCL_CHECK( err, err = krnl_systolic_array.setArg(4, buffer_result) );

    //cout << "Ready to start kernel" << endl;

    float   no_err;
    float   ab_err;
    float   mean;
    FILE*   fout6 = fopen( "diag.txt", "w" );
    int     k;
    //while(1) {
    for( int l = 0; l < 10; l++ ) {
        no_err = 0;
        ab_err = 0;

        fscanf( fPeak, "%d", &peak );
        if( feof(fPeak) ) break;
        //cout << k << "th loop" << endl;

        // Peak correction
        p_max   = 0;
        p_loc   = 0;
        for( i = peak-TS; i < peak+TS; i++ ) {
            if( p_max < array[i] ) {
                p_max   = array[i];
                p_loc   = i;
            }
        }
        peak    = p_loc;

        // get data
        index = peak-Sl;
        mean  = 0;
        for( i = 0; i < Sl+Sr; i++ ) {
            data[i] = array[index++];
            mean   += data[i];
        }

        // data offset
        mean /= (Sl+Sr);
        for( i = 0; i < Sl+Sr; i++ ) {
            data[i] -= mean;
        }

        // Diagnosis
        d_norm->pos = d_norm->front;
        no_err      = 0;
        while( d_norm->pos != NULL ) {
            // Set data to buffer
            for( k = 0; k < PAR; k++ ) {
                if( d_norm->pos != NULL ) {
                    for( i = 0; i < N; i++ ) {
                        tpl_index [k*N+i] = d_norm->pos->index[i];
                        tpl_data  [k*N+i] = d_norm->pos->data [i];
                        tpl_angle [k*N+i] = d_norm->pos->angle[i];
                        src_data  [k*N+i] = data[ tpl_index[k*N+i] ];
                    }
                    d_norm->pos = d_norm->pos->next;
                }
                else {
                    for( i = 0; i < N; i++ ) {
                        tpl_index [k*N+i] = d_norm->front->index[i];
                        tpl_data  [k*N+i] = d_norm->front->data [i];
                        tpl_angle [k*N+i] = d_norm->front->angle[i];
                        src_data  [k*N+i] = d_norm->front->data [i];
                    }
                }
            }

            // Send data to kernel
            OCL_CHECK( err, err = q.enqueueMigrateMemObjects(
                {buffer_index, buffer_t_data, buffer_angle,
                buffer_s_data}, 0) );

            // Execute kernel
            OCL_CHECK( err, err = q.enqueueTask(krnl_systolic_array) );

            // Get data from kernel
            OCL_CHECK( err, err = q.enqueueMigrateMemObjects(
                {buffer_result}, CL_MIGRATE_MEM_OBJECT_HOST) );

            q.finish();
            for( i = 0; i < PAR; i++ ) {
                no_err += result[i];
            }
            //d_norm->pos = d_norm->pos->next;
        }
        no_err /= (d_norm->count);

        d_abno->pos = d_abno->front;
        ab_err      = 0;
        while( d_abno->pos != NULL ) {
            for( k = 0; k < PAR; k++ ) {
                if( d_abno->pos != NULL ) {
                    for( i = 0; i < N; i++ ) {
                        tpl_index [k*N+i] = d_abno->pos->index[i];
                        tpl_data  [k*N+i] = d_abno->pos->data [i];
                        tpl_angle [k*N+i] = d_abno->pos->angle[i];
                        src_data  [k*N+i] = data[ tpl_index[k*N+i] ];
                    }
                    d_abno->pos = d_abno->pos->next;
                }
                else {
                    for( i = 0; i < N; i++ ) {
                        tpl_index [k*N+i] = d_abno->front->index[i];
                        tpl_data  [k*N+i] = d_abno->front->data [i];
                        tpl_angle [k*N+i] = d_abno->front->angle[i];
                        src_data  [k*N+i] = d_abno->front->data [i];
                    }
                }
            }

            // Send data to kernel
            OCL_CHECK( err, err = q.enqueueMigrateMemObjects(
                {buffer_index, buffer_t_data, buffer_angle,
                buffer_s_data}, 0) );

            // Execute kernel
            OCL_CHECK( err, err = q.enqueueTask(krnl_systolic_array) );

            // Get data from kernel
            OCL_CHECK( err, err = q.enqueueMigrateMemObjects(
                {buffer_result}, CL_MIGRATE_MEM_OBJECT_HOST) );

            q.finish();
            for( i = 0; i < PAR; i++ ) {
                ab_err += result[i];
            }
            //d_abno->pos = d_abno->pos->next;
        }
        ab_err /= (d_abno->count);
        /*
        no_err  = get_sum_err( d_norm, data );
        ab_err  = get_sum_err( d_abno, data );
        */
        //cout << "no_err " << no_err << " ab_err " << ab_err << endl;
        if( no_err > ab_err )
            fprintf( fout6, "V\n" );
        else
            fprintf( fout6, "N\n" );

        no_err = 0;
        ab_err = 0;
    }
    fclose( fout6 );

    free( array );
    fclose( fPeak );

    delete_dplist( d_norm );
    delete_dplist( d_abno );

    return 0;
}

float cal_angle( int x1, float y1, int x2, float y2, int x3, float y3 ) {
    float   aa  = powf( x3-x1, 2 ) + powf( y3-y1, 2 );
    float   bb  = powf( x3-x2, 2 ) + powf( y3-y2, 2 );
    float   cc  = powf( x2-x1, 2 ) + powf( y2-y1, 2 );
    float   res;

    float   cos2 = (bb+cc-aa)*(bb+cc-aa)/(4*bb*cc);
    
    res         = -3.141593*cos2;
/*
    float   cosa = (bb+cc-aa)/(2*sqrtf(bb)*sqrtf(cc));


    if( fabs(cosa+1.0) < 0.000001 )
        res = 3.141593;
    else if( fabs(cosa-1.0) < 0.000001 )
        res = 0;
    else
        res = acosf( cosa );

*/
    return res;
}

void get_dp_angle( DLIST* list ) {
    int i;
    DP* pos = list->front;
    while( pos != NULL ) {
        for( i = 0; i < N; i++ ) {
            if( (i==0) | (i==N-1) ) {
                pos->angle[i]   = 0;
            }
            else {
                pos->angle[i]   = cal_angle(pos->index[i-1], pos->data[i-1],
                                            pos->index[i  ], pos->data[i  ],
                                            pos->index[i+1], pos->data[i+1] );
            }
        }
        pos = pos->next;
    }
}

void get_verr( DP* dp, float* data, float* res ) {
    int i;

    for( i = 0; i < N; i++ ) {
        res[i] = dp->data[i] - data[ dp->index[i] ];
    }
}

void get_aerr( DP* dp, float* data, float* res ) {
    int i;
    int x, y, z;
    float tmp;

    for( i = 0; i < N; i++ ) {
        if( (i==0) | (i==N-1) ) {
            res[i] = 0;
        }
        else {
            x   = dp->index[i-1];
            y   = dp->index[i  ];
            z   = dp->index[i+1];

            tmp = cal_angle( x, data[x], y, data[y], z, data[z] );
            res[i] = dp->angle[i] - tmp;
        }
    }
}

float get_sum_err( DLIST* list, float* data ) {
    int     i;
    float   sum = 0;
    float   v_err[N];
    float   a_err[N];
    DP*     pos = list->front;
    FILE*   fv = fopen( "v_err.txt", "w" );
    FILE*   fa = fopen( "a_err.txt", "w" );

    while( pos != NULL ) {
        get_verr( pos, data, v_err );
        get_aerr( pos, data, a_err );

        for( i = 0; i < N; i++ ) {
            fprintf( fv, "%f ", v_err[i] );
            fprintf( fa, "%f ", a_err[i] );
            sum += v_err[i]*v_err[i];
            sum += a_err[i]*a_err[i];
        }
        fprintf( fv, "\n" );
        fprintf( fa, "\n" );
        pos = pos->next;
    }
    sum /= (list->count);

    return sum;
}

float p2l_dist( float x1, float x2, float xp, float y1, float y2, float yp ) {
    float   lx1 = x1/Fs;
    float   lx2 = x2/Fs;
    float   lxp = xp/Fs;
    float   dist;
    
    dist = fabs( (lx2-lx1)*(y1-yp) - (lx1-lxp)*(y2-y1) )
            / sqrtf( powf((lx2-lx1),2) + powf((y2-y1),2) );

    return dist;
}

float func_d( float* data, int from, int to, float dist_weight ) {
    int     i;
    float   temp = 0;

    for( i = from; i <= to; i++ ) {
        temp += fabs( p2l_dist( from, to, i, data[from], data[to], data[i] ) );
    }

    return dist_weight * temp;
}

int dynamic_programming( LLIST* list, DLIST* dp ) {
    int     len     = Sl + Sr;
    int     vertex  = N - 2;
    int     i, j, dep, col, tmp_pos;

    float*  data;
    float   tmp, tmp_min;

    float*  base_mat    = (float*) malloc(sizeof(float)*N_BIT);
    float*  cost_tmp    = (float*) malloc(sizeof(float)*N_BIT);

    float   cost_mat[vertex][len];
    int     way_mat [vertex][len];

    float   output  [N];
    int     way     [N];

    for( i = 0; i < vertex; i++ ) {
        for( j = 0; j < len; j++ ) {
            cost_mat[i][j] = INF;
            way_mat [i][j] = 0;
        }
    }

    float   diff_max, diff_min;
    float   dist_max, dist_weight;

    list->pos = list->front;

    while( list->pos->next != NULL ) {
        data = (list->pos->data);

        // Set distance weight
        diff_max    = -1;
        diff_min    = INF;

        for( i = 0; i < len; i++ ) {
            tmp = p2l_dist( 0, len-1, i, data[0], data[len-1], data[i] );
            if( diff_max < tmp )
                diff_max = tmp;
            else if( diff_min > tmp )
                diff_min = tmp;
        }

        dist_max = (diff_max - diff_min) *
            sqrtf( powf(len-1, 2) + powf(data[len-1] - data[0], 2) );
        dist_weight = floorf(65535/dist_max);


        // Cost temp matrix
        cost_tmp[0] = 0;
        for( i = 1; i < N_BIT; i++ ) {
            cost_tmp[i] = func_d( data, 0, i+1, dist_weight );
        }

        for( col = 3; col < len; col++ ) {
            // Update base matrix
            for( i = max(1, col-N_BIT); i < col; i++ ) {
                base_mat[i-col+N_BIT] = func_d( data, i-1, col-1, dist_weight );
                //printf( "col = %d, i = %d, base[%d] = %f\n", col, i, i-col+N_BIT, base_mat[i-col+N_BIT] );
            }

            for( dep = max(1, col-len+vertex); dep < min(vertex, col-1); dep++ ) {
                tmp_min = INF;
                tmp_pos = dep+1;

                if( dep == 1 ) {
                    for( i = max(dep+1, col-N_BIT); i < col; i++ ) {
                        if( i < 34 ) {
                            //printf( "ct[%d] = %f, c0[%d] = %f\n", i-2, cost_tmp[i-2], i-col+N_BIT, base_mat[i-col+N_BIT] );

                            tmp = cost_tmp[i-2] + base_mat[i-col+N_BIT];

                            if( tmp < tmp_min ) {
                                tmp_min = tmp;
                                tmp_pos = i-1;
                            }
                        }
                    }
                }
                else {
                    for( i = max(dep+1, col-N_BIT); i < col; i++ ) {
                        tmp = cost_mat[dep-2][i-1] + base_mat[i-col+N_BIT];

                        if( tmp < tmp_min ) {
                            tmp_min = tmp;
                            tmp_pos = i-1;
                        }
                    }
                }

                cost_mat[dep-1][col-1] = tmp_min;
                way_mat [dep-1][col-1] = tmp_pos;
            } // dep loop
        } // col loop
        
        col = len;
        dep = vertex;

        for( i = max(1, col-N_BIT); i < col; i++ ) {
            base_mat[i-col+N_BIT] = func_d( data, i, col, dist_weight );
        }

        tmp_min = INF;
        tmp_pos = dep+1;
        for( i = max(dep+1, col-N_BIT); i < col; i++ ) {
            tmp = cost_mat[dep-2][i-1] + base_mat[i-col+N_BIT];

            if( tmp < tmp_min ) {
                tmp_min = tmp;
                tmp_pos = i-1;
            }            
        }
        way_mat[dep-1][col-1] = tmp_pos;
    
    /*
        for( i = 0; i < vertex; i++ ) {
            for( j = 0; j < len; j++ ) {
                //if( cost_mat[i][j] != INF )
                    //printf( "c[%d][%d] = %f\n", i, j, cost_mat[i][j] );

                if( way_mat[i][j] != 0 )
                    printf( "w[%d][%d] = %d\n", i, j, way_mat[i][j] );
            }
        }
        */
        
        // Select shortest way
        way[0] = 0;
        way[N-1] = len-1;

        for( i = vertex; i > 0; i-- )
            way[i] = way_mat[i-1][way[i+1]];

        for( i = 0; i < N; i++ )
            output[i] = data[ way[i] ];

/*
        for( i = 0; i < N; i++ ) {
            printf( "%d = %f\n", way[i], output[i] );
        }
        printf( "\n" );
        */
        

        if( !add_dpdata( dp, list->pos->count, way, output ) )
            return -1;

        list->pos = list->pos->next;
    }


    free( base_mat );
    free( cost_tmp );

    return 0;
}
