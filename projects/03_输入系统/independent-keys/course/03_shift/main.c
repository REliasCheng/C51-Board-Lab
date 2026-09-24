#include <REGX52.H>
#include <INTRINS.H>

void Delay1ms(unsigned int xms);

unsigned char ucled=0x01;//0000 0001
void main()
{
	while(1)
	{
		if(P3_3==0) //°´¼üK2°´ÏÂ
		{
			Delay1ms(20);
			while(P3_3==0);
			Delay1ms(20);
			
			ucled<<=1;
			if(ucled==0x00) ucled=0x01;
		}
		P2=~ucled;
	}
}


void Delay1ms(unsigned int xms)	//@11.0592MHz
{
	unsigned char data i, j;
	while(xms--)
	{
			_nop_();
		i = 2;
		j = 199;
		do
		{
			while (--j);
		} while (--i);
	}
}

