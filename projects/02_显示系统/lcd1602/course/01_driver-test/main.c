#include <REGX52.H>
#include "LCD1602.h"
void main()
{
	LCD_Init();
	LCD_ShowChar(1,1,'A');
	//LCD_ShowString(1,1,"hello");
	LCD_ShowNum(1,3,123,4);
	LCD_ShowSignedNum(1,8,-12,3);
	LCD_ShowHexNum(2,1,12,3);
	LCD_ShowBinNum(2,5,6,4);
	while(1)
	{
		
	}
}
