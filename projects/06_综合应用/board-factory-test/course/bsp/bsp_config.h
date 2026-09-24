#ifndef __BSP_CONFIG_H__
#define __BSP_CONFIG_H__

#include <STC89C5xRC.H>
#include "config.h"

sbit HC_SCK = P3^6;
sbit HC_RCK = P3^5;
sbit HC_DIN = P3^4;

#define LEDs_PIN P0
#define LEDo_PIN P2

typedef unsigned int u16;	//对系统默认数据类型进行重定义
typedef unsigned char u8;
typedef unsigned long u32;

void delay_10us(u16 ten_us);
void delay_ms(u16 ms);



void bsp_init(void);
void bsp_apps_init(void);
	
#endif

