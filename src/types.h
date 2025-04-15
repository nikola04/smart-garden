#ifndef TYPES_H
#define TYPES_H

#include "stdint.h"

typedef uint64_t uint64;
typedef uint32_t uint;
typedef uint16_t ushort;
typedef uint8_t uchar;

#ifndef __cplusplus
    typedef int bool;
    #define true 1
    #define false 0
#endif


#endif // TYPES_H
