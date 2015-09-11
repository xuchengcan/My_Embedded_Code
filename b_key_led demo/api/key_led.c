#include "stdio.h"		//printf scanf ....
#include "s5pv210.h"	//�Ĵ���

void KEY_Init(void)
{
	/* 1.��ʼ������Ϊ����ģʽ */
	rGPH2CON &= ~(0xFFFF<<0);
	rGPH3CON &= ~(0xFFFF<<0);
}

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

void show(volatile unsigned long * data,volatile unsigned long * led_data,char * name,int num)
{
	/* ���¶���ȥ�� */
	Delay();
	if((*data & (1<<num))==0)
	{
		int i = 0;
		if (name!="key 1")
		{
			/* ����GPJ2_0���ſ��Ƶ�LED�� */
			*led_data &=~(1<<num);
			printf("%s Down led%d on\r\n",name,num);
			/* �ȴ��ͷ� */
			while((*data & (1<<num))==0);
		}else
		{
			while((*data & (1<<num))==0)
			{
				/* ����GPJ2_0���ſ��Ƶ�LED�� */
				*led_data &=~(1<<i);
				Delay();
				printf(".");
				if(i%10==0)
					printf("\r\n");
				/* Ϩ��GPJ2_0���ſ��Ƶ�LED�� */
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
		/* Ϩ��GPJ2_0���ſ��Ƶ�LED�� */
		*led_data |= (1<<num);
		/* �ɿ�����ȥ�� */
		Delay();
	}
}

void KEY_Test(void)
{
	printf("This is KEY Test\r\n");
	
	/* ��ʼ���������� */
	KEY_Init();
	
	/* ��ʼ��Led������ */
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
