#include <REGX52.H>
#include "MatrixKey.h"
#include "delay.h"


unsigned char MatrixKey_Scan(void)
{
	unsigned char key_val=0;
	/*扫描第一列*/
	P1=0xFF;
	P1_3=0;
	if(P1_7==0){Delay1ms(20);while(P1_7==0);Delay1ms(20);key_val=1;}
	if(P1_6==0){Delay1ms(20);while(P1_6==0);Delay1ms(20);key_val=5;}
	if(P1_5==0){Delay1ms(20);while(P1_5==0);Delay1ms(20);key_val=9;}
	if(P1_4==0){Delay1ms(20);while(P1_4==0);Delay1ms(20);key_val=13;}
	
	/*扫描第二列*/
	P1=0xFF;
	P1_2=0;
	if(P1_7==0){Delay1ms(20);while(P1_7==0);Delay1ms(20);key_val=2;}
	if(P1_6==0){Delay1ms(20);while(P1_6==0);Delay1ms(20);key_val=6;}
	if(P1_5==0){Delay1ms(20);while(P1_5==0);Delay1ms(20);key_val=10;}
	if(P1_4==0){Delay1ms(20);while(P1_4==0);Delay1ms(20);key_val=14;}
	
		/*扫描第三列*/
	P1=0xFF;
	P1_1=0;
	if(P1_7==0){Delay1ms(20);while(P1_7==0);Delay1ms(20);key_val=3;}
	if(P1_6==0){Delay1ms(20);while(P1_6==0);Delay1ms(20);key_val=7;}
	if(P1_5==0){Delay1ms(20);while(P1_5==0);Delay1ms(20);key_val=11;}
	if(P1_4==0){Delay1ms(20);while(P1_4==0);Delay1ms(20);key_val=15;}
	
		/*扫描第四列*/
	P1=0xFF;
	P1_0=0;
	if(P1_7==0){Delay1ms(20);while(P1_7==0);Delay1ms(20);key_val=4;}
	if(P1_6==0){Delay1ms(20);while(P1_6==0);Delay1ms(20);key_val=8;}
	if(P1_5==0){Delay1ms(20);while(P1_5==0);Delay1ms(20);key_val=12;}
	if(P1_4==0){Delay1ms(20);while(P1_4==0);Delay1ms(20);key_val=16;}
	
	return key_val;
}
