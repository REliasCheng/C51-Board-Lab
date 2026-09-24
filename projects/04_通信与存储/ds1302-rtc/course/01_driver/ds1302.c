#include <REGX52.H>
#include "ds1302.h"

sbit DS1302_SCLK=P3^6;
sbit DS1302_IO=P3^4;
sbit DS1302_CE=P3^5;

void DS1302_Init(void)
{
	DS1302_SCLK=0;
	DS1302_IO=0;
}

void DS1302_WriteByte(unsigned char cmd,Data)
{
	unsigned char i;
	DS1302_CE=1;
	for(i=0;i<8;i++)
	{
		DS1302_IO=cmd&(0x01<<i);
		DS1302_SCLK=1;
		DS1302_SCLK=0;
	}
	for(i=0;i<8;i++)
	{
		DS1302_IO=Data&(0x01<<i);
		DS1302_SCLK=1;
		DS1302_SCLK=0;
	}
	DS1302_CE=0;
}

unsigned char DS1302_ReadByte(unsigned char cmd)
{
	unsigned char Data=0x00;
	unsigned char i;
	cmd|=0x01;
	DS1302_CE=1;
	for(i=0;i<8;i++)
	{
		DS1302_IO=cmd&(0x01<<i);
		DS1302_SCLK=0;
		DS1302_SCLK=1;
	}
	for(i=0;i<8;i++)
	{
		DS1302_SCLK=1;
		DS1302_SCLK=0;
		if(DS1302_IO) Data|=(0x01<<i);
	}
	DS1302_CE=0;
	DS1302_IO=0;	//读取后将IO设置为0，否则读出的数据会出错
	return Data;
}