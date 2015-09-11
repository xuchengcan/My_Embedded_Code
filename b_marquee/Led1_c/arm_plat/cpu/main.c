#include "stdio.h"

extern void LED_Test(void);

int Start_ArmBoot(void)  //main
{
	printf(">>>> welcome to teacher wen class <<<<\n");

	/* 请添加要执行的函数 */
	
	LED_Test();
	
	
	return 0;
}





