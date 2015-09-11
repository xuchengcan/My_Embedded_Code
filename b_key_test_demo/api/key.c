#include "stdio.h"		//printf scanf ....
#include "s5pv210.h"	//寄存器

void KEY_Init(void)
{
	/* 1.初始化引脚为输入模式 */
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
	/* 按下抖动去抖 */
	Delay();
	if((*data & (1<<num))==0)
	{
		printf("%s Down,\r\n",name);
		/* 等待释放 */
		while((*data & (1<<num))==0);
		printf("%s Up\r\n",name);
		/* 松开抖动去抖 */
		Delay();
	}
}

void KEY_Test(void)
{
	printf("This is KEY Test\r\n");
	
	/* 初始化按键引脚 */
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
