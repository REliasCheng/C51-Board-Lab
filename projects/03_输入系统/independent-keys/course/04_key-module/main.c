#include <REGX52.H>
#include "key.h"
unsigned char key_val;
void main()
{
	while(1)
	{
		key_val=Key_Scan();
		if(key_val==1) P2_0=~P2_0;
		if(key_val==2) P2_1=~P2_1;
		if(key_val==3) P2_2=~P2_2;
		if(key_val==4) P2_3=~P2_3;
	}
}
