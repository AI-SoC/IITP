#include <iostream>
#include "CDecode.h"
#include "CRegister.h"
#include "CMemory.h"

#pragma once

using namespace std;

class CExecute {
public:
    CExecute() { }
    virtual ~CExecute() { }

};

class CT1ExecuteTinyUnit: public CExecute {
public:
    CT1ExecuteTinyUnit(CT1DecodeDirectFetch& decode, 
                       C16RegisterFile& regs, 
                       CSRAM_256W& mems) 
        : m_decode_unit(decode), m_regs(regs), m_mems(mems) { }
    virtual ~CT1ExecuteTinyUnit() { }

    bool do_execute();
private:
    
    CT1DecodeDirectFetch& m_decode_unit;
    C16RegisterFile&      m_regs;
    CSRAM_256W&           m_mems;

    
};



