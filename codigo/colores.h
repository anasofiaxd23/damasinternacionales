#pragma once

#if defined(__MINGW32__) || defined(__MINGW64__)
#include <cstdio>
inline void colorRojo() { std::fputs("\x1b[31m", stdout); }
inline void colorAzul() { std::fputs("\x1b[34m", stdout); }
inline void fondoOscuro() { std::fputs("\x1b[40m", stdout); }
inline void fondoClaro() { std::fputs("\x1b[47m", stdout); }
inline void resetColor() { std::fputs("\x1b[0m", stdout); }

#elif defined(_WIN32)
#include <windows.h>
inline HANDLE hConsola() { static HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE); return h; }
inline WORD COLOR_DEFECTO = [](){ CONSOLE_SCREEN_BUFFER_INFO info; GetConsoleScreenBufferInfo(hConsola(), &info); return info.wAttributes; }();
inline void setFG(WORD fg) {
    CONSOLE_SCREEN_BUFFER_INFO info; GetConsoleScreenBufferInfo(hConsola(), &info);
    WORD bg = info.wAttributes & (BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY);
    SetConsoleTextAttribute(hConsola(), fg | bg);
}
inline void setBG(WORD bg) {
    CONSOLE_SCREEN_BUFFER_INFO info; GetConsoleScreenBufferInfo(hConsola(), &info);
    WORD fg = info.wAttributes & (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    SetConsoleTextAttribute(hConsola(), fg | bg);
}
inline void colorRojo() { setFG(FOREGROUND_RED | FOREGROUND_INTENSITY); }
inline void colorAzul() { setFG(FOREGROUND_BLUE | FOREGROUND_INTENSITY); }
inline void fondoOscuro() { setBG(0); }
inline void fondoClaro() { setBG(BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY); }
inline void resetColor() { SetConsoleTextAttribute(hConsola(), COLOR_DEFECTO); }

#else
inline void colorRojo() {}
inline void colorAzul() {}
inline void fondoOscuro() {}
inline void fondoClaro() {}
inline void resetColor() {}
#endif