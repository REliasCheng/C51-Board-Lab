#include <REGX52.H>
#include "ds1302.h"
#include "LCD1602.h"
unsigned char seconds;
void main()
{
	LCD_Init();
	DS1302_Init();
	SetTime();
	while(1)
	{
		GetTime();
		LCD_ShowNum(1,1,DS1302_Time[0],2);
		LCD_ShowChar(1,3,'-');
		LCD_ShowNum(1,4,DS1302_Time[1],2);
		LCD_ShowChar(1,6,'-');
		LCD_ShowNum(1,7,DS1302_Time[2],2);
		
		LCD_ShowNum(2,1,DS1302_Time[3],2);
		LCD_ShowChar(2,3,':');
		LCD_ShowNum(2,4,DS1302_Time[4],2);
		LCD_ShowChar(2,6,':');
		LCD_ShowNum(2,7,DS1302_Time[5],2);
	}
}
