#include <REGX52.H>
#include "delay.h"
#include "MxLED.h"

unsigned char MxLED_buf[]={
	0x3C,0x42,0xA9,0x85,0x85,0xA9,0x42,0x3C,
	0x3C,0x42,0xA5,0x89,0x89,0xA5,0x42,0x3C

};

void main()
{
	unsigned char i,count,offset;
	MxLED_Init();
	
	while(1)
	{
		for(i=0;i<8;i++)
		{
			MxLED_ShowColumn(i,MxLED_buf[i+offset]);
		}
		count++;
		
		if(count>16)
		{
			count=0;
			offset+=8;
			if(offset>=16)
			{
				offset=0;
			}
		}
		
	}
}

