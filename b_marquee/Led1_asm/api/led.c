#include "stdio.h"		//printf scanf ....
#include "s5pv210.h"	//寄存器

void LED_Init(void)
{
	/* 1.初始化GPJ2_0引脚为输出模式 */
	rGPJ2CON &= ~(0xF<<0);
	rGPJ2CON |=  (0x1<<0);	
}

void Delay(void)
{
	volatile int i=0x50000;
	
	while(i--);	
}

void LED_Test(void)
{
	printf("This is Led Test\r\n");
	
	/* 初始化Led灯引脚 */
	LED_Init();
	
	while(1)
	{
		/* 点亮GPJ2_0引脚控制的LED灯 */
		rGPJ2DAT &=~(1<<0);
		
		Delay();
		
		/* 熄灭GPJ2_0引脚控制的LED灯 */
		rGPJ2DAT |= (1<<0);
		
		Delay();		
		
	}

}
