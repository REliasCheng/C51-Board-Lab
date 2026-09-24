#ifndef __XPT2046_H
#define __XPT2046_H

unsigned char XPT2046_XP=0x9C;//1001 1100  AD1
unsigned char XPT2046_YP=0xDC;//1101 1100  NTC1
unsigned char XPT2046_VBAT=0xAC ;//1010 1100 GR1
unsigned char XPT2046_AUX=0xEC ;//1110 1100


unsigned int XPT2046_ReadAD(unsigned char cmd);

#endif

