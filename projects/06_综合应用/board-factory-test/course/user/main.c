#include "config.h"
#include "leds.h"
#include "../bsp/xpt2046/xpt2046.h"
#include "../bsp/ds1302/ds1302.h"
#include "../bsp/ds18b20/ds18b20.h"
#include "../bsp/pwm/pwm.h"
#include "../bsp/24c02/24c02.h"
#include "../bsp/lcd1602/lcd1602.h"
#include "../bsp/lcd12864/lcd12864.h"



unsigned char dispy = 0;
unsigned char dispup = 0;
unsigned char beep_start = 0;

union
{
	unsigned char led_byte;
	struct
	{
		unsigned char led_pin_l : 2;
		unsigned char seg_pin : 3;
		unsigned char led_pin_h : 3;
	} led_bit;
} pin_byte;

unsigned char key_read(void)
{
	unsigned char key_data = 0;
	P1 = 0X0F;
	switch (P1)
	{
	case 0X07:
		key_data = 1;
		break;
	case 0X0B:
		key_data = 2;
		break;
	case 0X0D:
		key_data = 3;
		break;
	case 0X0E:
		key_data = 4;
		break;
	default:
		key_data = 0;
		break;
	}
	P1 = 0XF0;
	switch (P1)
	{
	case 0X70:
		key_data += 0;
		break;
	case 0XB0:
		key_data += 4;
		break;
	case 0XD0:
		key_data += 8;
		break;
	case 0XE0:
		key_data += 12;
		break;
	default:
		key_data = 0;
		break;
	}
	if (key_data == 0)
	{
		switch (P3 & 0x0f)
		{
		case 0X07:
			key_data += 20;
			break;
		case 0X0B:
			key_data += 19;
			break;
		case 0X0D:
			key_data += 17;
			break;
		case 0X0E:
			key_data += 18;
			break;
		default:
			key_data = 0;
			break;
		}
	}
	return key_data;
}

typedef enum
{
	steady = 0x01,
	posedge = 0x02,
	negedge = 0x03,
	key_error = 0x04
} key_State;

unsigned char key_ready_up[2] = 0;

union
{
	unsigned char ready;
	struct
	{
		unsigned char updata : 5; // 键值
		unsigned char state : 3;  // 状态
	} ready_bit;
} key_state;

#if 1
unsigned char key_updata(void)
{
	key_ready_up[1] = key_ready_up[0]; // 传递按键参数
	key_ready_up[0] = key_read();	   // 读入一个按键
	if (key_ready_up[1] == 0)
	{
		if (key_ready_up[0] != 0)
		{
			key_state.ready_bit.updata = key_ready_up[0]; // 传出键值
			key_state.ready_bit.state = posedge;		  // 键值参数为上升沿
		}
		else
		{
			key_state.ready_bit.updata = 0x00;	// 传出键值
			key_state.ready_bit.state = steady; // 键值参数为稳态按键
		}
	}
	else
	{
		if (key_ready_up[0] == 0)
		{
			key_state.ready_bit.updata = key_ready_up[1]; // 传出键值
			key_state.ready_bit.state = negedge;		  // 键值参数为下降沿
		}
		else if (key_ready_up[1] == key_ready_up[0])
		{
			key_state.ready_bit.updata = key_ready_up[1]; // 传出键值
			key_state.ready_bit.state = steady;			  // 键值参数为稳态按键
		}
		else
		{
			key_state.ready_bit.state = key_error; // 键值参数为稳态按键
		}
	}
	return key_state.ready;
}
#endif

unsigned char code disp_value[19] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,
				0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71,0x00,0x3e,0x40};
unsigned char disp_buff[8] = {16, 16, 16, 16, 16, 16, 6, 7};
void segs_disp(void)
{
	P0 = 0X00;
	pin_byte.led_bit.seg_pin -= 1;
	P2 = ~pin_byte.led_byte;
	if(disp_buff[pin_byte.led_bit.seg_pin] >= 20)
	{
		P0 = disp_value[disp_buff[pin_byte.led_bit.seg_pin] - 20] | 0x80;
	}
	else
	{
			P0 = disp_value[disp_buff[pin_byte.led_bit.seg_pin]];
	}
	
}

unsigned int seg_disp_up = 0;
unsigned char seg_disp = 0;
unsigned char wind = 0;

void led_demo(void)
{
	seg_disp_up = 0;
	seg_disp = 0;
	LEDo_PIN = 0xff;
	while (1) // LED测试，闪烁3次
	{
		seg_disp_up += 1;
		if (seg_disp_up >= 20000)
		{
			seg_disp_up = 0;
			seg_disp += 1;
			if (seg_disp >= 6)
				break;
			LEDo_PIN = ~LEDo_PIN;
		}
	}
}

