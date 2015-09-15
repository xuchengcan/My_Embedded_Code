
CC = arm-linux-gcc
CFLAGS =  -L /usr/arm-jpeg/lib -L /usr/tslib/lib -I /usr/arm-jpeg/include -I /usr/tslib/include  -lts -ljpeg -lpthread
OBJS1 = graphic.c  graphic_main.c
OBJS2 = graphic.c  graphic_main2.c thread_pool.c


main1:
	$(CC) $(OBJS1) $(CFLAGS) -o main1
	
main2:
	$(CC) $(OBJS2) $(CFLAGS) -o main2
	
clean1:
	rm main1 
clean2:
	rm main2



