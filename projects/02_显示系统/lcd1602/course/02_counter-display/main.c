#include <REGX52.H>
#include "LCD1602.h"
#include "delay.h"


unsigned char i;//8λ 0~255
void main()
{
	LCD_Init();
	LCD_ShowString(1,1,"hello");
	

	while(1)
	{
		LCD_ShowNum(1,7,i,3);
		Delay1ms(1000);
		i++;
	}
}
