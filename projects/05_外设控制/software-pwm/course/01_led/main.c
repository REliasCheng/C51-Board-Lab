#include <REGX52.H>
#include "key.h"
unsigned char count,compare=0;
unsigned char key_val;
sbit LED1=P2^0;
sbit LED2=P2^1;

void Timer0_Init(void)		//100微秒@11.0592MHz
{
	//AUXR &= 0x7F;			//定时器时钟12T模式
	TMOD &= 0xF0;			//设置定时器模式
	TMOD |= 0x01;			//设置定时器模式
	TL0 = 0xA4;				//设置定时初始值
	TH0 = 0xFF;				//设置定时初始值
	TF0 = 0;				//清除TF0标志
	TR0 = 1;				//定时器0开始计时
	ET0 = 1;				//使能定时器0中断
	EA = 1;//使能中断
	PT0 = 0;//中断优先级
}

void main()
{
	Timer0_Init();
	while(1)
	{
		key_val=Key_Scan();
		if(key_val==1)
		{
			compare+=20;
			if(compare>=100) compare=0;
		}
	}
}

void Timer0_Isr(void) interrupt 1
{
	TL0 = 0xA4;				//设置定时初始值
	TH0 = 0xFF;				//设置定时初始值
	count++;//每隔100us自动加一
	if(count>=100) count=0;
	if(count<compare)
	{
		LED1=0;
	}
	else
	{
		LED1=1;
	}
}
