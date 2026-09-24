#ifndef __LEDS_H__
#define __LEDS_H__

#include "bsp_config.h"

typedef struct
{
    unsigned char ledx;
    unsigned char ledy;
    unsigned char ledca;
    unsigned char leds_buff[8];
} Leds_TypeDef;

extern Leds_TypeDef Leds_t;

//extern unsigned char dispy;
//extern unsigned char dispup;

void leds_init(void);
void hc_send_byte(unsigned char send);
void leds_demo(void);
void leds_app_up(void);

#endif
