#include "main.hpp"
#include "LLIST_ECG.hpp"


#define WIN_CLK
#ifdef  WIN_CLK
#include <windows.h>
#include <process.h>
#endif


using namespace std;


namespace gpu {
/*{{{*/
    vector<cl::Device> get_gpu_devices( const string& vendor_name ) {
/*{{{*/
        unsigned int            i;
        cl_int                  err;
        vector<cl::Platform>    platforms;

        OCL_CHECK(err, err=cl::Platform::get(&platforms));

        // Get platform
        cl::Platform    platform;
        for( i = 0; i < platforms.size(); i++ ) {
            platform    = platforms[i];
            OCL_CHECK( err, string platform_name =
                platform.getInfo<CL_PLATFORM_NAME>(&err) );

            if( platform_name == vendor_name ) {
                cout << "Found Platform" << endl;
                cout << "Platform name: " << platform_name.c_str() << endl;
                break;
            }
        }

        // Get devices
        vector<cl::Device> devices;
        OCL_CHECK( err, err=platform.getDevices(
                CL_DEVICE_TYPE_GPU, &devices) );

        return devices;
/*}}}*/
    }

    vector<cl::Device> get_gpu_device() {
        return get_gpu_devices( "NVIDIA Corporation" );
    }

/*}}}*/
};

