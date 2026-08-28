#pragma once

#ifdef DEBUG

    #include <stdio.h>
    #include <stdarg.h>

    static inline void log_handler(const char* func, int linha, const char* fmt, ...) {
        fprintf(stderr, "[LOG] %s (linha %d): ", func, linha);
        
        va_list args;
        va_start(args, fmt);
        vfprintf(stderr, fmt, args);
        va_end(args);
        
        fprintf(stderr, "\n");
    }

    #define LOG(fmt, ...) log_handler(__func__, __LINE__, fmt __VA_OPT__(,) __VA_ARGS__)

#else

    #define LOG(fmt, ...) do {} while(0)

#endif

#define sucesso LOG("executado")