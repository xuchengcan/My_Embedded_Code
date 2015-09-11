#include "stdio.h"		//printf scanf ....
#include "s5pv210.h"	//寄存器

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

// void LED0(void)
// {
// 	/* 1.初始化GPJ2_0引脚为输出模式 */
// 	rGPJ2CON &= ~(0xF<<0);
// 	rGPJ2CON |=  (0x1<<0);
// 	/* 点亮GPJ2_0引脚控制的LED灯 */
// 	rGPJ2DAT &=~(1<<0);
// 	Delay();
// 	/* 熄灭GPJ2_0引脚控制的LED灯 */
// 	rGPJ2DAT |= (1<<0);
// 	Delay();		
// }

// void LED1(void)
// {
// 	/* 1.初始化GPJ2_1引脚为输出模式 */
// 	rGPJ2CON &= ~(0xF<<4);
// 	rGPJ2CON |=  (0x1<<4);	
// 	/* 点亮GPJ2_1引脚控制的LED灯 */
// 	rGPJ2DAT &=~(1<<1);
// 	Delay();
// 	/* 熄灭GPJ2_1引脚控制的LED灯 */
// 	rGPJ2DAT |= (1<<1);
// 	Delay();	
// }

// void LED2(void)
// {
// 	/* 1.初始化GPJ2_2引脚为输出模式 */
// 	rGPJ2CON &= ~(0xF<<8);
// 	rGPJ2CON |=  (0x1<<8);	
// 	/* 点亮GPJ2_2引脚控制的LED灯 */
// 	rGPJ2DAT &=~(1<<2);
// 	Delay();
// 	/* 熄灭GPJ2_2引脚控制的LED灯 */
// 	rGPJ2DAT |= (1<<2);
// 	Delay();	
// }

// void LED3(void)
// {
// 	/* 1.初始化GPJ2_3引脚为输出模式 */
// 	rGPJ2CON &= ~(0xF<<12);
// 	rGPJ2CON |=  (0x1<<12);	
// 	/* 点亮GPJ2_3引脚控制的LED灯 */
// 	rGPJ2DAT &=~(1<<3);
// 	Delay();
// 	/* 熄灭GPJ2_3引脚控制的LED灯 */
// 	rGPJ2DAT |= (1<<3);
// 	Delay();	
// }

void LED_Test(void)
{
	printf("This is Led Test\r\n");
	
	/* 初始化Led灯引脚 */
	LED_Init();
	
	int i = 0;
	while(1)
	{
		/* 点亮GPJ2_0引脚控制的LED灯 */
		rGPJ2DAT &=~(1<<i);
		
		Delay();
		
		/* 熄灭GPJ2_0引脚控制的LED灯 */
		rGPJ2DAT |= (1<<i);
		
		Delay();		
		
		i++;
		if (i==4)
		{
			i=0;
		}
		// LED0();
		// LED1();
		// LED2();
		// LED3();

	}

}
