#ifndef COMMON_H
#define COMMON_H

#include <iostream>

#ifdef DEBUG
    #define DBG(x)  std::cout<<x<<std::endl
#else
    #define DBG(x)
#endif

#define GG_BUTTON_MAX_SOUND_LIB     5

#endif // COMMON_H
