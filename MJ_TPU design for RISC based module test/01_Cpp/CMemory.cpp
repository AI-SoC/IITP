#include "CMemory.h"

void CSRAM_256W::show_mems(unsigned int start_addr, unsigned int end_addr) {
	cout << "--- Memory Dump (addr: " << (int)start_addr << "~" << (int)end_addr << ")" << endl;
	for(unsigned int i=start_addr; i<=end_addr; i++) {
		cout << m_mems[i] << " ";
	}
	cout << endl;

}

