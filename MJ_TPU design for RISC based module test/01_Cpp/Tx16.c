#include <stdio.h>
//Tx16  16 to 21
int main(){
	FILE *fp, *ffp;
    char data;
    int num[22];
    int i,j;
/*
    fp = fopen("test.txt", "w");

    fclose(fp);
*/
    fp = fopen("02_Test.bin", "r");
	ffp = fopen("02_Test_ECC.bin","w");
    for(j=0; j<17; j++){
	    for(i=0; i<22 ; i++)
		{	
			fscanf(fp,"%c",&data);
            if(i== 5 || i == 13 || i== 17 || i == 19 || i== 20)
                i++;
			num[i]=data-48;
		}
		
		printf("\n");
		
		if(num[20]==-38)
			break;
		
		// Make ECC
		num[5] = num[0] ^ num[1] ^ num[2] ^ num[3] ^ num[4];

		num[13] = num[6] ^ num[7] ^ num[8] ^ num[9] ^ num[10] ^ num[11] ^ num[12];

		num[17] = num [0] ^ num[1] ^ num[6] ^ num[7] ^ num[8] ^ num[9] ^ num[14] ^ num[15] ^ num[16];
		
		num[19] = num[2] ^ num[3] ^ num[6] ^ num[7] ^ num[10] ^ num[11] ^ num[14] ^ num[15] ^ num[18] ;

		num[20] = num[0] ^ num[2] ^ num[4] ^ num[6] ^ num[8] ^ num[10] ^ num[12] ^ num[14] ^ num[16] ^ num[18];
	
		
		for(i=0;i<21;i++){
			printf("%d",num[i]);
		}
	    
	    for(i = 0 ; i < 21 ; i++){
	        fprintf(ffp, "%d", num[i]);
	    }
	    fprintf(ffp,"\n");
	}    
	    fclose(fp);
	    fclose(ffp);
	    return 0;
}
	
