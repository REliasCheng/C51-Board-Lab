#ifndef __I2C_H
#define __I2C_H

void I2C_Start(void);
void I2C_Stop(void);
void I2C_SendByte(unsigned char Byte);
unsigned char I2C_ReceiveByte(void);
void I2C_SendAck(unsigned char ack);
unsigned I2C_ReceiveAck(void);

unsigned char AT24C02_ReadByte(unsigned char Addr);
void AT24C02_WriteByte(unsigned char Addr,Data);

#endif
