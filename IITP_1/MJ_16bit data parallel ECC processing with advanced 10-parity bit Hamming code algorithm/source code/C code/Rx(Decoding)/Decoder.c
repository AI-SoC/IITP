#include<stdio.h>

int main(){
	FILE *fp, *ffp;
	char data;
	int num[26];
	int i,index;
	int syn[10];

	fp = fopen("ECC_Data.txt","r");
	ffp = fopen("Corrected_Data.txt","w");

	while(1){
		for(i=0; i<26;i++)
		{
				fscanf(fp,"%c",&data);
				num[i]=data-48;
		}
		if(num[25]==-38)
			break;

		syn[0] = num[2] ^ num[4] ^ num[6] ^ num[8] ^ num[10];

        syn[1] = num[2] ^ num[5] ^ num[6] ^ num[9] ^ num[10];

        syn[2] = num[4] ^ num[5] ^ num[6];

        syn[3] = num[8] ^ num[9] ^ num[10] num[11];

        
		syn[4] = num[14] ^ num[16] ^ num[18];

        syn[5] = num[14] ^ num[17] ^ num[18];

        syn[6] = num[16] ^ num[17] ^ num[18];


        syn[7] = num[21] ^ num[23] ^ num[25];

        syn[8] = num[21] ^ num[24] ^ num[25];

        syn[9] = num[23] ^ num[24] ^ num[25];


		index=0;
		if(syn[0] != num[0])
			index+=1;
		if(syn[1] != num[1])
			index+=2;
		if(syn[2] != num[3])
			index+=4;
		if(syn[3] != num[7])
			index+=8;

		if(index!=0)
			if(num[index]==0)
				num[index]=1;
			else
				num[index]=0;

        index=12;
		if(syn[4] != num[12])
			index+=1;
		if(syn[5] != num[13])
			index+=2;
		if(syn[6] != num[15])
			index+=4;

		if(index!=12)
			if(num[index]==0)
				num[index]=1;
			else
				num[index]=0;

        index=19;
		if(syn[7] != num[19])
			index+=1;
		if(syn[8] != num[20])
			index+=2;
		if(syn[9] != num[22])
			index+=4;

		if(index!=19)
			if(num[index]==0)
				num[index]=1;
			else
				num[index]=0;

		for(i=0;i<26;i++)
			fprintf(ffp,"%d",num[i]);
		fprintf(ffp,"\n");
	}

}

