#include "CDecode.h"
#include <math.h>

bool CT1DecodeDirectFetch::do_fetch_from(int PC) {
    int j=0;
    if(PC >= 0 && PC < m_code_memory.code_memory_size() ) {
        m_inst_buffer1 = m_code_memory.code_at(PC); 
    //Decoding ECC

    Buffer1[0] = m_inst_buffer1[0] ^ m_inst_buffer1[1] ^ m_inst_buffer1[2] ^ m_inst_buffer1[3] ^ m_inst_buffer1[4] ;
    //P5
    Buffer1[1] = m_inst_buffer1[6] ^ m_inst_buffer1[7] ^ m_inst_buffer1[8] ^ m_inst_buffer1[9] ^ m_inst_buffer1[10] ^ m_inst_buffer1[11] ^ m_inst_buffer1[12];
    //P4
    Buffer1[2] = m_inst_buffer1[0] ^ m_inst_buffer1[1] ^ m_inst_buffer1[6] ^ m_inst_buffer1[7] ^ m_inst_buffer1[8] ^ m_inst_buffer1[9] ^ m_inst_buffer1[14] ^ m_inst_buffer1[15] ^ m_inst_buffer1[16];
    //P3
    Buffer1[3] = m_inst_buffer1[2] ^ m_inst_buffer1[3] ^ m_inst_buffer1[6] ^ m_inst_buffer1[7] ^ m_inst_buffer1[10] ^ m_inst_buffer1[11] ^ m_inst_buffer1[14] ^ m_inst_buffer1[15] ^ m_inst_buffer1[18];
    //P2
    char a = m_inst_buffer1[0] ^ m_inst_buffer1[2] ^ m_inst_buffer1[4] ^ m_inst_buffer1[6] ^ m_inst_buffer1[8]; 
    char b = m_inst_buffer1[10] ^ m_inst_buffer1[12] ^ m_inst_buffer1[14] ^ m_inst_buffer1[16] ^ m_inst_buffer1[18];
    
    if(a==b)
        Buffer1[4] = '0';
    else
        Buffer1[4] = '1';
    //P1
    Buffer1[5] = '\0';
    cout << " Decoded Buffer1 : " << Buffer1 <<endl;

    for(int i=0,k=0; i<21; i++){
        if(i == 5 || i==13 || i==17 || i==19 || i==20){
            Buffer[j]=m_inst_buffer1[i];
            j++;
        }
        else{
                m_inst_buffer[k]=m_inst_buffer1[i];
                k++;
            }
    }// put inst_buffer
        m_inst_buffer[16]='\0';
        cout << " m_inst_buffer1 : " << m_inst_buffer1 <<endl;
        cout << " m_inst_buffer  : " << m_inst_buffer <<endl;
        cout << " Decoded Buffer : " << Buffer <<endl;


    Syndrome = 0;
    for(int i=0; i<5; i++){
        if(Buffer[4-i] != Buffer1[4-i])
            Syndrome += pow(2,i);
    } 
    Syndrome = 21-Syndrome;
    cout << " Syndrome : " << Syndrome << endl;

    if(Syndrome != 21)
    {
        cout << "Error! " << endl;
        if(m_inst_buffer1[Syndrome]=='0')
            m_inst_buffer1[Syndrome]='1';
        else
            m_inst_buffer1[Syndrome]='0';

        for(int i=0,k=0; i<21; i++){
            if((i == 5 || i==13 || i==17 || i==19 || i==20))
                k=k;
            else{
                m_inst_buffer[k]=m_inst_buffer1[i];
                k++;
            }
        }
        
//Decoding END
    }

    cout << " new m_inst_buffer  : " << m_inst_buffer <<endl;
    return true;
            
    }


    else
        return false;
}

bool CT1DecodeDirectFetch::do_decode() {

    int decoded = 0;


    /// Decoding OPCODE
    if(m_inst_buffer[0] == '1')
        decoded |= 8; // b 1000
    if(m_inst_buffer[1] == '1')
        decoded |= 4; // b 0100
    if(m_inst_buffer[2] == '1')
        decoded |= 2; // b 0010
    if(m_inst_buffer[3] == '1')
        decoded |= 1; // b 0001

    m_instruction.OPCODE = decoded;


    decoded = 0;
    // Decoding OP1
    if(m_inst_buffer[4] == '1')
        decoded |= 8; // b 1000
    if(m_inst_buffer[5] == '1')
        decoded |= 4; // b 0100
    if(m_inst_buffer[6] == '1')
        decoded |= 2; // b 0010
    if(m_inst_buffer[7] == '1')
        decoded |= 1; // b 0001

    m_instruction.OP1 = decoded;

    decoded = 0;
    // Decoding OP2
    if(m_inst_buffer[8] == '1')
        decoded |= 128;// b 1000 0000 
    if(m_inst_buffer[9] == '1')
        decoded |= 64; // b 0100 0000
    if(m_inst_buffer[10] == '1')
        decoded |= 32; // b 0010 0000
    if(m_inst_buffer[11] == '1')
        decoded |= 16; // b 0001 0000 
    if(m_inst_buffer[12] == '1')
        decoded |= 8; //  b 0000 1000
    if(m_inst_buffer[13] == '1')
        decoded |= 4; //  b 0000 0100
    if(m_inst_buffer[14] == '1')
        decoded |= 2; //  b 0000 0010
    if(m_inst_buffer[15] == '1')
        decoded |= 1; //  b 0000 0001

    m_instruction.OP2 = decoded;


    return true;
}

void CT1DecodeDirectFetch::show_instruction() {
    if(m_instruction.OPCODE == MOV3)  {
        cout << "MOV3 " << "R" << m_instruction.OP1 << ", #" << m_instruction.OP2 << endl;
    } else if(m_instruction.OPCODE == ADD )  {
		unsigned int op2 = (m_instruction.OP2 >> 4) & 0xF; // Masking [3:0]
        cout << "ADD  " << "R" << m_instruction.OP1 << ", R" << op2 << endl;
    } else if(m_instruction.OPCODE == SUB )  {
		unsigned int op2 = (m_instruction.OP2 >> 4) & 0xF; // Masking [3:0]
        cout << "SUB  " << "R" << m_instruction.OP1 << ", R" << op2 << endl;
    } else if(m_instruction.OPCODE == MOV0)  {
		unsigned int op2 = m_instruction.OP2 & 0xFF; // Masking [7:0]
        cout << "MOV0 " << "R" << m_instruction.OP1 << ", [" << op2 << "]" << endl;
    } else if(m_instruction.OPCODE == MOV1)  {
		unsigned int op2 = m_instruction.OP2 & 0xFF; // Masking [7:0]
        cout << "MOV1 " << "[" << op2 << "], R" << m_instruction.OP1 << endl;
    } else if(m_instruction.OPCODE == MUL)  {
		unsigned int op2 = (m_instruction.OP2 >> 4) & 0xF; // Masking [3:0]
		cout << "MUL " << "R" << m_instruction.OP1 << ", R" << op2 << endl;
	}
}
