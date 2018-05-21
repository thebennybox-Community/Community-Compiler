#include <fstream>
#include <iostream>
#include <vector>
#include "DuskAssembly.h"

#ifdef _WIN32
#include <windows.h>
#endif

int main(int argc, char **argv) {

    if(argc < 3) {
        printf("Missing filename in args.\n");
        return 1;
    }

#ifdef _WIN32
    {
        // Set output mode to handle virtual terminal sequences
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

        if(hOut == INVALID_HANDLE_VALUE) {
            return GetLastError();
        }

        DWORD dwMode = 0;

        if(!GetConsoleMode(hOut, &dwMode)) {
            return GetLastError();
        }

        dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;

        if(!SetConsoleMode(hOut, dwMode)) {
            return GetLastError();
        }
    }
#endif

    DuskAssembly ds;

    for(int i = 2; i < argc; i++) {
        ds.queue_file(argv[i]);
    }

    ds.compile_write_binary(argv[1]);

    return 0;
}
