float cal_angle( float y1, float y2, float y3 ) {
    float   aa  = 100 + (y3-y1)*(y3-y1);
    float   bb  = 100 + (y3-y2)*(y3-y2);
    float   cc  = 100 + (y2-y1)*(y2-y1);

    float   cos2 = (bb+cc-aa)*(bb+cc-aa)/(4*bb*cc);

    return -3.141593*cos2;
}

__kernel void diag(          const int      tpl_cnt,
                    __global const int*     tpl_index,
                    __global const float*   tpl_data,
                    __global const float*   data,
                    __global       float*   res         )
{
    int     i = get_global_id(0);   // PAR
    int     j, k;

    float   verr    = 0.0f;
    float   aerr    = 0.0f;

    float   temp    = 0.0f;
    for( j = 0; j < tpl_cnt; j++ ) {
        for( k = 0; k < 20; k++ ) {
            temp  = (tpl_data[j*20+k] - data[i*234 + tpl_index[j*20+k]]);
            verr += temp*temp;
            if( (k!=0) && (k!=19) ) {
                temp  = (cal_angle( tpl_data[j*20+k-1],
                                    tpl_data[j*20+k  ],
                                    tpl_data[j*20+k+1])
                        -cal_angle( data[i*234 + tpl_index[j*20+k-1]],
                                    data[i*234 + tpl_index[j*20+k  ]],
                                    data[i*234 + tpl_index[j*20+k+1]]));
                aerr += temp*temp;
            }
        }
    }

    verr /= tpl_cnt;
    aerr /= tpl_cnt;

    res[i]  = verr + aerr;
}
