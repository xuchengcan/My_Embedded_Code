
#include "graphic.h"

int Show=0;

int main(int argc,char **argv)
{
	int ret,Load=0;
	int pool_num=0;
	pthread_t view_id;
	pid_t view_pid=0,audio_pid=0;

       	// LCD ³õÊ¼»¯
	fb_init();
      	// ´¥ÃþÆÁ³õÊ¼»¯
	ts_init();
       	// ÏÔÊ¾Ò»ÕÅÆô¶¯logo
    pool_init(10);    
    	// Ïß³Ì³Ø³õÊ¼»¯

	show_full_jpg("./image/welcome.jpg");
	sleep(1);
	// ÏÔÊ¾¿ØÖÆÖ÷½çÃæ²Ëµ¥
	show_menu();

	signal(SIGUSR1,view_prev);
	signal(SIGUSR2,view_next);
	signal(SIGCHLD,waitp);
	signal(SIGALRM,alarmfuns);

	printf("main   thread  ID :%d \n",pthread_self());
	
	ret =  pthread_create(&view_id, NULL,picview_thread, NULL);
	if(ret == -1)
		goto pthread_create_err;

	traverse_file_dir("/photo_project/music_pic");
	view_pid = start_picview();

	while(1)
	{
		
		// µÈ´ý»ñÈ¡×ø±ê
	
		m = get_motion();
		if(Show == 1 && m != button6)
			continue;
	
		if (m == button_exit)
		{
			printf("exit \n");
			if(audio_pid != 0)
					kill(audio_pid,SIGKILL);
			if(view_pid != 0)
					kill(view_pid,SIGKILL);
			break;
		}


		switch(m)
		{

			case button1:
				printf("button1 Prev\n");
				if(audio_pid != 0)
					kill(audio_pid,SIGKILL);
				audio_pid = start_audio(FORWARD);
				pthread_kill(view_id,SIGUSR1);
				//raise(SIGUSR1);
				break;
				
			case button2:
				printf("button2 Next\n");
				if(audio_pid != 0)
					kill(audio_pid,SIGKILL);
				audio_pid = start_audio(BACKWARD);
				pthread_kill(view_id,SIGUSR2);
				//raise(SIGUSR2);
				break;
				
			case button3:
				printf("button3 Stop\n");
				kill(audio_pid,SIGSTOP);
				//pthread_kill(view_pid,SIGSTOP);	
				break;
				
			case button4:
				printf("button4 play\n");
				kill(audio_pid,SIGCONT);
				
				//pthread_kill(view_pid,SIGCONT);					
				break;

			case button5:
				printf("button5 Load\n");
				if(Load == 0)
					{
						traverse_file_dir("/photo_project/music_pic");
						Load = 1;
						//view_pid = start_picview();
					}
				else
					printf("Already Load !\n");
				
				break;

			case button6:
				printf("button6 Center \n");
				//kill(audio_pid,SIGCONT);
				//pthread_kill(view_pid,SIGCONT);
				if(Show == 0)
					{				
						show_full_jpg(current_pinfo.pic_name);
						Show =1;
						alarm(5);
						printf("1--%d\n",Show );
					}
				else
					{
					     Show =0;
					     usleep(3000);
					     show_menu();
					     show_micro_jpg(current_pinfo.pic_name);
					     printf("2--%d\n",Show );
					}

				break;
			case button_cleck:
				printf("clecking . . . . . . \n");
				do_lock();
				break;
		}


	}

	fb_uninit();
	ts_uninit();
	pool_destroy(); 

	return 0;
	
pthread_create_err:
	printf("  pthread_create  err!\n");
	return -1;

}







		

