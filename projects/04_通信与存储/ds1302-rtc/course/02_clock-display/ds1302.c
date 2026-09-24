#include <REGX52.H>
#include "ds1302.h"

sbit DS1302_SCLK=P3^6;
sbit DS1302_IO=P3^4;
sbit DS1302_CE=P3^5;

//写入命令字
#define DS1302_SECOND  0x80
#define DS1302_MINUTE  0x82
#define DS1302_HOUR    0x84
#define DS1302_DATE    0x86
#define DS1302_MONTH   0x88
#define DS1302_DAY     0x8A
#define DS1302_YEAR    0x8C
#define DS1302_WP  0x8

unsigned char DS1302_Time[]={24,5,25,9,47,55,6};

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
	cmd|=0x01;//读命令转写命令
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

void SetTime(void)
{
	DS1302_WriteByte(DS1302_WP,0x00);
	DS1302_WriteByte(DS1302_YEAR,DS1302_Time[0]/10*16+DS1302_Time[0]%10);
	DS1302_WriteByte(DS1302_MONTH,DS1302_Time[1]/10*16+DS1302_Time[1]%10);
	DS1302_WriteByte(DS1302_DATE,DS1302_Time[2]/10*16+DS1302_Time[2]%10);
	DS1302_WriteByte(DS1302_HOUR,DS1302_Time[3]/10*16+DS1302_Time[3]%10);
	DS1302_WriteByte(DS1302_MINUTE,DS1302_Time[4]/10*16+DS1302_Time[4]%10);
	DS1302_WriteByte(DS1302_SECOND,DS1302_Time[5]/10*16+DS1302_Time[5]%10);
	DS1302_WriteByte(DS1302_DAY,DS1302_Time[6]/10*16+DS1302_Time[6]%10);
	DS1302_WriteByte(DS1302_WP,0x80);
}
void GetTime(void)
{
	unsigned char temp;
	temp=DS1302_ReadByte(DS1302_YEAR);
	DS1302_Time[0]=temp/16*10+temp%16;
	temp=DS1302_ReadByte(DS1302_MONTH);
	DS1302_Time[1]=temp/16*10+temp%16;
	temp=DS1302_ReadByte(DS1302_DATE);
	DS1302_Time[2]=temp/16*10+temp%16;
	temp=DS1302_ReadByte(DS1302_HOUR);
	DS1302_Time[3]=temp/16*10+temp%16;
	temp=DS1302_ReadByte(DS1302_MINUTE);
	DS1302_Time[4]=temp/16*10+temp%16;
	temp=DS1302_ReadByte(DS1302_SECOND);
	DS1302_Time[5]=temp/16*10+temp%16;
	temp=DS1302_ReadByte(DS1302_DAY);
	DS1302_Time[6]=temp/16*10+temp%16;
}
