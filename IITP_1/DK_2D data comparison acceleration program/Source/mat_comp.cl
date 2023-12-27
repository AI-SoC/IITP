__kernel void mat_com(__global float *A,__global float *B,int num,int DIFF)
{
        int i, j;
        float dif;

        for(i=0;i<num-1;i++)
        {
            for(j=0;j<5550;j++)
            {
                dif = A[i*5550+j]-A[(i+1)*5550+j];
                if((dif>((-1)*DIFF))||(dif<DIFF)) 
                {   
                    B[(i+1)*5550+j] = 0;
                }
                else
                {
                    B[(i+1)*5550+j] = A[(i+1)*5550+j];
                }
            }
       }
}
