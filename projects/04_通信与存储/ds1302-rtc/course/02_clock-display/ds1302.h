#ifndef __DS1302_H
#define __DS1302_H
extern unsigned char DS1302_Time[];

void DS1302_Init(void);
void DS1302_WriteByte(unsigned char cmd,Data);
unsigned char DS1302_ReadByte(unsigned char cmd);
void SetTime(void);
void GetTime(void);
#endif
