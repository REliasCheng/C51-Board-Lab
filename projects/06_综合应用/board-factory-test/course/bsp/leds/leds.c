#include "leds.h"
#include "stdio.h"

void leds_demo(void);



Leds_TypeDef Leds_t;

void leds_init(void)
{
    LEDs_PIN = 0xef; // 点阵屏停止显示
    LEDo_PIN = 0xff;
    Leds_t.ledx = 0;
    Leds_t.ledy = 0;
    Leds_t.ledca = 0;
    Leds_t.leds_buff[8] = 0xff;
    hc_send_byte(0x00);
    hc_send_byte(0x01);
//    while(1)leds_demo();
}

void hc_send_byte(unsigned char send)
{
    unsigned char cnt = 8;
    HC_SCK = 0;
    HC_RCK = 0;
    HC_DIN = 0;
    for (cnt = 0; cnt < 8; cnt++)
    {
        if ((send & (0x80 >> cnt)) != 0)
            HC_DIN = 1;
        else
            HC_DIN = 0;
        HC_SCK = 1;
        HC_SCK = 0;
    }
    HC_RCK = 1;
    HC_RCK = 0;
}



