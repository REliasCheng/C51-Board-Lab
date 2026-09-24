#include <REGX52.H>
#include <INTRINS.H>
sbit beep=P2^5;

void Delay250us(void)	//@11.0592MHz
{
	unsigned char data i;

	_nop_();
	i = 112;
	while (--i);
}

void main()
{
	unsigned char i=0;
	while(1)
	{
		i=!i;
		beep=i;
		Delay250us();
	}
}