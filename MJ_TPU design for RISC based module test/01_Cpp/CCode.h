
#include <iostream>

#pragma once

using namespace std;

class CCode {
public: 
    CCode() { }
    virtual ~CCode() { }


private:

};


class CFlash1KWord : public CCode {
public: 
    CFlash1KWord(char* filename, int line);
    virtual ~CFlash1KWord();

    virtual string& code_at(int addr);
    int code_memory_size() { return 1024; }

private:
    string m_filename;
    int m_line;
    string m_code[1024];

};


