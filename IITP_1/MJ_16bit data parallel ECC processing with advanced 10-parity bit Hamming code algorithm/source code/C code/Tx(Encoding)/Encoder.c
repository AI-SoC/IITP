#include<stdio.h>
//Tx32
int main(){
	FILE *fp, *ffp;
	char data;
	int num[26];
	int i;

	fp = fopen("Data32.txt","r");
	ffp = fopen("ECC_Data.txt","w");

	while(1){
		for(i=0; i < 26; i++)
			{
				fscanf(fp,"%c",&data);
				if(i==0 || i==1 || i== 3 || i==7 || i==12 || i==13 || i== 15 || i==19  || i== 20 || i==22)
					i+=1;
				printf("%c , %d\n", data, i);
				num[i]=data-48;
			}
		printf("1\n");
		num[0] = num[2] ^ num[4] ^ num[6] ^ num[8] ^ num[10];

        num[1] = num[2] ^ num[5] ^ num[6] ^ num[9] ^ num[10];

        num[3] = num[4] ^ num[5] ^ num[6];

        num[7] = num[8] ^ num[9] ^ num[10] num[11];

        
		num[12] = num[14] ^ num[16] ^ num[18];

        num[13] = num[14] ^ num[17] ^ num[18];

        num[15] = num[16] ^ num[17] ^ num[18];


        num[19] = num[21] ^ num[23] ^ num[25];

        num[20] = num[21] ^ num[24] ^ num[25];

        num[22] = num[23] ^ num[24] ^ num[25];


	
		for(i=0;i<26;i++)
			fprintf(ffp,"%d", num[i]);
		fprintf(ffp,"\n");
		//for(i=0;i<

	}
	fclose(ffp);	
	fclose(fp);	
}
