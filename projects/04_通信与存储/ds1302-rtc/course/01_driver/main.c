#include <REGX52.H>
#include "ds1302.h"
#include "LCD1602.h"
unsigned char seconds;
void main()
{
	LCD_Init();
	DS1302_Init();
	LCD_ShowString(1,1,"Second:");
	DS1302_WriteByte(0x8E,0x00);
	DS1302_WriteByte(0x80,0x07);
	while(1)
	{

		seconds=DS1302_ReadByte(0x80);
		LCD_ShowNum(2,1,seconds/16*10+seconds%16,2);
	}
}
