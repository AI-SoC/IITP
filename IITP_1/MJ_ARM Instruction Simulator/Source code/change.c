#include<stdio.h>
#include<stdlib.h>
char* binary(int a);

   char bin[7]={0};
int main(int argc, char **argv){

    int i=0,num, address=0;
    FILE *fi = NULL;
    FILE *fo = NULL;
    char c[2];
    fi=fopen(argv[1],"rb");
   
   if(argc != 3)
    {   printf("Input Style Error\n");
        return 0;
     }   
    
    if(fi == NULL){
        printf("Cannot Read\n");
        return 0;
    }
    printf("%s\n",(char*)argv[2]);


    if(argv[2][1]=='h')
      {
        printf("1\n");
        fo=fopen("hex.bin","w");
        printf("1-Success\n");        
        
         while(!feof(fi)){

            if(i==0)
                fprintf(fo,"%.7x: ",address);
            if(i%2==1 )
            {    fprintf(fo,"%.2x ",getc(fi));
                 i++;
            }
            else if(i%2==0)
            {    fprintf(fo,"%.2x",getc(fi));
                 i++;
            }
            if(i>14){
                fprintf(fo,"%.2x\n",getc(fi));
                i=0;
                address++;    
            }
            address++;    
        }
    
    fclose(fi);
    fclose(fo);
    printf("Success\n");
    }
       
    else if (argv[2][1]=='b'){
      fo=fopen("binary.bin","w");
      printf("2-Success\n");
      while(!feof(fi)){
        num=(int)getc(fi);
        binary(num);
        if(i==0){
            fprintf(fo,"%d%d%d%d%d%d%d%d",bin[7],bin[6],bin[5],bin[4],bin[3],bin[2],bin[1],bin[0]);
            i++;
            address++;
            }
        else
        {
            i=0;
            fprintf(fo,"%d%d%d%d%d%d%d%d",bin[7],bin[6],bin[5],bin[4],bin[3],bin[2],bin[1],bin[0]);
            address++;
        }
       }    
         
        fclose(fi);
        fclose(fo);
        printf("Success and End\n");

    }
}

char* binary(int a){
    bin[7]=(char)(a/128);
    a%=128;
    bin[6]=(char)(a/64);
    a%=64;
    bin[5]=(char)(a/32);
    a%=32;
    bin[4]=(char)(a/16);
    a%=16;
    bin[3]=(char)(a/8);
    a=a%8;
    bin[2]=(char)(a/4);
    a=a%4;
    bin[1]=(char)(a/2);
    a=a%2;
    bin[0]=(char)a;

  
} 
