#include <REGX52.H>
#include "delay.h"
#include "LCD1602.h"

unsigned char key_val;

void main()
{
	LCD_Init();	
	while(1)
	{
		key_val = MatrixKey_Scan();
		if(key_val)
		{
			LCD_ShowNum(1,1,key_val,2);
		}
		
	}
}
