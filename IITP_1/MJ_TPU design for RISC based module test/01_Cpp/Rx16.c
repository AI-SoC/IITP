#include<stdio.h>
//RX
int main(){
	FILE *fp, *ffp;
	char data;
	int num[13];
	int i,index;
	int syn[4];

	fp = fopen("ECC_Data.txt","r");
	ffp = fopen("Corrected_Data.txt","w");
	while(1){
		for(i=0;i<13;i++)
		{
			fscanf(fp,"%c",&data);
			num[i]=data-48;
		}
		if(num[11]==-38)
			break;
		syn[0] = num[0] ^ num[1] ^ num[2] ^ num[3];
		syn[1] = num[0] ^ num[5] ^ num[6] ^ num[7];
		syn[2] = num[1] ^ num[2] ^ num[5] ^ num[6] ^ num[9];
		syn[3] = num[1] ^ num[3] ^ num[5] ^ num[7] ^ num[9];
		index=0;
		if(syn[0] != num[4])
			index=index+8;
		if(syn[1] != num[8])
			index=index+4;
		if(syn[2] != num[10])
			index=index+2;
		if(syn[3] != num[11])
			index=index+1;
		if(index!=0)
			if(num[12-index]==0)
				num[12-index]=1;
			else
				num[12-index]=0;
	
		for(i=0;i!=11;i++)
			printf("%d",num[i]);
		printf("\n");	
		for(i=0;i<12;i++)
			fprintf(ffp,"%d",num[i]);
		fprintf(ffp,"\n");
	}
}
