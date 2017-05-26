
#include "suart.h"

void suart::uart_ticker()
{
    if(is_getting_late)
    {
        if(bufi != 0)
        {
            ser->printf("UART reset counter got (%d)\n",bufi);
            ser->printf("0x");
            for(int i=0;i<bufi;i++)
            {
                ser->printf(" %02x",ubuf[i]);
            }
            ser->printf("\n");
            bufi = 0;
            //timeout = 0;
        }
    }
    else
    {
        is_getting_late = 1;
        //timeout = 1;
    }
    if(update)
    {
        uint8_t c1,c2;
        c1 = val / 256;
        c2 = val % 256;
        ser->printf("Servo %d at 0x%02x %02x\n",s_id,c1,c2);
        update = 0;
    }
}


void suart::uart_callback() 
{
    //Uart_Int = 1;
    char c = ser->getc();
    if(bufi < 32)
    {
        ubuf[bufi] = c;
        if(bufi == 0)
        {
            ud_size = ubuf[0];
            bufi++;
        }
        else if(bufi == ud_size+1)//size + 2x CRC
        {
            message_received(ubuf);
            bufi = 0;
        }
        else
        {
            bufi++;
        }
    }
    else//ovewrite last char
    {
        bufi = 0;
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
    update = 0;
    ser->attach(callback(this,&suart::uart_callback));
    tick.attach(callback(this,&suart::uart_ticker),0.1);
}

void suart::attach(Callback<void(uint8_t *data)> func)
{
    message_received = func;
}