#include "bsp_config.h"
#include ".\segs\segs.h"
#include ".\timer\timer.h"
#include ".\leds\leds.h"
#include ".\keys\keys.h"

void bsp_init(void)
{
		//bsp Init
		timer_init();
		segs_init();
		leds_init();
		keys_init();
		
		//bsp APPs Init
		bsp_apps_init();
}

void bsp_apps_init(void)
{

}

/*******************************************************************************
* 函 数 名       : delay_10us
* 函数功能		 : 延时函数，ten_us=1时，大约延时10us
* 输    入       : ten_us
* 输    出    	 : 无
*******************************************************************************/
void delay_10us(u16 ten_us)
{
	while(ten_us--);	
}

/*******************************************************************************
* 函 数 名       : delay_ms
* 函数功能		 : ms延时函数，ms=1时，大约延时1ms
* 输    入       : ms：ms延时时间
* 输    出    	 : 无
*******************************************************************************/
void delay_ms(u16 ms)
{
	u16 i,j;
	for(i=ms;i>0;i--)
		for(j=110;j>0;j--);
}

//void bap_apps_up(void)
//{

//}
