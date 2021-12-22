#include "CRegister.h"

void C16RegisterFile::show_regs() {
	cout << "--- Register file ---------------" << endl;
	for(int i=0; i<10; i++) {
		cout << " R" << i << ": " << m_regs[i] << endl;
	}
	for(int i=10; i<16; i++) {
		cout << "R" << i << ": " << m_regs[i] << endl;
	}
}

