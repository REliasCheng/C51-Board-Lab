#include <REGX52.H>

void Uart1_Init(void)	//9600bps@11.0592MHz
{
	PCON &= 0x7F;		//波特率不倍速
//	SCON = 0x50;		//8位数据,可变波特率 0101 0000
	SCON = 0x40;		//8位数据,可变波特率 0100 0000
//	AUXR &= 0xBF;		//定时器时钟12T模式
//	AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
	TMOD &= 0x0F;		//设置定时器模式
	TMOD |= 0x20;		//设置定时器模式 0010 0000
	TL1 = 0xFD;			//设置定时初始值
	TH1 = 0xFD;			//设置定时重载值
	ET1 = 0;			//禁止定时器中断
	TR1 = 1;			//定时器1开始计时
}


void UART_SendByte(unsigned char Byte)
{
	SBUF=Byte;
	while(TI==0);
	TI=0;
}

void main()
{
	Uart1_Init();
	UART_SendByte(0xAA);
	while(1)
	{
		
	}
}

