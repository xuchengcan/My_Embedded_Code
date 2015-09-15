

#ifndef __GRAPHIC__
#define __GRAPHIC__

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <linux/input.h>
#include <linux/fb.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <dirent.h>
#include <errno.h>
#include <signal.h>
#include <wait.h>
#include <pthread.h>
#include "tslib.h"
#include "jpeglib.h"
#include "kernel_list.h"
#include "time.h"

#define BACKWARD 0
#define FORWARD  1
#define DIRADDR "/photo_project/music_pic"

#define SCREEN_WIDTH  800
#define SCREEN_HEIGH  480
#define PIX_BITS      4
#define SCREEN_SIZE  (SCREEN_WIDTH)*(SCREEN_HEIGH)*(PIX_BITS)

typedef struct Music_Info{

	char music_name[100];
	struct list_head list;
	
}MInfo;

typedef struct Pic_Info{

	char pic_name[100];
	struct list_head list;
	
}PInfo;

typedef struct Pic_x_Info{

	char pic_x_name[100];
	struct list_head list;
	
}PxInfo;

enum motion{button1, button2, button3, button4,button5,button6,button_exit,button_cleck};
enum motion m;
extern pthread_mutex_t mutex;
extern PInfo current_pinfo;
extern int Show;

int fb_init(void);
int fb_uninit(void);
int ts_init(void);
int ts_uninit(void);
int show_full_jpg(char *pic_name);
int show_button(int x, int y,int width,int heigh, unsigned long color,char *string);
int show_menu(void);
int start_audio(int dir);
int start_picview(void);
enum motion get_motion(void);
int Insert_MNode(struct list_head *head,MInfo Node);
int Insert_PNode(struct list_head *head,PInfo Node);
void Delete_music_list(void);
void Delete_pic_list(void);
int traverse_file_dir(char *dir);
int Find_Prev_music(struct list_head *head,MInfo *current_minfo);
int Find_Next_music(struct list_head *head,MInfo *current_minfo);
int Find_Prev_pic(struct list_head *head,PInfo *current_pinfo);
int Find_Next_pic(struct list_head *head,PInfo *current_minfo);
int show_micro_jpg(char *pic_name);
void view_prev(int signo);
void view_next(int signo);
void waitp(int signo);
void *  picview_thread (void *arg);
void alarmfuns(int signo);
void *  view_thread (void *arg);
void Draw_Text16(unsigned int x,unsigned int y,unsigned int color,const unsigned char ch[]);
void Draw_ASCII(unsigned int x,unsigned int y,unsigned int color,const unsigned char ch[]);
void Display_character(unsigned int x,unsigned int y,unsigned int len,unsigned char *string);
int  pic_check(char *pic_name);



void play(void);
void pool_init(int thread_num);
int pool_add_task(void *(*) (void *), void *); 
int pool_destroy(void);
void *pthread2lock(void *arg) ;
void *pthread2up(void *arg) ;
void *pthread2down(void *arg) ;
void *pthread2left(void *arg) ;
void *pthread2right(void *arg) ;
int Insert_P_x_Node(struct list_head *head,PxInfo Node);
int do_lock(void);
void write2log(char * str);
void *pthread2save(void *arg);
void show2LCD(int lcd_x,int lcd_y,unsigned long *buf);

void write_my_JPEG_file (char * filename, int quality,unsigned long *fb_buffer);
int get_micro_jpg(char *pic_name,unsigned long *pic_buf);

#endif  // __GRAPHIC__



