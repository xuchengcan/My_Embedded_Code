
#include "graphic.h"


int main(int argc,char **argv)
{
	int ret;
	pthread_t view_id;
	pid_t view_pid=0,audio_pid=0;

       // LCD 初始化
	fb_init();
      // 触摸屏初始化
	ts_init();
       // 显示一张启动logo
	show_jpglogo("./image/welcome.jpg");
	sleep(3);
	// 显示控制主界面菜单
	show_menu();


	ret =  pthread_create(&view_id, NULL,picview_thread, NULL);
	if(ret == -1)
		goto pthread_create_err;



	while(1)
	{
		
		// 等待获取坐标
		pthread_mutex_lock(&mutex);
		printf(" main get lock!\n");
	
		m = get_motion();
		
		printf(" main put lock!\n");
		pthread_mutex_unlock(&mutex);
		usleep(1000);


		switch(m)
		{

			case button1:
				printf("button1 Prev\n");
				if(audio_pid != 0)
					kill(audio_pid,SIGKILL);
				audio_pid = start_audio(BACKWARD);
				//kill(view_pid,SIGUSR1);
				break;
				
			case button2:
				printf("button2 Next\n");
				if(audio_pid != 0)
					kill(audio_pid,SIGKILL);
				audio_pid = start_audio(FORWARD);
				//kill(view_pid,SIGUSR2);				
				break;
				
			case button3:
				printf("button3 Stop\n");
				kill(audio_pid,SIGSTOP);
				//kill(view_pid,SIGSTOP);				
				break;
				
			case button4:
				printf("button4 Continue\n");
				kill(audio_pid,SIGCONT);
				//kill(view_pid,SIGCONT);					
				break;

			case button5:
				printf("button5 Load\n");
				traverse_file_dir("/photo/dir");
				//view_pid = start_picview();
				signal(SIGCHLD,waitp);
				break;
				
		}


	}

	fb_uninit();
	ts_uninit();
	
	return 0;
	
pthread_create_err:
	printf("  pthread_create  err!\n");
	return -1;

}

