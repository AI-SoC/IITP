
#include <iostream>

#pragma once

using namespace std;

class CRegister {
public:
    CRegister() { }
    virtual ~CRegister() { }

};

class C16RegisterFile : public CRegister {
public:
	C16RegisterFile() {}
	virtual ~C16RegisterFile() {}

	void write_on_reg(unsigned int index, int data) { m_regs[index] = data; }
	int read_from_reg(unsigned int index)           { return m_regs[index]; }

	void show_regs();
private:
	int m_regs[16];
};



