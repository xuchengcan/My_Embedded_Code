#include "stdio.h"		//printf scanf ....
#include "s5pv210.h"	//�Ĵ���

void LED_Init(void)
{
	/* 1.��ʼ��GPJ2_0����Ϊ���ģʽ */
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
// 	/* 1.��ʼ��GPJ2_0����Ϊ���ģʽ */
// 	rGPJ2CON &= ~(0xF<<0);
// 	rGPJ2CON |=  (0x1<<0);
// 	/* ����GPJ2_0���ſ��Ƶ�LED�� */
// 	rGPJ2DAT &=~(1<<0);
// 	Delay();
// 	/* Ϩ��GPJ2_0���ſ��Ƶ�LED�� */
// 	rGPJ2DAT |= (1<<0);
// 	Delay();		
// }

// void LED1(void)
// {
// 	/* 1.��ʼ��GPJ2_1����Ϊ���ģʽ */
// 	rGPJ2CON &= ~(0xF<<4);
// 	rGPJ2CON |=  (0x1<<4);	
// 	/* ����GPJ2_1���ſ��Ƶ�LED�� */
// 	rGPJ2DAT &=~(1<<1);
// 	Delay();
// 	/* Ϩ��GPJ2_1���ſ��Ƶ�LED�� */
// 	rGPJ2DAT |= (1<<1);
// 	Delay();	
// }

// void LED2(void)
// {
// 	/* 1.��ʼ��GPJ2_2����Ϊ���ģʽ */
// 	rGPJ2CON &= ~(0xF<<8);
// 	rGPJ2CON |=  (0x1<<8);	
// 	/* ����GPJ2_2���ſ��Ƶ�LED�� */
// 	rGPJ2DAT &=~(1<<2);
// 	Delay();
// 	/* Ϩ��GPJ2_2���ſ��Ƶ�LED�� */
// 	rGPJ2DAT |= (1<<2);
// 	Delay();	
// }

// void LED3(void)
// {
// 	/* 1.��ʼ��GPJ2_3����Ϊ���ģʽ */
// 	rGPJ2CON &= ~(0xF<<12);
// 	rGPJ2CON |=  (0x1<<12);	
// 	/* ����GPJ2_3���ſ��Ƶ�LED�� */
// 	rGPJ2DAT &=~(1<<3);
// 	Delay();
// 	/* Ϩ��GPJ2_3���ſ��Ƶ�LED�� */
// 	rGPJ2DAT |= (1<<3);
// 	Delay();	
// }

void LED_Test(void)
{
	printf("This is Led Test\r\n");
	
	/* ��ʼ��Led������ */
	LED_Init();
	
	int i = 0;
	while(1)
	{
		/* ����GPJ2_0���ſ��Ƶ�LED�� */
		rGPJ2DAT &=~(1<<i);
		
		Delay();
		
		/* Ϩ��GPJ2_0���ſ��Ƶ�LED�� */
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
