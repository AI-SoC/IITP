// Serial Port Handler in Windows
// Author : Seunghyun Park

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    FILE*            fp;
    HANDLE           hComm;
    char             ComPortName[32];
    char             fileName[128];
    unsigned char    tok;
    char             ch;
    DWORD            dCOUNT;
    BOOL             STATUS;
    int              line = 0;

    printf( "\n\n====================================================" );
    printf( "\n|              Serial Transmission                 |" );
    printf( "\n====================================================\n\n" );
    
    printf( "[INFO] COM port ex) \\\\\\\\.\\\\COM7 : " );
    scanf( "%s", ComPortName );
    hComm = CreateFileA( ComPortName,                  // port name
                         GENERIC_READ | GENERIC_WRITE, // Read / Write
                         0,                            // No sharing
                         NULL,                         // No security
                         OPEN_EXISTING,                // Open existing port only
                         0,                            // Non overlapped I/O
                         NULL                          // Null for comm devices
                         );

    if( hComm == INVALID_HANDLE_VALUE )
        printf( "[INFO] Error in opening serial port\n" );
    else
        printf( "[INFO] Opening serial port successfully\n" );

    DCB dcbSerialParams = { 0 };
    dcbSerialParams.DCBlength = sizeof( dcbSerialParams );
    
    if( !GetCommState(hComm, &dcbSerialParams) )
        printf( "[INFO] Error in getting information of serial port\n" );
    else
        printf( "[INFO] Getting comm information successfully\n" );

    dcbSerialParams.BaudRate = CBR_2400;                // set baud rate to 2400
    dcbSerialParams.ByteSize = 8;                       // set byte size to 8 bits
    dcbSerialParams.StopBits = ONESTOPBIT;              // set stopbits to 1
    dcbSerialParams.Parity   = NOPARITY;                // set parity to none

    if( !SetCommState(hComm, &dcbSerialParams) )
        printf( "[INFO] Error in setting information of serial port\n" );
    else
        printf( "[INFO] Setting DCB structure successfully\n" );

    COMMTIMEOUTS timeouts = { 0 };
    timeouts.ReadIntervalTimeout         = 50;          // mili second, max read interval
    timeouts.ReadTotalTimeoutConstant    = 50;          // mili second, total read time
    timeouts.ReadTotalTimeoutMultiplier  = 10;          // increase timeout propotional to total number
    timeouts.WriteTotalTimeoutConstant   = 50;          // mili second, max write interval
    timeouts.WriteTotalTimeoutMultiplier = 10;          // increase timeout propotional to total number

    if( !SetCommTimeouts(hComm, &timeouts) )
        printf( "[INFO] Error in setting timeout information\n" );
    else
        printf( "[INFO] Setting serial port timeouts successfully\n" );

    printf( "[INFO] Binary file name ex) uart_test.bin : ");
    scanf( "%s", fileName );
    if( ( fp = fopen(fileName, "rb") ) == NULL )
        printf( "[INFO] Error in opening binary file\n" );
    else
        printf( "[INFO] Opening binary file successfully\n" );

    printf( "\n" );
    while( ( ch = fgetc(fp) ) != EOF ) {
        if( !(line % 16) )
            printf( "\n" );
        tok = (unsigned char) ch;
        STATUS = WriteFile( hComm,
                            &ch,
                            1,
                            &dCOUNT,
                            NULL
                            );
        if( STATUS == TRUE )
            printf( "%02x ", tok );
        else
            printf( "\n\n[INFO] Error in writing data to serial port" );
        Sleep(5);
        line++;
    }

    fclose( fp );
    CloseHandle( hComm );
    printf( "\n\n[INFO] UART transmission done" );
    printf( "\n====================================================\n" );

    return 0;
}