void lattice_demo(void)
{
	leds_init();
	seg_disp_up = 0;
	wind = 0;
	while (1)
	{
		seg_disp_up += 1;
		if (seg_disp_up >= 150)
		{
			seg_disp_up = 0;

			if (Leds_t.ledca == 0)
				Leds_t.leds_buff[Leds_t.ledy] = Leds_t.leds_buff[Leds_t.ledy] | (0x01 << Leds_t.ledx);
			else
				Leds_t.leds_buff[Leds_t.ledy] = Leds_t.leds_buff[Leds_t.ledy] & (~(0x01 << Leds_t.ledx));

			Leds_t.ledx += 1;
			if (Leds_t.ledx >= 8)
			{
				Leds_t.ledx = 0;
				Leds_t.ledy += 1;

				if (Leds_t.ledy >= 8)
				{
					Leds_t.ledy = 0;
					Leds_t.ledca = ~Leds_t.ledca;
					wind += 1;
					if (wind >= 6)
					{
						break;
					}
				}
			}
		}

		dispup += 1;
		if (dispup >= 50)
		{
			dispup = 0;
			hc_send_byte(0x00);

			LEDs_PIN = Leds_t.leds_buff[dispy];
			LEDo_PIN = pin_byte.led_byte;

			hc_send_byte(0x80 >> dispy);

			dispy += 1;
			if (dispy >= 8)
			{
				dispy = 0;
			}
		}
	}
}

void xpt_demo(void)
{	
	u16 adc_value=0;
	float adc_vol;//ADC电压值
	seg_disp_up = 0;
	wind = 0;
	while(1)
	{				
		wind += 1;
		if(wind >= 200)
		{
				wind = 0;
				disp_buff[0] = 16;
				disp_buff[1] = 16;
				disp_buff[2] = 16;
				disp_buff[3] = 16;
				disp_buff[4] = 16;
				disp_buff[5] = 16;
				disp_buff[6] = 16;
				disp_buff[7] = 16;	
				segs_disp();//清屏
				seg_disp_up += 1;
				if(seg_disp_up >= 350)
				{
						seg_disp_up = 0;
						break;
				}
			
						adc_value=xpt2046_read_adc_value(0x94);//测量电位器
						adc_vol=5.0*adc_value/4096;//将读取的AD值转换为电压
						adc_value=adc_vol*10;//放大10倍，即保留小数点后一位
						disp_buff[0]=adc_value/10 + 20;
						disp_buff[1]=adc_value%10;
					
					
						adc_value=xpt2046_read_adc_value(0xA4);//测量光敏电阻
						adc_vol=5.0*adc_value/4096;//将读取的AD值转换为电压
						adc_value=adc_vol*10;//放大10倍，即保留小数点后一位
					
						disp_buff[3]=adc_value/10 + 20;
						disp_buff[4]=adc_value%10;
					
						adc_value=xpt2046_read_adc_value(0xD4);//测量热敏电阻
						adc_vol=5.0*adc_value/4096;//将读取的AD值转换为电压
						adc_value=adc_vol*10;//放大10倍，即保留小数点后一位
					
						disp_buff[6]=adc_value/10 + 20;
						disp_buff[7]=adc_value%10;
		}
		segs_disp();
	}		
}

void ds1302_demo(void)
{		
	ds1302_init();//初始化DS1302
	seg_disp_up = 0;
	wind = 0;
	while(1)
	{	
		wind += 1;
		if(wind >= 200)
		{
				wind = 0;
				disp_buff[0] = 16;
				disp_buff[1] = 16;
				disp_buff[2] = 16;
				disp_buff[3] = 16;
				disp_buff[4] = 16;
				disp_buff[5] = 16;
				disp_buff[6] = 16;
				disp_buff[7] = 16;	
				segs_disp();
			
				seg_disp_up += 1;
				if(seg_disp_up >= 350)
				{
						seg_disp_up = 0;
						break;
				}
				
				ds1302_read_time();
				disp_buff[0]=gDS1302_TIME[2]/16;
				disp_buff[1]=gDS1302_TIME[2]&0x0f;
				disp_buff[2]=18;
				disp_buff[3]=gDS1302_TIME[1]/16;
				disp_buff[4]=gDS1302_TIME[1]&0x0f;
				disp_buff[5]=18;
				disp_buff[6]=gDS1302_TIME[0]/16;
				disp_buff[7]=gDS1302_TIME[0]&0x0f;				
		}
		segs_disp();
	}		
}

