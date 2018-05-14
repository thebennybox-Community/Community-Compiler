#ifndef TERMINAL_H
#define TERMINAL_H

#ifdef _WIN32
#include <windows.h>
#include <wchar.h>
#else
#include <sys/ioctl.h>
#include <unistd.h>
#endif

namespace TermColour {
enum Name {
    Black,
    Red,
    Green,
    Yellow,
    Blue,
    Magenta,
    Cyan,
    White,
    Grey,
};
}

static constexpr const char *const term_fg[] = {
    "\x1B[30m",
    "\x1B[31m",
    "\x1B[32m",
    "\x1B[33m",
    "\x1B[34m",
    "\x1B[35m",
    "\x1B[36m",
    "\x1B[37m",
    "\x1B[90m",
};

static constexpr const char *const term_bg[] = {
    "\x1B[40m",
    "\x1B[41m",
    "\x1B[42m",
    "\x1B[43m",
    "\x1B[44m",
    "\x1B[45m",
    "\x1B[46m",
    "\x1B[47m",
    "",
};

static constexpr const char *const term_reset     = "\x1B[0m";
static constexpr const char *const term_bold      = "\x1B[1m";
static constexpr const char *const term_dim       = "\x1B[2m";
static constexpr const char *const term_underline = "\x1B[4m";
static constexpr const char *const term_reverse   = "\x1B[7m";

static void get_term_size(int *lines, int *columns) {
    *columns = 80;
    *lines   = 24;

#ifdef _WIN32
    /* Create a handle to the console screen. */
    HANDLE console = CreateFileW(L"CONOUT$", GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
    if(console == INVALID_HANDLE_VALUE) {
        return 1;
    }

    CONSOLE_SCREEN_BUFFER_INFO info;
    if(GetConsoleScreenBufferInfo(console, &info) == 0) {
        return 1;
    }

    CloseHandle(console);
    *columns = info.srWindow.Right  - info.srWindow.Left + 1;
    *lines   = info.srWindow.Bottom - info.srWindow.Top  + 1;
#else
    #ifdef TIOCGSIZE
        struct ttysize ts;
        ioctl(STDIN_FILENO, TIOCGSIZE, &ts);
        *columns = ts.ts_cols;
        *lines   = ts.ts_lines;
    #elif defined(TIOCGWINSZ)
        struct winsize ts;
        ioctl(STDIN_FILENO, TIOCGWINSZ, &ts);
        *columns = ts.ws_col;
        *lines   = ts.ws_row;
    #endif /* TIOCGSIZE */
#endif /* _WIN32 */
}

#endif /* TERMINAL_H */
