#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>

#define move 10
#define threashold 250


int window[50][50];
int data [5800][100];

char* day[3];
char* month[3];
char* date[2];
char* tim[8];
char* year[4];

int datswit = 0;
int counts = 198;

float alltime = 0;

double get_time() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return (double)tv.tv_sec + (double)1e-6 * tv.tv_usec;
}

void convolution(int x,int y)
{
    int amount = 0;

    for(int i=0;i<50;i++)
    {
        for(int j=0;j<50;j++)
        {
            amount += data[x+i][y+j]*window[i][j] ;
        }
    }
    if(amount/100>threashold)
    {
        if(datswit==0)
        {
            printf("%s %s %s %s %s\n",day,month,date,tim,year);
            datswit++;
        }
        printf("Movement Detected at [%d/%d] : %d\n",x/move,y/move,amount/100);
    }
}

int main()
{
    FILE* file = fopen("output.txt","r");
    int buff;

    for(int i=0;i<50;i++)
    {
        for(int j=0;j<50;j++)
        {
            window[i][j] = 1;
        }
    }

    #if 1
    for(int i=10;i<40;i++)
    {
        for(int j=20;j<30;j++)
        {
            window[i][j] = 1;
        }
    }
    #endif

    #if 1
    for(int i=20;i<30;i++)
    {
        for(int j=20;j<30;j++)
        {
            window[i][j] = 1;
        }
    }
    #endif

    fscanf(file,"%s %s %s %s %s",day,month,date,tim,year);

    for(int i=0;i<5800;i++)
    {
        for(int j=0;j<100;j++)
        {
            fscanf(file,"%d",&buff);
            data[i][j] = buff;
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
                data[i][j] = buff;
            }
        }

        for(int i=4;i<53;i++)
        {
            for(int j=0;j<5;j++)
            {
                convolution(i*move,j*move);
            }
        }
        datswit = 0;

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
