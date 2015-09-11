#include "stdio.h"		//printf scanf ....
#include "s5pv210.h"	//�Ĵ���

void LED_Init(void)
{
	/* 1.��ʼ��GPJ2_0����Ϊ���ģʽ */
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
	
	/* ��ʼ��Led������ */
	LED_Init();
	
	while(1)
	{
		/* ����GPJ2_0���ſ��Ƶ�LED�� */
		rGPJ2DAT &=~(1<<0);
		
		Delay();
		
		/* Ϩ��GPJ2_0���ſ��Ƶ�LED�� */
		rGPJ2DAT |= (1<<0);
		
		Delay();		
		
	}

}
