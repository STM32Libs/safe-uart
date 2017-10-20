
#include "suart.h"
#include "crc.h"

void suart::uart_ticker()
{
    if(is_getting_late)
    {
        if(bufi != 0)
        {
            /*ser->printf("UART reset counter got (%d)\n",bufi);
            ser->printf("0x");
            for(int i=0;i<bufi;i++)
            {
                ser->printf(" %02x",ubuf[i]);
            }
            ser->printf("\n");
            */
            message_received(ubuf,bufi);
            bufi = 0;
            //timeout = 0;
        }
    }
    else
    {
        is_getting_late = 1;
        //timeout = 1;
    }
}


void suart::uart_callback() 
{
    //Uart_Int = 1;
    char c = ser->getc();
    if(bufi < sizeof(ubuf))
    {
        ubuf[bufi] = c;
        if(bufi == 0)
        {
            ud_size = ubuf[0];
            bufi++;
        }
        else if(bufi == ud_size+1)//size + 2x CRC
        {
            if(crc::check(ubuf))
            {
                message_received(ubuf,bufi);
            }
            else
            {
                ser->printf("UART> CRC Error\n");
            }
            bufi = 0;
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
    //timeout = 0;

    //Uart_Int = 0;
}

suart::suart(Serial *ps):
            ser(ps)
{
    bufi = 0;
    ud_size = 0;
    is_getting_late = 0;
    ser->attach(callback(this,&suart::uart_callback));
    tick.attach(callback(this,&suart::uart_ticker),0.1);
}

void suart::attach(Callback<void(uint8_t *data,uint8_t size)> func)
{
    message_received = func;
}