#include <REGX52.H>
#include "key.h"
#include "delay.h"

unsigned char Key_Scan(void)
{
	unsigned char Key_Val=0;
	if(P3_1==0){Delay1ms(20);while(P3_1==0);Delay1ms(20);Key_Val=1;}
	if(P3_0==0){Delay1ms(20);while(P3_0==0);Delay1ms(20);Key_Val=2;}
	if(P3_2==0){Delay1ms(20);while(P3_2==0);Delay1ms(20);Key_Val=3;}
	if(P3_3==0){Delay1ms(20);while(P3_3==0);Delay1ms(20);Key_Val=4;}
	return Key_Val;
}