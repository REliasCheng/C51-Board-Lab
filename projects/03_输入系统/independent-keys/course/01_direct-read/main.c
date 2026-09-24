#include <REGX52.H>

void main()
{
	while(1)
	{
		if(P3_1==0) //按键K1按下
		{
			P2_0=0;
		}
		else //按键没有按下
		{
			P2_0=1;
		}
	}
}
