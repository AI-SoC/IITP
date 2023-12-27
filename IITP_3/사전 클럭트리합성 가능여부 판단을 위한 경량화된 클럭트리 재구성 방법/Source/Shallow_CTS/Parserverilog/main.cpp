#include "ot_parser.h"

int main(const int argc, const char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: ./sample_parser verilog_file\n";
        return EXIT_FAILURE;
    }

    if (!std::filesystem::exists(argv[1])) {
        return EXIT_FAILURE;
    }

    OpenTimerParser parser;
    parser.read(argv[1]);
    parser.print();
    
    parser.route();             // original source�� ���� route
    parser.route_print();       // original source�� ���� route ���� print
    
    parser.Pre_route();         

    parser.CTS_route();         // CTS ������ �ϱ� ���� route
    parser.CTS_route_print();   // CTS route ���� ���

    
    parser.PRE_CTS_PRINT();
    parser.PRE_CTS_FILE_PRINT();
    
    return EXIT_SUCCESS;
}