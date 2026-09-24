#include <REGX52.H>
#include "i2c.h"
#include "LCD1602.h"
unsigned char count;
void main()
{
	LCD_Init();
	count=AT24C02_ReadByte(0);
	count++;
	AT24C02_WriteByte(0,count);
	LCD_ShowNum(1,1,count,3);
	while(1)
	{
		
	}
}