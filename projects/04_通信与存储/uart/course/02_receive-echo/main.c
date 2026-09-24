#include <REGX52.H>

void Uart1_Init(void)	//9600bps@11.0592MHz
{
	PCON &= 0x7F;		//波特率不倍速
	SCON = 0x50;		//8位数据,可变波特率
//	AUXR &= 0xBF;		//定时器时钟12T模式
//	AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
	TMOD &= 0x0F;		//设置定时器模式
	TMOD |= 0x20;		//设置定时器模式
	TL1 = 0xFD;			//设置定时初始值
	TH1 = 0xFD;			//设置定时重载值
	ET1 = 0;			//禁止定时器中断
	TR1 = 1;			//定时器1开始计时
	ES = 1;				//使能串口1中断
	EA = 1;
	
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


void Uart1_Isr(void) interrupt 4
{
	if (RI)				//检测串口1接收中断
	{
		RI = 0;			//清除串口1接收中断请求位
		P2=~SBUF;
		UART_SendByte(SBUF);
	}
}
