#include <REGX52.H>
#include "XPT2046.h"
#include "LCD1602.h"

unsigned int ad_val;
void main()
{
	LCD_Init();
	LCD_ShowString(1,1,"AD");
	while(1)
	{
		ad_val=XPT2046_ReadAD(XPT2046_XP);
		LCD_ShowNum(2,1,ad_val,3);
		ad_val=XPT2046_ReadAD(XPT2046_YP);
		LCD_ShowNum(2,6,ad_val,3);
		ad_val=XPT2046_ReadAD(XPT2046_VBAT);
		LCD_ShowNum(2,11,ad_val,3);
	}
}
