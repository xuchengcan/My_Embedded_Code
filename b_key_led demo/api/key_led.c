#include "stdio.h"		//printf scanf ....
#include "s5pv210.h"	//寄存器

void KEY_Init(void)
{
	/* 1.初始化引脚为输入模式 */
	rGPH2CON &= ~(0xFFFF<<0);
	rGPH3CON &= ~(0xFFFF<<0);
}

void LED_Init(void)
{
	/* 1.初始化GPJ2_0引脚为输出模式 */
	rGPJ2CON &= ~(0xFFFF<<0);
	rGPJ2CON |=  (0x1111<<0);	
}

void Delay(void)
{
	volatile int i=0x50000;
	while(i--);	
}

void show(volatile unsigned long * data,volatile unsigned long * led_data,char * name,int num)
{
	/* 按下抖动去抖 */
	Delay();
	if((*data & (1<<num))==0)
	{
		int i = 0;
		if (name!="key 1")
		{
			/* 点亮GPJ2_0引脚控制的LED灯 */
			*led_data &=~(1<<num);
			printf("%s Down led%d on\r\n",name,num);
			/* 等待释放 */
			while((*data & (1<<num))==0);
		}else
		{
			while((*data & (1<<num))==0)
			{
				/* 点亮GPJ2_0引脚控制的LED灯 */
				*led_data &=~(1<<i);
				Delay();
				printf(".");
				if(i%10==0)
					printf("\r\n");
				/* 熄灭GPJ2_0引脚控制的LED灯 */
				*led_data |= (1<<i);
				Delay();		
				i++;
				if (i==4)
				{
					i=0;
				}
			}
		}

		if (name!="key 1")
		{
			printf("%s Up led%d off\r\n",name,num);
		}else
		{
			printf("%s Up led off\r\n",name);
		}
		/* 熄灭GPJ2_0引脚控制的LED灯 */
		*led_data |= (1<<num);
		/* 松开抖动去抖 */
		Delay();
	}
}

void KEY_Test(void)
{
	printf("This is KEY Test\r\n");
	
	/* 初始化按键引脚 */
	KEY_Init();
	
	/* 初始化Led灯引脚 */
	LED_Init();

	while(1)
	{
		switch(rGPH2DAT){
			case 14:
				show(&rGPH2DAT,&rGPJ2DAT,"back",0);
				break;
			case 13:
				show(&rGPH2DAT,&rGPJ2DAT,"home",1);
				break;
			case 11:
				show(&rGPH2DAT,&rGPJ2DAT,"menu",2);
				break;
			case 7:
				show(&rGPH2DAT,&rGPJ2DAT,"key 1",3);
				break;
			default :
				Delay();
			}

		switch(rGPH3DAT){
			case 14:
				show(&rGPH3DAT,&rGPJ2DAT,"key 2",0);
				break;
			case 13:
				show(&rGPH3DAT,&rGPJ2DAT,"key 3",1);
				break;
			case 11:
				show(&rGPH3DAT,&rGPJ2DAT,"key 4",2);
				break;
			case 7:
				show(&rGPH3DAT,&rGPJ2DAT,"key 5",3);
				break;
			default :
				Delay();
			}
	}
}
