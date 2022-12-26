#include<stdio.h>
#include<stdlib.h>

typedef struct
{
    char OPCODE[20];
    int rd;
    int rb;
    int offset;
} instruction;
int Z;
static const unsigned int regs=16   ;
instruction program[1024];
int reg[16]={0};
char buf[20];
int stack_memory[32]={0};

int run_program();
int decode();
int getoffset(char buf[20], int start, int end);
int read_data();
// push sub add add1 adds movs str ldr bn add cmp ble  -