int main( int argc, char* argv[] ) {

    //
    // Check arguments
    //
/*{{{*/
    if( argc != 3 ) {
        cout << "Usage: main.exe [DB] [R_peak]" << endl;
        return -1;
    }

    ifstream    fDB( argv[1] );
    if( !fDB.is_open() ) {
        cout << "Cannot open " << argv[1] << endl;
        return -1;
    }

    ifstream    fPeak( argv[2] );
    if( !fPeak.is_open() ) {
        cout << "Cannot open " << argv[2] << endl;
        return -1;
    }
/*}}}*/

    //
    // Data insertion
    //
/*{{{*/
    int     line    = -1;
    float   temp    = 0;
    while( fPeak >> temp ) {
        line++;
    }

    int     index   = 0;
    int     peak    = 0;
    int     p_loc   = 0;
    float   p_max   = 0;
    float** data    = (float**)malloc(sizeof(float*)*line);
    for( int i = 0; i < line; i++ )
        data[i] = (float*)malloc(sizeof(float)*DLENGTH);

    float   array[DLENGTH+2*TS] = {0};

    fPeak.clear();
    fPeak.seekg(0, ios::beg);
    fPeak >> peak;      // Discard first peak
    for( int i = 0; i < line; i++ ) {
    //for( int i = 0; i < 3; i++ ) {
        fPeak >> peak;
        if( fPeak.eof() ) break;

        // Data shift
        while( index < (peak+SR+TS-1) ) {
            for( int j = 1; j < DLENGTH+2*TS; j++ ) {
                array[j-1] = array[j];
            }
            fDB >> array[DLENGTH+2*TS-1];
            index++;
        }

        // peak correction
        p_max   = 0;
        p_loc   = 0;
        for( int j = SL-2; j < SL+2*TS+2; j++ ) {
            if( p_max < array[j] ) {
                p_max   = array[j];
                p_loc   = j;
            }
        }

        peak    = index - (DLENGTH+2*TS) + p_loc;

        temp    = 0;        // for data offset
        for( int j = 0; j < DLENGTH; j++ ) {
            data[i][j]  = array[p_loc-SL+j];
            temp       += data[i][j];
        }
        temp   /= DLENGTH;

        for( int j = 0; j < DLENGTH; j++ ) {
            data[i][j] -= temp;
        }
    }

    fPeak.close();
    fDB.close();
/*}}}*/

#ifdef  PRINT01
/*{{{*/
    ofstream    f01("01_data20.txt");
    f01.setf(ios::fixed);
    f01.precision(1);
    for( int i = 0; i < 100; i++ ) {
        for( int j = 0; j < DLENGTH; j++ ) {
            f01 << j << " " << data[i][j] << endl;
        }
        f01 << endl;
    }
    f01.unsetf(ios::fixed);
    f01.close();
/*}}}*/
#endif

    //
    // Template learning
    //
/*{{{*/
    LLIST   abnormal;

    for( int i = 0; i < line; i++ ) {
        if( !abnormal.train_data(&data[i]) ) {
            cout << "Data training error in " << i
                << endl;
            return -1;
        }
    }
/*}}}*/

#ifdef  PRINT02
/*{{{*/
    ofstream    f02("02_tpl.txt");
    abnormal.printList( f02 );
    f02.close();
/*}}}*/
#endif

    //
    // Divide TPL
    //
/*{{{*/
    LLIST   normal;

    if( !abnormal.divide_tpl( normal ) ) {
        cout << "Divide error" << endl;
        return -1;
    }
/*}}}*/

#ifdef  PRINT03
/*{{{*/
    ofstream    f03_1("03_normal_tpl.txt");
    ofstream    f03_2("03_abnormal_tpl.txt");
    normal.printList( f03_1 );
    abnormal.printList( f03_2 );
    f03_1.close();
    f03_2.close();
/*}}}*/
#endif

    //
    // Dynamic programming 
    //
/*{{{*/
    DLIST   d_norm;
    DLIST   d_abno;

    d_norm.dynamic_programming( normal );
    d_abno.dynamic_programming( abnormal );

    d_norm.bubble_sort_dp();
    d_abno.bubble_sort_dp();
/*}}}*/

#ifdef  PRINT04
/*{{{*/
    ofstream    f04_1("04_norm_dp.txt");
    ofstream    f04_2("04_abno_dp.txt");
    d_norm.printList( f04_1 );
    d_abno.printList( f04_2 );
    f04_1.close();
    f04_2.close();
/*}}}*/
#endif


#ifdef  WIN_CLK
    /*{{{*/
    LARGE_INTEGER   freq;
    LARGE_INTEGER   start;
    LARGE_INTEGER   end;
    double          interval;

    QueryPerformanceFrequency( &freq );
    QueryPerformanceCounter( &start );
/*}}}*/
#endif
    //
    // Create Context, command queue, build program, kernel
    //
    cl_int              err;
    vector<cl::Device>  devices = gpu::get_gpu_device();
    cl::Device          device  = devices[0];

    OCL_CHECK( err, cl::Context context(device, NULL, NULL, NULL, &err) );
    OCL_CHECK( err, cl::CommandQueue queue(context, device, NULL, &err) );

    ifstream    fKern("kernels.cl");
    string      kSrc( (istreambuf_iterator<char>(fKern)), istreambuf_iterator<char>() );
    cl::Program::Sources    srcs(1, make_pair(kSrc.c_str(), kSrc.length()) );

    OCL_CHECK( err, cl::Program program(context, srcs, &err) );
    err = program.build(NULL,NULL,NULL);
    if( err != CL_SUCCESS ) {
        string  buildLog;
        buildLog    = program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device, &err);
        cout << "Build log: " << buildLog << endl;
    }

    OCL_CHECK( err, cl::Kernel kernel1(program, "diag", &err) );
    OCL_CHECK( err, cl::Kernel kernel2(program, "diag", &err) );


    //
    //  Set kernel memory and execute
    //
    int d_cnt   = PAR;
    int n_cnt   = d_norm.get_count();
    int a_cnt   = d_abno.get_count();

    int*    n_index = (int*  )malloc(sizeof(int  )*NN*n_cnt);
    float*  n_data  = (float*)malloc(sizeof(float)*NN*n_cnt);
    float*  n_res   = (float*)malloc(sizeof(float)*PAR);

    d_norm.get_index( n_index );
    d_norm.get_data ( n_data  );

    int*    a_index = (int*  )malloc(sizeof(int  )*NN*a_cnt);
    float*  a_data  = (float*)malloc(sizeof(float)*NN*a_cnt);
    float*  a_res   = (float*)malloc(sizeof(float)*PAR);

    d_abno.get_index( a_index );
    d_abno.get_data ( a_data  );

    float*  d_data  = (float*)malloc(sizeof(float)*DLENGTH*PAR);

    // kern1
    OCL_CHECK( err, cl::Buffer buf_n_index(context, CL_MEM_READ_ONLY, 
        sizeof(int  )*NN*n_cnt, NULL, &err) );
    OCL_CHECK( err, cl::Buffer buf_n_data (context, CL_MEM_READ_ONLY, 
        sizeof(float)*NN*n_cnt, NULL, &err) );
    OCL_CHECK( err, cl::Buffer buf_n_res  (context, CL_MEM_WRITE_ONLY, 
        sizeof(float)*PAR,      NULL,   &err) );

    // kern2
    OCL_CHECK( err, cl::Buffer buf_a_index(context, CL_MEM_READ_ONLY, 
        sizeof(int  )*NN*a_cnt, NULL, &err) );
    OCL_CHECK( err, cl::Buffer buf_a_data (context, CL_MEM_READ_ONLY, 
        sizeof(float)*NN*a_cnt, NULL, &err) );
    OCL_CHECK( err, cl::Buffer buf_a_res  (context, CL_MEM_WRITE_ONLY, 
        sizeof(float)*PAR,      NULL,   &err) );

    OCL_CHECK( err, cl::Buffer buf_d_data (context, CL_MEM_READ_ONLY, 
        sizeof(float)*DLENGTH*PAR,NULL,&err) );

    // set Arg
    OCL_CHECK( err, err = kernel1.setArg(0, n_cnt      ) );
    OCL_CHECK( err, err = kernel1.setArg(1, buf_n_index) );
    OCL_CHECK( err, err = kernel1.setArg(2, buf_n_data ) );
    OCL_CHECK( err, err = kernel1.setArg(3, buf_d_data ) );
    OCL_CHECK( err, err = kernel1.setArg(4, buf_n_res  ) );

    OCL_CHECK( err, err = kernel2.setArg(0, a_cnt      ) );
    OCL_CHECK( err, err = kernel2.setArg(1, buf_a_index) );
    OCL_CHECK( err, err = kernel2.setArg(2, buf_a_data ) );
    OCL_CHECK( err, err = kernel2.setArg(3, buf_d_data ) );
    OCL_CHECK( err, err = kernel2.setArg(4, buf_a_res  ) );


    // Transmit data to kernel
    OCL_CHECK( err, err = queue.enqueueWriteBuffer( buf_n_index, 
        CL_FALSE, 0, sizeof(int  )*NN*n_cnt, (void*)n_index, NULL, NULL) );
    OCL_CHECK( err, err = queue.enqueueWriteBuffer( buf_n_data, 
        CL_FALSE, 0, sizeof(float)*NN*n_cnt, (void*)n_data , NULL, NULL) );
    OCL_CHECK( err, err = queue.enqueueWriteBuffer( buf_a_index, 
        CL_FALSE, 0, sizeof(int  )*NN*a_cnt, (void*)a_index, NULL, NULL) );
    OCL_CHECK( err, err = queue.enqueueWriteBuffer( buf_a_data, 
        CL_FALSE, 0, sizeof(float)*NN*a_cnt, (void*)a_data , NULL, NULL) );

    cl::NDRange gl( PAR );
    cl::NDRange lo( 1 );

    ofstream    fout("diag.txt");

    for( int k = 0; k <= (line/PAR); k++ ) {
    //for( int k = 0; k < 2; k++ ) {
        // Set data
        for( int i = 0; i < PAR; i++ ) {
            for( int j = 0; j < DLENGTH; j++ ) {
                if( i+k*PAR < line )
                    d_data[i*DLENGTH+j] = data[i+k*PAR][j];
                else
                    d_data[i*DLENGTH+j] = 0;
            }
        }

        // Send data
        OCL_CHECK( err, err = queue.enqueueWriteBuffer(buf_d_data,
            CL_FALSE, 0, sizeof(float)*DLENGTH*PAR, (void*)d_data, NULL, NULL) );


        OCL_CHECK( err, err = queue.enqueueNDRangeKernel(
            kernel1, 0, gl, lo, NULL, NULL) );
        OCL_CHECK( err, err = queue.enqueueNDRangeKernel(
            kernel2, 0, gl, lo, NULL, NULL) );

        OCL_CHECK( err, err = queue.enqueueReadBuffer(buf_n_res,
            CL_FALSE, 0, sizeof(float)*PAR, (void*)n_res, NULL, NULL) );
        OCL_CHECK( err, err = queue.enqueueReadBuffer(buf_a_res,
            CL_FALSE, 0, sizeof(float)*PAR, (void*)a_res, NULL, NULL) );


#ifdef  WIN_CLK
#else
        for( int i = 0; i < PAR; i++ ) {
            if( n_res[i] > a_res[i] ) {
//                cout << i << " n " << n_res[i] << " a " << a_res[i] << endl;
                fout << "V" << endl;
            }
            else if( d_data[i*DLENGTH] == 0) {
                
            }
            else {
//                cout << i << " n " << n_res[i] << " a " << a_res[i] << endl;
                fout << "N" << endl;
            }
        }
#endif
    }   // For loop

#ifdef  WIN_CLK

    QueryPerformanceCounter( &end );
    interval    = (double)(end.QuadPart - start.QuadPart) / freq.QuadPart;

    cout << "Duration: " << interval << endl;

#endif



    return 0;
}