void dac_demo(void)
{	
	u8 dir=0;//默认为0
	u8 duty=0;
	u16 adc_value=0;
	float adc_vol;//ADC电压值
	seg_disp_up = 0;
	wind = 0;
	
	pwm_init(0XFF,0XF6,100,0);//定时时间为0.01ms，PWM周期是100*0.01ms=1ms，占空比为0%

	while(1)
	{
		wind += 1;
		if(wind >= 10)
		{
				wind = 0;
				if(dir==0)//当dir为递增方向
				{
					duty++;//占空比递增
					if(duty==70)dir=1;//当到达一定值切换方向，占空比最大能到100，但到达70左右再递增，
										//肉眼也分辨不出亮度变化	
				}
				else
				{
					duty--;
					if(duty==0)dir=0;//当到达一定值切换方向	
				}
				pwm_set_duty_cycle(duty);//设置占空比		

				seg_disp_up += 1;
				if(seg_disp_up >= 350)
				{
						seg_disp_up = 0;
						disp_buff[0] = 16;
						disp_buff[1] = 16;
						disp_buff[2] = 16;
						disp_buff[3] = 16;
						disp_buff[4] = 16;
						disp_buff[5] = 16;
						disp_buff[6] = 16;
						disp_buff[7] = 16;	
						segs_disp();
						TR0=0;//打开定时器
						break;
				}
				
				if((seg_disp_up % 20) == 0)
				{
						disp_buff[0] = 16;
						disp_buff[1] = 16;
						disp_buff[2] = 16;
						disp_buff[3] = 16;
						disp_buff[4] = 16;
						disp_buff[5] = 16;
						disp_buff[6] = 16;
						disp_buff[7] = 16;	
						segs_disp();
					

						
						adc_value=xpt2046_read_adc_value(0xe4);//测量电位器
						adc_vol=5.0*adc_value/4096;//将读取的AD值转换为电压
						adc_value=adc_vol*10;//放大10倍，即保留小数点后一位
						disp_buff[0]=adc_value/10 + 20;
						disp_buff[1]=adc_value%10;				
				}
		}
		segs_disp();
	}
}

void lcd1602_demo(void)
{
		lcd1602_init();//LCD1602初始化
		lcd1602_clear();
		lcd1602_show_string(0,0,"hello wolrd");//第一行显示
		lcd1602_show_string(0,1,"0123456789");//第二行显示
		seg_disp_up = 0;
		while(1)
		{
				seg_disp_up += 1;
				if(seg_disp_up >= 35000)
				{
						seg_disp_up = 0;
						break;
				}
		}
}

void lcd12864_demo(void)
{
		lcd12864_init();//LCD12864初始化
		lcd12864_clear();
		lcd12864_show_string(0,0,"Hello World!");//第1行字符串显示
		lcd12864_show_string(0,1,"感谢大家使用！");//第2行字符串显示
		lcd12864_show_string(0,2,"鹿小班开天开发板");//第3行字符串显示
		lcd12864_show_string(0,3,"好好学习天天向上");//第4行字符串显示
		seg_disp_up = 0;
		while(1)
		{
				seg_disp_up += 1;
				if(seg_disp_up >= 35000)
				{
						seg_disp_up = 0;
						break;
				}
		}
}

void ds18b20_demo(void)
{		
	int temp_value;
	ds18b20_init();//初始化DS18B20
	seg_disp_up = 0;
	wind = 0;
	while(1)
	{	
		wind += 1;
		if(wind >= 100)
		{
				wind = 0;
				disp_buff[0] = 16;
				disp_buff[1] = 16;
				disp_buff[2] = 16;
				disp_buff[3] = 16;
				disp_buff[4] = 16;
				disp_buff[5] = 16;
				disp_buff[6] = 16;
				disp_buff[7] = 16;	
				segs_disp();
			
				seg_disp_up += 1;
				if(seg_disp_up >= 200)
				{
						seg_disp_up = 0;
						break;
				}
				
				temp_value=ds18b20_read_temperture()*10;//保留温度值小数后一位
				
				if(temp_value<0)//负温度
				{
					temp_value=-temp_value;
					disp_buff[0]=18;//显示负号	
				}
				else
					disp_buff[0]=16;//不显示
		
				disp_buff[1]=temp_value/1000;
				disp_buff[2]=temp_value%1000/100;
				disp_buff[3]=(temp_value%1000%100/10) + 20;
				disp_buff[4]=temp_value%1000%100%10;	
		}
		segs_disp();
	}		
}

sbit beep_pin = P2^5;

void beep_demo(void)
{
		beep_start =! beep_start;
}

void BeepUpdata(void)
{
		if(beep_start == 1)
		{
				beep_pin =! beep_pin;
		}
		else
		{
				beep_pin = beep_pin;
		}
}



void main(void)
{

	bsp_init();
	LEDo_PIN = 0xff;
	at24c02_write_one_byte(0,8);
	disp_buff[0]=at24c02_read_one_byte(0);
	while (1)
	{
		seg_disp_up += 1;
		if (seg_disp_up >= 10)
		{
			seg_disp_up = 0;
			key_updata();
			if (key_state.ready_bit.updata != 0)
			{
				if (key_state.ready_bit.state == negedge)
				{
					disp_buff[6] = 0;
					disp_buff[7] = 0;
				}
				if (key_state.ready_bit.state == posedge)
				{
					disp_buff[6] = key_state.ready_bit.updata / 10;
					disp_buff[7] = key_state.ready_bit.updata % 10;
					switch (key_state.ready_bit.updata)
					{
					case 1:
						led_demo();
						break;
					case 2:
						lattice_demo();
						break;
					case 3:
						xpt_demo();
						break;
					case 4:
						ds1302_demo();
						break;
					case 5:
						dac_demo();
						break;
					case 6:
						ds18b20_demo();
						break;
					case 7:
						lcd1602_demo();
						break;
					case 8:
						lcd12864_demo();
						break;
					case 9:
						beep_demo();
						break;
					default:
						break;
					}
				}
			}
		}
		BeepUpdata();
		segs_disp();
	}
}
