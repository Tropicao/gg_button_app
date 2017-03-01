#ifndef COMMON_H
#define COMMON_H

#include <iostream>

#ifdef DEBUG
    #define DBG(...) {fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\n"); fflush(stderr);}
#else
    #define DBG(...)
#endif

#define GG_BUTTON_MAX_SOUND_LIB     5

#endif // COMMON_H
