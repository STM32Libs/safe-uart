
#include "suart.h"
#include "crc.h"

static void suart_donothing(uint8_t *data,uint8_t size) {};

void suart::uart_ticker()
{
    if(is_getting_late)
    {
        if(bufi != 0)
        {
            txt_message_received(ubuf+1,bufi-1);//remove first char to widen second char options to all characters
            bufi = 0;
        }
    }
    else
    {
        is_getting_late = 1;
    }
}


// msg_type , size , data , crc
//            ----------->
void suart::uart_callback() 
{
    //Uart_Int = 1;
    char c = ser->getc();
    if(bufi < sizeof(ubuf))
    {
        ubuf[bufi] = c;
        if(bufi == 0)       //message type : 'b' binary, 'c' binary with CRC, others : text
        {
            msg_type = c;
            bufi++;
        }
        else if(bufi == 1) //message size depending on type
        {
            if((msg_type == 'b') | (msg_type == 'c'))
            {
                ud_size = c;
            }
            else
            {
                ud_size = 255;//unknown, stops with timeout
            }
            bufi++;
        }
        else if(bufi == ud_size)
        {
            if(msg_type == 'b')
            {
                bin_message_received(ubuf+1,bufi-1);//remove message type identifier
                bufi = 0;
            }
        }
        else if(bufi == ud_size+2)
        {
            if(msg_type == 'c')
            {
                if(crc::check(ubuf+1))
                {
                    bin_message_received(ubuf+1,bufi-1);//remove message type identifier
                }
                else
                {
                    ser->printf("UART> CRC Error\n");
                }
                bufi = 0;
            }
        }
        else
        {
            bufi++;
        }
    }
    else//ovewrite last char
    {
        bufi = sizeof(ubuf)-1;
    }
    is_getting_late = 0;
}

suart::suart(Serial *ps):
            ser(ps)
{
    bufi = 0;
    ud_size = 0;
    is_getting_late = 0;
    msg_type = 't';
    txt_message_received = suart_donothing;
    bin_message_received = suart_donothing;
    ser->attach(callback(this,&suart::uart_callback));
    tick.attach(callback(this,&suart::uart_ticker),0.1);
}

void suart::attach_txt(Callback<void(uint8_t *data,uint8_t size)> func)
{
    txt_message_received = func;
}

void suart::attach_bin(Callback<void(uint8_t *data,uint8_t size)> func)
{
    bin_message_received = func;
}