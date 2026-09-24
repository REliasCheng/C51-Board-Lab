#include <REGX52.H>
#include "delay.h"
sbit SER=P3^4; 
sbit SCK=P3^6;
sbit RCK=P3^5;

//低位在下
void MxLED_WriteByte(unsigned char Byte)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		SER=(Byte&0x80>>i);
		SCK=1;
		SCK=0;
	}
	RCK=1;
	RCK=0;
}
	

//column: 列号0~7 从左向右
void MxLED_ShowColumn(unsigned char column,unsigned char ucmxled)
{
	MxLED_WriteByte(ucmxled);
	P0= ~(0x80>>column);
	Delay1ms(1);
	P0=0xFF;
}

void main()
{
	SCK=0;
	RCK=0;
	while(1)
	{
		//0x3C,0x42,0xA9,0x85,0x85,0xA9,0x42,0x3C,
		MxLED_ShowColumn(0,0x3C);
		MxLED_ShowColumn(1,0x42);
		MxLED_ShowColumn(2,0xA9);
		MxLED_ShowColumn(3,0x85);
		
		MxLED_ShowColumn(4,0x85);
		MxLED_ShowColumn(5,0xA9);
		MxLED_ShowColumn(6,0x42);
		MxLED_ShowColumn(7,0x3C);
	}
}

