#include "CExecute.h"

bool CT1ExecuteTinyUnit::do_execute() {

    // ex. MOV3 R0, #3
    if(m_decode_unit.get_opcode() == MOV3) {
        unsigned int reg_index = m_decode_unit.get_op1();
                 int      data = m_decode_unit.get_op2();

        m_regs.write_on_reg(reg_index, data);

        return true;
    
      // ex. ADD R0, R1  --> R0 = R0 + R1 
    } else if( m_decode_unit.get_opcode() == ADD) {
        unsigned int reg_n = m_decode_unit.get_op1();
		unsigned int reg_m = (m_decode_unit.get_op2() >> 4) & 0xF; // Masking [3:0]


        int Rn = m_regs.read_from_reg(reg_n);
        int Rm = m_regs.read_from_reg(reg_m);

        Rn = Rn + Rm;
        m_regs.write_on_reg(reg_n, Rn);

        return true;
      
      // ex. SUB R0, R1  --> R0 = R0 - R1 
    } else if( m_decode_unit.get_opcode() == SUB) {
        unsigned int reg_n = m_decode_unit.get_op1();
		unsigned int reg_m = (m_decode_unit.get_op2() >> 4) & 0xF; // Masking [3:0] 

        int Rn = m_regs.read_from_reg(reg_n);
        int Rm = m_regs.read_from_reg(reg_m);

        Rn = Rn - Rm;
        m_regs.write_on_reg(reg_n, Rn);

        return true;


      // ex. MOV0 R1, [3] : R1 <- M[3]
    } else if( m_decode_unit.get_opcode() == MOV0 ) {
        unsigned int reg_n    = m_decode_unit.get_op1();
        unsigned int mem_addr = m_decode_unit.get_op2() & 0xFF; // Masking [7:0]

        int memory_data = m_mems.read_from_memory(mem_addr);

		m_regs.write_on_reg(reg_n, memory_data);

        return true;

      // ex. MOV1 [3], R1 : M[3] <- R1 
    } else if( m_decode_unit.get_opcode() == MOV1 ) {
        unsigned int reg_n    = m_decode_unit.get_op1();
        unsigned int mem_addr = m_decode_unit.get_op2() & 0xFF; // Masking [7:0]

		int Rn = m_regs.read_from_reg(reg_n);

        m_mems.write_on_memory(mem_addr, Rn);

        return true;

      // ex. MUL R1, R2 --> R1 = R1 * R2 
    } else if( m_decode_unit.get_opcode() == MUL ) {
		unsigned int reg_n = m_decode_unit.get_op1();
		unsigned int reg_m = (m_decode_unit.get_op2() >> 4) & 0xF; // Masking [3:0]

        int Rn = m_regs.read_from_reg(reg_n);
        int Rm = m_regs.read_from_reg(reg_m);

        Rn = Rn * Rm;
        m_regs.write_on_reg(reg_n, Rn);

        return true;

	} else {
        cout << "Not executable instruction, not yet implemented, sorry !!. " << endl;
        return false;
    }
}
