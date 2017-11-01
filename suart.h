
#ifndef __UART__
#define  __UART__

#include "mbed.h"


class suart
{
    public:
        suart(Serial *ps);
        void uart_ticker();
        void uart_callback();
        void attach_txt(Callback<void(uint8_t *data,uint8_t size)> func);
        void attach_bin(Callback<void(uint8_t *data,uint8_t size)> func);
    public:
        Serial      *ser;
        Ticker      tick;
        uint8_t ubuf[128];
        uint8_t bufi;
        uint8_t ud_size;
        uint8_t is_getting_late;
        char msg_type;
        uint8_t update;
        uint8_t s_id;
        int16_t val;
        Callback<void(uint8_t *data,uint8_t size)> txt_message_received;
        Callback<void(uint8_t *data,uint8_t size)> bin_message_received;
        
};

#endif /*__UART__*/