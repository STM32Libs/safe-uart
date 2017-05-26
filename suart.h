
#ifndef __UART__
#define  __UART__

#include "mbed.h"

class suart
{
    public:
        suart(Serial *ps,):
            ser(ps)
    public:
        Serial      *ser;
        
};

#endif /*__UART__*/