#include <REGX52.H>
sbit SER=P3^4; 
sbit SCK=P3^6;
sbit RCK=P3^5;

//低位在下
void MxLED_WriteByte(unsigned char Byte)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		SER=(Byte&0x80>>i);
		SCK=1;
		SCK=0;
	}
	RCK=1;
	RCK=0;
}
	

//column: 列号0~7 从左向右
void MxLED_ShowColumn(unsigned char column,unsigned char ucmxled)
{
	 P0= ~(0x80>>column);
	 MxLED_WriteByte(ucmxled);
}

void main()
{
	SCK=0;
	RCK=0;
	while(1)
	{
		MxLED_ShowColumn(0,0x88);
	}
}

