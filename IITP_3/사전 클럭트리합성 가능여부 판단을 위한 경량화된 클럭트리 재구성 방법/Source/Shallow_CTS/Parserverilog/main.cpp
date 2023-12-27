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
    
    parser.route();             // original source에 대한 route
    parser.route_print();       // original source에 대한 route 정보 print
    
    parser.Pre_route();         

    parser.CTS_route();         // CTS 과정을 하기 위한 route
    parser.CTS_route_print();   // CTS route 정보 출력

    
    parser.PRE_CTS_PRINT();
    parser.PRE_CTS_FILE_PRINT();
    
    return EXIT_SUCCESS;
}