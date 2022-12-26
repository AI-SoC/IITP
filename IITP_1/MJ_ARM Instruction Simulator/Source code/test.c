#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"sim.h"
#define main_start 202
#define main_end 227 
FILE *fi=NULL;
FILE *fo=NULL;


int main(int argc, char **argv){
    int i=0;
    fi=fopen(argv[1],"rb");
    if(argc !=2){
        printf("Input Error!\n");
        return 0;
        }
    if(fi ==NULL){
        printf("File Reading Error!\n");
        return 0;
    }

    while(!feof(fi))
    {
        read_data();
        decode();
        printf("%s\n",buf);
        printf("program code : %s\t%d\t%d\t%d\n",program[i].OPCODE,program[i].rd,program[i].rb,program[i].offset);
        i++;
    }
    printf("\n\n\nLoad Complete!\n");
    run_program();
}

int read_data(){
    int i;
    for(i=0;i<16;i++)
    {   if(i>7)
            buf[23-i]=getc(fi);
        else if(i<8)
            buf[7-i]=getc(fi);
    }
}
int decode(){
    static int pc=-1;
    int i;
    char opbuf[7];
    for(i=0;i<6;i++)
        opbuf[i]=buf[15-i];
    pc++;
    
    printf("opbuf: %s PC : %d \n",opbuf,pc);
    if(!strcmp(opbuf,"101101")){
        
        strcpy(program[pc].OPCODE, "push");
        program[pc].rd=7;
    }
    else if(!strcmp(opbuf,"101100")){
        if(buf[7]=='0')
            strcpy(program[pc].OPCODE, "add");
        else strcpy(program[pc].OPCODE, "sub");
            program[pc].offset=getoffset(buf, 6,0);
        program[pc].rd=7;
        program[pc].rb=7;
    }
    else if(!strcmp(opbuf,"001000")){
        strcpy(program[pc].OPCODE,"movs");        
        program[pc].rd=getoffset(buf,10,8);
        program[pc].offset=getoffset(buf,7,0);
    }
    else if(!strcmp(opbuf,"011000")){
        strcpy(program[pc].OPCODE,"str");        
        program[pc].rb=getoffset(buf,5,3);
        program[pc].rd=getoffset(buf,2,0);
        program[pc].offset=getoffset(buf,10,6);
    }  
    else if(!strcmp(opbuf,"011010")){
        strcpy(program[pc].OPCODE,"ldr");        
        program[pc].rb=getoffset(buf,5,3);
        program[pc].rd=getoffset(buf,2,0);
        program[pc].offset=getoffset(buf,10,6);
    }
    else if(!strcmp(opbuf,"101011") || !strcmp(opbuf,"101010")){
        strcpy(program[pc].OPCODE,"add1");        
        program[pc].rd=getoffset(buf,10,8);
        program[pc].rb=7;
        program[pc].offset=getoffset(buf,7,0);
    }
    else if(!strcmp(opbuf,"111000")){
        strcpy(program[pc].OPCODE,"bn");        
        program[pc].offset=getoffset(buf,10,0);  
    }
    else if(!strcmp(opbuf,"001011")|| !strcmp(opbuf,"001010")){
         strcpy(program[pc].OPCODE,"cmp");        
        program[pc].rd=getoffset(buf,10,8);  
        program[pc].offset=getoffset(buf,7,0);  
    }
    else if(!strcmp(opbuf,"010001")){
        if(buf[9]=='0' && buf[8]=='0')
            strcpy(program[pc].OPCODE,"add2"); 
        else if(buf[9]=='1' &&buf[8]=='0')
            strcpy(program[pc].OPCODE,"mov");
        else if(buf[9]=='1' && buf[8]== '1'){
            strcpy(program[pc].OPCODE,"bx");
        }
        program[pc].rd=getoffset(buf,2,0);
        program[pc].rb=getoffset(buf,5,3);

    }
    else if(!strcmp(opbuf,"001100") || !strcmp(opbuf,"001101")){
        strcpy(program[pc].OPCODE,"adds");        
        program[pc].rd=getoffset(buf,10,8);
        program[pc].offset=getoffset(buf,7,0);
    }
    else if(!strcmp(opbuf,"110111")){
        strcpy(program[pc].OPCODE,"ble.n");        
        program[pc].offset=-getoffset(buf,6,0);
    }
   
    //ld.w
    //bx
}

