
#ifndef __UART__
#define  __UART__

#include "mbed.h"

class suart
{
    public:
        suart(Serial *ps);
        void uart_ticker();
        void uart_callback();
        void attach(Callback<void(uint8_t *data)> func);
    public:
        Serial      *ser;
        Ticker      tick;
        uint8_t ubuf[32];
        uint8_t bufi;
        uint8_t ud_size;
        uint8_t is_getting_late;
        uint8_t update;
        uint8_t s_id;
        int16_t val;
        Callback<void(uint8_t *data)> message_received;
        
};

#endif /*__UART__*/