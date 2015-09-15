#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
int  x_num=0;
char x_ch[]={'x','_','x','.','j','p','g'};

int main(int argc, char const *argv[])
{
/*	char a[1];

	
	sprintf(a,"%d",x_num);
	if (x_num==10)
	{
		x_num =0;
	}
	int fd;
	
	
	printf("%s\n", x_ch);

	x_num=x_num+1;

	time_t t;
	time(&t);
	struct tm *tt;
	tt = localtime(&t);
	printf("%d-%d-%d\n",tt->tm_year,tt->tm_mon,tt->tm_mday);
*/

	long a[2]={0x114477,0x558899};

	long x = (a[0]>>16 )& 0xff;

	printf("%lx\n",x );

	return 0;
}