int getoffset(char buf[20], int start, int end){
    int offset=0;
    int num=1;
    int i;
    for(i=end;i<=start;i++){
        offset=offset+(buf[i]-48)*num;
        num=num*2;
    }

    return offset;

}

int run_program(){
    int pc = main_start;
    int flag;
    int i;
    char inst[20]={0};
    reg[7]=30;
    printf("\n\n\n\n");
    while(pc<main_end){
        printf("명령어 입력 : ");
        gets(inst);
        
        if(!strcmp(inst,"si"))
        {
          if(!strcmp(program[pc].OPCODE, "push")){
               printf("push!\n");
               stack_memory[reg[program[pc].rd]]=reg[program[pc].rd];
               printf("complet!!\n");
            }
            else if(!strcmp(program[pc].OPCODE, "sub")){
                printf("sub!\n");
                reg[program[pc].rd]=reg[program[pc].rb]-program[pc].offset;
                printf("complet!!\n");
            }
            else if(!strcmp(program[pc].OPCODE, "add"))
                printf("add!\n");
            else if(!strcmp(program[pc].OPCODE, "movs")){
                printf("movs!\n");
                reg[program[pc].rd]=program[pc].offset;    
                printf("complet!!\n");
            }
            else if(!strcmp(program[pc].OPCODE, "str")){
                printf("str!\n");
                stack_memory[reg[program[pc].rb]+program[pc].offset]=reg[program[pc].rd];
                printf("complet!!\n");
            }
            else if(!strcmp(program[pc].OPCODE, "ldr")){
                printf("ldr!\n");
                reg[program[pc].rd]=stack_memory[reg[program[pc].rb]+program[pc].offset];
                printf("complet!!\n");
            }
            else if(!strcmp(program[pc].OPCODE, "add1")){
                printf("add1!\n");
                reg[program[pc].rd]=reg[program[pc].rb]+program[pc].offset;
                printf("complet!!\n");
            }
            else if(!strcmp(program[pc].OPCODE, "bn")){
                printf("bn!\n");
                pc+= program[pc].offset-1;
                printf("complet!!\n");
                continue;
            }
            else if(!strcmp(program[pc].OPCODE, "cmp")){
                printf("cmp\n");
                flag=reg[program[pc].rd]-program[pc].offset;
                if(flag==0)
                    Z=1;
                else Z=0;
            }
            else if(!strcmp(program[pc].OPCODE, "add2")){
                printf("add2!\n");
                reg[program[pc].rd]=reg[program[pc].rd]+reg[program[pc].rb];
                printf("complete\n");    
            }
            else if(!strcmp(program[pc].OPCODE, "mov"))
                printf("mov!\n");
            else if(!strcmp(program[pc].OPCODE, "bx"))
                printf("bx!\n");
            else if(!strcmp(program[pc].OPCODE, "adds")){
                printf("adds!\n");
                reg[program[pc].rd]+=program[pc].offset;
                printf("complete!\n");
            }
            else if(!strcmp(program[pc].OPCODE, "ble.n")){
                printf("ble.n!\n");
                if(Z==0){
                    printf("yeah\n\n");
                    pc=pc+(program[pc].offset+1)/8;
                }
            }
            pc++;
        }
        if(!strcmp(inst,"register"))
            for(i=0;i<16;i++)
                printf(" register[%d] = %d\n",i, reg[i]);
        if(!strcmp(inst,"stack_memory"))
            for(i=30;i>=0;i--)
                printf("---address: %.2d  data : %.2d ---\n",i,stack_memory[i]);

    }
}
