#define NUM 20
#define PAR 5
const unsigned int vertex = NUM;
const unsigned int parall = PAR;


extern "C" {
    void geterr(
        const int*      index,  // Read-only index value
        const float*    t_data, // Read-only template data
        const float*    angle,  // Read-only template angle
        const float*    s_data, // Read-only source data
        float*          result  // Sum of square output
    ) {
#pragma HLS INTERFACE m_axi     port=index  offset=slave    bundle=gmem
#pragma HLS INTERFACE m_axi     port=t_data offset=slave    bundle=gmem
#pragma HLS INTERFACE m_axi     port=angle  offset=slave    bundle=gmem
#pragma HLS INTERFACE m_axi     port=s_data offset=slave    bundle=gmem
#pragma HLS INTERFACE m_axi     port=result offset=slave    bundle=gmem

#pragma HLS INTERFACE s_axilite port=index                  bundle=control
#pragma HLS INTERFACE s_axilite port=t_data                 bundle=control
#pragma HLS INTERFACE s_axilite port=angle                  bundle=control
#pragma HLS INTERFACE s_axilite port=s_data                 bundle=control
#pragma HLS INTERFACE s_axilite port=result                 bundle=control
#pragma HLS INTERFACE s_axilite port=return                 bundle=control

        int     local_index [parall][vertex];
#pragma HLS ARRAY_PARTITION variable=local_index    dim=1 complete
        float   local_t_data[parall][vertex];
#pragma HLS ARRAY_PARTITION variable=local_t_data   dim=1 complete
        float   local_angle [parall][vertex];
#pragma HLS ARRAY_PARTITION variable=local_angle    dim=1 complete
        float   local_s_data[parall][vertex];
#pragma HLS ARRAY_PARTITION variable=local_s_data   dim=1 complete
        float   local_res   [parall]            = {0};
#pragma HLS ARRAY_PARTITION variable=local_res      dim=1 complete

        int     i, j;

        // Burst read memory
        readA: for( i = 0; i < parall; i++ ) {
#pragma HLS LOOP_TRIPCOUNT min=vertex*parall max=vertex*parall
#pragma HLS PIPELINE II=1
            for( j = 0; j < vertex; j++ ) {
                local_index [i][j]  = index [i*parall+j];
            }
        }

        readB: for(i = 0; i < parall; i++ ) {
#pragma HLS LOOP_TRIPCOUNT min=vertex*parall max=vertex*parall
#pragma HLS PIPELINE II=1
            for( j = 0; j < vertex; j++ ) {
                local_t_data[i][j]  = t_data[i*parall+j];
            }
        }

        readC: for(i = 0; i < parall; i++ ) {
#pragma HLS LOOP_TRIPCOUNT min=vertex*parall max=vertex*parall
#pragma HLS PIPELINE II=1
            for( j = 0; j < vertex; j++ ) {
                local_angle [i][j]  = angle [i*parall+j];
            }
        }

        readD: for(i = 0; i < parall; i++ ) {
#pragma HLS LOOP_TRIPCOUNT min=vertex*parall max=vertex*parall
#pragma HLS PIPELINE II=1
            for( j = 0; j < vertex; j++ ) {
                local_s_data[i][j]  = s_data[i*parall+j];
            }
        }


        // Perform execution
        float   v_err[parall][vertex];
        systolic1: for(i = 0; i < parall; i++ ) {
#pragma HLS LOOP_TRIPCOUNT min=vertex*parall max=vertex*parall
#pragma HLS PIPELINE II=1
            for( j = 0; j < vertex; j++ ) {
                v_err[i][j] = (local_t_data[i][j] - local_s_data[i][j]);
            }
        }


        float   a_err[parall][vertex] = {0};
        float   aa, bb, cc, cos2;
        //a_err[0] = 0;
        //a_err[vertex-1] = 0;
        systolic2: for( i = 0; i < parall; i++ ) {
#pragma HLS LOOP_TRIPCOUNT min=parall*(vertex-2) max=parall*(vertex-2)
#pragma HLS PIPELINE II=1
            for( j = 1; j < vertex-1; j++ ) {
                aa  = (local_index [i][j+1]-local_index [i][j-1])
                    * (local_index [i][j+1]-local_index [i][j-1])
                    + (local_s_data[i][j+1]-local_s_data[i][j-1])
                    * (local_s_data[i][j+1]-local_s_data[i][j-1]);

                bb  = (local_index [i][j+1]-local_index [i][j  ])
                    * (local_index [i][j+1]-local_index [i][j  ])
                    + (local_s_data[i][j+1]-local_s_data[i][j  ])
                    * (local_s_data[i][j+1]-local_s_data[i][j  ]);

                cc  = (local_index [i][j  ]-local_index [i][j-1])
                    * (local_index [i][j  ]-local_index [i][j-1])
                    + (local_s_data[i][j  ]-local_s_data[i][j-1])
                    * (local_s_data[i][j  ]-local_s_data[i][j-1]);

                cos2= (bb+cc-aa)*(bb+cc-aa)/(4*bb*cc);

                a_err[i][j] = local_angle[i][j] + 3.141593*cos2;
            }
        }

        systolic3: for( i = 0; i < vertex; i++ ) {
#pragma HLS DEPENDENCE variable=local_res intra true
            for( j = 0; j < parall; j++ ) {
                local_res[j] += v_err[j][i]*v_err[j][i] + a_err[j][i]*a_err[j][i];
            }
        }

        // Write result
        writeRES: for( i = 0; i < parall; i++ ) {
#pragma HLS LOOP_TRIPCOUNT min=parall max=parall
#pragma HLS PIPELINE II=1
            result[i] = local_res[i];
        }
    }
}
