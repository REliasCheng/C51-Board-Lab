#include <REGX52.H>
#include "i2c.h"
#include "delay.h"

sbit I2C_SCL=P2^1;
sbit I2C_SDA=P2^0;

void I2C_Start(void)
{
	I2C_SDA=1;
	I2C_SCL=1;
	I2C_SDA=0;
	I2C_SCL=0;
}

void I2C_Stop(void)
{
	I2C_SDA=0;
	I2C_SCL=1;
	I2C_SDA=1;
}

void I2C_SendByte(unsigned char Byte)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		I2C_SDA=(Byte&0x80>>i);
		I2C_SCL=1;
		I2C_SCL=0;
	}
}

unsigned char I2C_ReceiveByte(void)
{
	unsigned char i,Byte=0x00;
	I2C_SDA=1;
	for(i=0;i<8;i++)
	{
		I2C_SCL=1;
		if(I2C_SDA) Byte|=(0x80>>i);
		I2C_SCL=0;
	}
	return Byte;
}
void I2C_SendAck(unsigned char ack)
{
	I2C_SDA=ack;
	I2C_SCL=1;
	I2C_SCL=0;
}

unsigned I2C_ReceiveAck(void)
{
	unsigned char ack;
	I2C_SDA=1;
	I2C_SCL=1;
	ack=I2C_SDA;
	I2C_SCL=0;
	return ack;
}

void AT24C02_WriteByte(unsigned char Addr,Data)
{
	I2C_Start();
	I2C_SendByte(0xa0);
	I2C_ReceiveAck();
	
	I2C_SendByte(Addr);
	I2C_ReceiveAck();
	
	I2C_SendByte(Data);
	I2C_ReceiveAck();
	I2C_Stop();
	Delay1ms(5);
}

unsigned char AT24C02_ReadByte(unsigned char Addr)
{
	unsigned char Data;
	
	I2C_Start();
	I2C_SendByte(0xa0);
	I2C_ReceiveAck();
	
	I2C_SendByte(Addr);
	I2C_ReceiveAck();
	
	I2C_Start();
	I2C_SendByte(0xa1);
	I2C_ReceiveAck();
	
	
	Data=I2C_ReceiveByte();
	I2C_SendAck(1);

	I2C_Stop();
	return Data;
}
