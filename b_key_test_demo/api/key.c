#include "stdio.h"		//printf scanf ....
#include "s5pv210.h"	//�Ĵ���

void KEY_Init(void)
{
	/* 1.��ʼ������Ϊ����ģʽ */
	rGPH2CON &= ~(0xFFFF<<0);
	rGPH3CON &= ~(0xFFFF<<0);
}

void Delay(void)
{
	volatile int i=0x50000;
	while(i--);	
}

void show(volatile unsigned long * data,char * name,int num)
{
	/* ���¶���ȥ�� */
	Delay();
	if((*data & (1<<num))==0)
	{
		printf("%s Down,\r\n",name);
		/* �ȴ��ͷ� */
		while((*data & (1<<num))==0);
		printf("%s Up\r\n",name);
		/* �ɿ�����ȥ�� */
		Delay();
	}
}

void KEY_Test(void)
{
	printf("This is KEY Test\r\n");
	
	/* ��ʼ���������� */
	KEY_Init();
	
	while(1)
	{
		switch(rGPH2DAT){
			case 14:
				show(&rGPH2DAT,"back",0);
				break;
			case 13:
				show(&rGPH2DAT,"home",1);
				break;
			case 11:
				show(&rGPH2DAT,"menu",2);
				break;
			case 7:
				show(&rGPH2DAT,"key 1",3);
				break;
			default :
				Delay();
			}

		switch(rGPH3DAT){
			case 14:
				show(&rGPH3DAT,"key 2",0);
				break;
			case 13:
				show(&rGPH3DAT,"key 3",1);
				break;
			case 11:
				show(&rGPH3DAT,"key 4",2);
				break;
			case 7:
				show(&rGPH3DAT,"key 5",3);
				break;
			default :
				Delay();
			}
	}
}
