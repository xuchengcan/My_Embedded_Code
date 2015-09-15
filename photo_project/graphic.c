
#include "graphic.h"
#include "font_libs.h"

#define br_colour 0xC4C2C6
#define SAVEFILE 0

int fbfd;
char x_ch[]={'x','_','x','.','j','p','g'};
int x_num=0;
int pool_num=10;
int play_fog=0;
unsigned long *fb_mem;
struct tsdev *ts;
LIST_HEAD(music_list);//#define LIST_HEAD(name) struct list_head name = LIST_HEAD_INIT(name)
LIST_HEAD(pic_list);
LIST_HEAD(pic_x_list);
MInfo current_minfo;
PInfo current_pinfo;
PxInfo current_x_pinfo;
PxInfo Pxnode;			//当前节点
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


void *pthread2save(void *arg)
{
	printf("....save....%s\n",(char*)arg );
	char dir[100];
	char saveadd[100];
	sprintf(dir,"%s/%s",DIRADDR, (char*)arg);
	sprintf(saveadd,"%s/%s","/home/save/", (char*)arg);

	int rd_fd,wr_fd;
	char read_buf[128]={0};
	int rd_ret = 0;

	rd_fd = open(dir,O_RDONLY);

	if (rd_fd < 0)
	{
		printf("open src file %s failure\n",dir);
		exit(2);
	}
	printf("open src file %s sucess,rd_fd=%d\n",saveadd,rd_fd );

	wr_fd = open(saveadd,O_WRONLY|O_CREAT);

	if (wr_fd < 0)
	{
		printf("open des file %s failure\n",saveadd);
		exit(3);
	}
	printf("open des file %s sucess,wr_fd=%d\n",saveadd,wr_fd );

	while(1)
	{
		rd_ret = read(rd_fd,read_buf,128);
		if (rd_ret < 128)
			break;
		write(wr_fd,read_buf,rd_ret);
		memset(read_buf,0,128);
	}
	write(wr_fd,read_buf,rd_ret);
	printf("save %s sucess\n", (char*)arg);
	close(rd_fd);
	close(wr_fd);
}

void show2LCD(int lcd_x,int lcd_y,unsigned long *buf)
{
	int i,x,y;
	for(i=0,y=lcd_x;y<lcd_x+120;y++)
	{
		for(x=lcd_y;x<lcd_y+200;x++)
		{
			 fb_mem[y*SCREEN_WIDTH+x]   =   buf[i];
			 i=i+2;
		}
		i=i+400;
	}

}

int do_lock(void)
{	
	JSAMPLE * image_buffer ;	/* Points to large array of R,G,B-order data */

	char a[1];
	unsigned long micro_buf[SCREEN_WIDTH/2*SCREEN_HEIGH/2];

	PxInfo *node,*p;
	int x=0,y=0,write_fog=0;
	bzero(fb_mem,800*480*4);
	list_for_each_entry(node, &pic_x_list, list)
	{
		
		//write_fog=1;
		get_micro_jpg(node->pic_x_name,micro_buf) ;
		show2LCD(x,y,micro_buf);
		printf("%s\n", node->pic_x_name);
		y=y+200;
		if (y== 800)
		{
			y=0;
			x=x+120;
			if (x==240)
			{
				x=0;
				// FILE *fp = fopen(x_ch, "w+");
				// write_my_JPEG_file (x_ch, 80,JSAMPLE * image_buffer)
				// x_num++;
				// if (x_num==10)
				// {
				// 	x_num =0;
				// }
				// sprintf(a,"%d",x_num);
				// x_ch[0]=a[0];
				// fclose(fp);
				// write_fog=0;
			}
		}
	}
	// if (write_fog)
	// {
		// FILE *fp = fopen(x_ch, "w+");
		// pr
		
		write_my_JPEG_file (x_ch, 80,fb_mem);
		printf("save sucess%s\n" );
		//fclose(fp);
	// }
	
		
	return 0;
} 


void *pthread2lock(void *arg) 
{ 
	printf("I am %u, arg: %s\n", (int)pthread_self(), (char *)arg);
	sleep(10);

	return NULL; 
} 
void *pthread2up(void *arg)
{
	int x,y;
	int i= 120;
	int j= 400;
	int fog = 0;
	while(fog<120){
		for(x=200;x<200+j;x++)
		{
			for(y=100;y<100+i-1;y++)
			{
				fb_mem[y*SCREEN_WIDTH+x]   =  fb_mem[(y+1)*SCREEN_WIDTH+x] ;
			}
			fb_mem[y*SCREEN_WIDTH+x]   =  br_colour ;
		}
		fog++;
	}
	printf("oK up\n");
	return NULL; 

}

void *pthread2down(void *arg)
{
	int x,y;
	int i= 120;
	int j= 400;
	int fog = 0;
	while(fog<120){
		for(x=200;x<200+j;x++)
		{
			for(y=220+i-1;y>220;y--)
			{
				fb_mem[y*SCREEN_WIDTH+x]   =  fb_mem[(y-1)*SCREEN_WIDTH+x] ;
			}
			fb_mem[y*SCREEN_WIDTH+x]   =  br_colour ;
		}
		fog++;
	}
	printf("ok down\n");
	return NULL;  
}

void *pthread2left(void *arg)
{
	int x,y;
	int i= 120;
	int j= 400;
	int fog = 0;
	while(fog<400){
		for(y=100;y<100+i;y++)
		{
			for(x=200;x<200+j-1;x++)
			{
				fb_mem[y*SCREEN_WIDTH+x]   =  fb_mem[y*SCREEN_WIDTH+x+1] ;
			}
			fb_mem[y*SCREEN_WIDTH+x]   =  br_colour ;
		}
		fog++;
	}
	printf("ok left");
	return NULL; 
}

void *pthread2right(void *arg)
{
	int x,y;
	int i= 240;
	int j= 400;
	int fog = 0;
	while(fog<400){
		for(y=220;y<100+i;y++)
		{
			for(x=200+j-1;x>200;x--)
			{
				fb_mem[y*SCREEN_WIDTH+x]   =  fb_mem[y*SCREEN_WIDTH+x-1] ;
			}
			fb_mem[y*SCREEN_WIDTH+x]   =  br_colour ;
		}
		fog++;
	}
	printf("ok right\n");
	return NULL; 	
}

void play(void)
{
	printf("show the picture\n");
	//pool_num=10;
	pthread_t pid1;
	pthread_t pid2;
	// pool_add_task(pthread2up, (void *)(pool_num+1)); 
	// pool_add_task(pthread2down, (void *)(pool_num+2)); 
	if (play_fog==0)
	{
		pthread_create(&pid1, NULL,pthread2up, NULL);
		pthread_create(&pid2, NULL,pthread2down, NULL);
		play_fog=1;
	}else{
		pthread_create(&pid1, NULL,pthread2left, NULL);
		pthread_create(&pid2, NULL,pthread2right, NULL);
		play_fog=0;
	}

	pthread_join(pid1,NULL);
	pthread_join(pid2,NULL);


	printf("pid1%d ---pid2 %d\n",pid1,pid2 );

	//pool_add_task(pthread2left, (void *)(pool_num+3)); 
	//pool_add_task(pthread2right, (void *)(pool_num+4)); 
}


int fb_init(void)
{
	char bug[20];
	char *fbdevice = getenv("TSLIB_FBDEVICE");

	// 获得文件描述符
	fbfd = open(fbdevice, O_RDWR);
	if(fbfd == -1)
		goto open_err;
       // 获得映射的显存地址
	fb_mem = (unsigned long *)mmap( NULL, 
		            				SCREEN_SIZE,
									PROT_READ | PROT_WRITE,
									MAP_SHARED, fbfd, 0);

	if(fb_mem == MAP_FAILED)
		goto mmap_err;

	return 0;
	
open_err:
	perror(" open LCD ");
	return -1;
mmap_err:
	perror(" mmap LCD ");
	return -1;	
}


int  fb_uninit(void)
{
	int ret;
	ret = munmap(fb_mem, SCREEN_SIZE);
	if(ret == -1)
		goto  munmap_err;

	ret = close(fbfd);
	if(ret == -1)
		goto  close_err;
	
	return 0;
	
munmap_err:
	perror(" munmap LCD ");
	return -1;
close_err:
	perror(" close LCD ");
	return -1;
}



int ts_init(void)
{
	int ret;
	char *tsdevice = getenv("TSLIB_TSDEVICE");
	// 获得触摸设备结构
	ts = ts_open(tsdevice, 0);
	if(ts == NULL)
		goto ts_open_err;
	// 调用校准文件进行触摸设备配置
	ret = ts_config(ts);
	if(ret == -1)
		goto  ts_config_err;	

	return 0;
	
ts_open_err:
	printf(" open TS Failed !\n");
	return -1;	
ts_config_err:
	printf(" ts_config Failed !\n");
	return -1;		
}


int ts_uninit(void)
{
	int ret;
	ret = ts_close(ts);
	if(ret == -1)
		goto ts_close_err;
	
	return 0;
	
ts_close_err:
	printf(" ts_close Failed !\n");
	return -1;		
}


/* 
 * 画点
 * 输入参数：
 * x、y : 象素坐标
 * color: 颜色值
 * 对于16BPP: color的格式为0xAARRGGBB (AA = 透明度),
 */
void PutPixel(void * framebuffer_ptr ,unsigned int x, unsigned int y, unsigned long color,int bpp)
{

    switch (bpp){

        case 32:
        {
            unsigned long *addr = (unsigned long *)framebuffer_ptr + (y * SCREEN_WIDTH+ x);
            *addr = (unsigned long) color;
            break;
        }
        
        case 16:
        {
            unsigned short *addr = (unsigned short *)framebuffer_ptr + (y * SCREEN_WIDTH + x);
            *addr = (unsigned short) color;
            break;
        }
        
        case 8:
        {
            unsigned char *addr = (unsigned char *)framebuffer_ptr + (y * SCREEN_WIDTH + x);
            *addr = (unsigned char) color;
            break;
        }

        default:
            break;
    }
}


void Draw_Text16(unsigned int x,unsigned int y,unsigned int color,const unsigned char ch[])
{
	unsigned short int i,j;
	unsigned char mask,buffer;
	for(i=0;i<16;i++)
	{
		mask =0x80;				//掩码
		buffer =ch[i*2];		//提取一行的第一个字节
		for(j=0;j<8;j++)
		{
			if(buffer &mask)
			{
				PutPixel(fb_mem,x+j,y+i,color,32);	//为画笔上色	
			}
			mask =mask >>1;
		}
		mask =0x80;
		buffer =ch[i*2+1];
		for(j=0;j<8;j++)
		{
			if(buffer &mask)
			{
				PutPixel(fb_mem,x+j+8,y+i,color,32);
			}
			mask =mask>>1;	
		}		
	}
}

void Draw_ASCII(unsigned int x,unsigned int y,unsigned int color,const unsigned char ch[])
{
       unsigned short int i,j;
       unsigned char mask,buffer;
      
       for(i=0;i<16;i++)
       {
              mask=0x80;
              buffer=ch[i];
              for(j=0;j<8;j++)
              {                   
                     if(buffer&mask)
                     {
                            PutPixel(fb_mem,x+j,y+i,color,32);
                     }
                     mask=mask>>1;                   
              }
       }
}


void Display_character(unsigned int x,unsigned int y,unsigned int len,unsigned char *string)
{
	int k, xx;
	unsigned char qh,wh;
	const unsigned char *mould;
	unsigned int length =len;
	
	for(k=0,xx=x;k<length;k++)
	{
		if(string[k]&0x80)   //中文字符
		{
			qh =string[k]-0xa0;			//区号
			wh =string[k+1]-0xa0;		//位号
			mould =&__CHS[((qh-1)*94+wh-1)*32];
			Draw_Text16(4+xx,y,0xFF0000,mould);
			xx+=16;
			k++;
		}
		else
		{
			mould =&__ASCII[string[k]*16];
			Draw_ASCII(4+xx,y,0xFF0000,mould);
			xx+=8;
		}
	}
}





int show_full_jpg(char *pic_name)
{
	int x,i=0;
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;

        FILE * infile;	
	JSAMPARRAY  buffer;		/* Output row buffer */
	int row_stride;		/* physical row width in output buffer */

	/* Step 1: allocate and initialize JPEG decompression object */
	if ((infile = fopen(pic_name, "rb")) == NULL) {
		fprintf(stderr, "can't open %s\n", pic_name);
		return 0;
	}


	/* We set up the normal JPEG error routines, then override error_exit. */
	cinfo.err = jpeg_std_error(&jerr);

	/* Now we can initialize the JPEG decompression object. */
	jpeg_create_decompress(&cinfo);

	/* Step 2: specify data source (eg, a file) */

	jpeg_stdio_src(&cinfo, infile);

	/* Step 3: read file parameters with jpeg_read_header() */

	(void) jpeg_read_header(&cinfo, TRUE);

	/* Step 4: set parameters for decompression */


	/* Step 5: Start decompressor */

	(void) jpeg_start_decompress(&cinfo);

	/* JSAMPLEs per row in output buffer */
	row_stride = cinfo.output_width * cinfo.output_components;

	/* Make a one-row-high sample array that will go away when done with image */
	buffer = (*cinfo.mem->alloc_sarray)
		((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

	/* Step 6: while (scan lines remain to be read) */
	/*           jpeg_read_scanlines(...); */

	while (cinfo.output_scanline < cinfo.output_height) {
	/* jpeg_read_scanlines expects an array of pointers to scanlines.
	 * Here the array is only one element long, but you could ask for
	 * more than one scanline at a time if that's more convenient.
	 */
	(void) jpeg_read_scanlines(&cinfo, buffer, 1);
	/* Assume put_scanline_someplace wants a pointer and sample count. */
	// printf("  cinfo.output_scanline = %d \n  size = row_stride\n");
	// 将24bit RGB 刷新到LCD
	//将bmp_buf 24bit的图像数据写入32bit的fb_mem

	
	for(i=0,x=0;x<cinfo.output_width;x++, i+=3)
			{
				*(fb_mem+(cinfo.output_scanline-1)*SCREEN_WIDTH+x) = ( buffer[0][i]<<16|buffer[0][i+1]<<8|buffer[0][i+2]<<0);
			}
	}

	/* Step 7: Finish decompression */

	(void) jpeg_finish_decompress(&cinfo);
	/* We can ignore the return value since suspension is not possible
	* with the stdio data source.
	*/

	/* Step 8: Release JPEG decompression object */

	/* This is an important step since it will release a good deal of memory. */
	jpeg_destroy_decompress(&cinfo);
	fclose(infile);

	return 0;
}


int show_button(int x, int y,int width,int heigh, unsigned long color,char *string)
{
	int i,j;
	
	for(i=y;i<y+heigh;i++)
		for(j=x;j<x+width;j++)
		{
			fb_mem[i*SCREEN_WIDTH+j] = color;
		}

	Display_character(x+20, y+10, strlen(string), string);

	return 0;
}
	
int show_menu(void)
{
	int x,y;

	for(y=0;y<SCREEN_HEIGH;y++)
		for(x=0;x<SCREEN_WIDTH;x++)
		{
			fb_mem[y*SCREEN_WIDTH+x] = br_colour;
		}


	// for(y=100;y<(100+(SCREEN_HEIGH/2));y++)
	// 	for(x=200;x<(200+(SCREEN_WIDTH/2));x++)
	// 	{
	// 		fb_mem[y*SCREEN_WIDTH+x] = 0xFFFFFFFF;
	// 	}
	show_micro_jpg("./image/welcome.jpg");


	show_button(50, 400,100,40, 0x112233,"上一张");
	show_button(200, 400,100,40, 0x112233,"下一张");
	show_button(350, 400,100,40, 0x112233,"Stop");
	show_button(500, 400,100,40, 0x112233,"Play");
	show_button(650, 400,100,40, 0x112233,"加载");
	show_button(650, 10,100,40, 0x112233,"退出");
	show_button(650, 200,100,40, 0x112233,"cleck");

	return 0;
}



enum motion get_motion(void)
{
	struct ts_sample samp;

    while(1)
    {
		ts_read(ts, &samp, 1);

		// 濡傛灉鍧愭爣鍊间笉鍚堟硶锛屽垯涓㈠純
		if(samp.x <= 0 || samp.x >= 800 ||
		   samp.y <= 0 || samp.y >= 480 ||
		   samp.pressure == 0 ) 
			continue;
		else
			break;
    }

		if( samp.x > 50 && samp.x < 150 &&  samp.y > 400 && samp.y < 440)
			return button1;
		if( samp.x > 200 && samp.x < 300 &&  samp.y > 400 && samp.y < 440)
			return button2;
		if( samp.x > 350 && samp.x < 450 &&  samp.y > 400 && samp.y < 440)
			return button3;
		if( samp.x > 500 && samp.x < 600 &&  samp.y > 400 && samp.y < 440)
			return button4;
		if( samp.x > 650 && samp.x < 750 &&  samp.y > 400 && samp.y < 440)
			return button5;
		if( samp.x > 200 && samp.x < 600 &&  samp.y > 100 && samp.y < 340)
			return button6;		
		if( samp.x > 650 && samp.x < 750 &&  samp.y > 10 && samp.y < 50)
			return button_exit;	
		if( samp.x > 650 && samp.x < 750 &&  samp.y > 200 && samp.y < 240)
			return button_cleck;	
		else
			return -1;
	
}



int Insert_MNode(struct list_head *head,MInfo Node)
{
	MInfo *node;

	node = (MInfo *)malloc(sizeof(MInfo));
	if(node == NULL)
		goto malloc_err;
	printf("insert node : %s\n",Node.music_name);
	strncpy(node->music_name,Node.music_name,sizeof(node->music_name));
	list_add(&node->list,head);

	return 0;
	
malloc_err:
	printf(" malloc Err!\n");
	return -1;
	
}


int Insert_PNode(struct list_head *head,PInfo Node)
{
	PInfo *node;

	node = (PInfo *)malloc(sizeof(PInfo));
	if(node == NULL)
		goto malloc_err;
	printf("insert node : %s\n",Node.pic_name);
	strncpy(node->pic_name,Node.pic_name,sizeof(node->pic_name));
	list_add(&node->list,head);

	return 0;
	
malloc_err:
	printf(" malloc Err!\n");
	return -1;
	
}

int Insert_P_x_Node(struct list_head *head,PxInfo Node)
{
	PxInfo *node;
	PxInfo *node2;

	node = (PxInfo *)malloc(sizeof(PxInfo));
	if(node == NULL)
		goto malloc_err;

	list_for_each_entry(node2,&pic_x_list, list)
	{
		if(strcmp(node2->pic_x_name,Node.pic_x_name) == 0)
		{
			return 0;
		}
	}

	printf("insert x_pnode : %s\n",Node.pic_x_name);
	strncpy(node->pic_x_name,Node.pic_x_name,sizeof(node->pic_x_name));
	list_add(&node->list,head);

	return 0;
	
malloc_err:
	printf(" malloc Err!\n");
	return -1;
	
}

void Delete_music_list(void)
{
	MInfo *node,*p;
	
	list_for_each_entry_safe(node,p, &music_list, list)
	{
				list_del(&node->list);
				free(node);
	}

}

void Delete_pic_list(void)
{
	PInfo *node,*p;
	
	list_for_each_entry_safe(node,p, &pic_list, list)
	{
				list_del(&node->list);
				free(node);
	}

}


int traverse_file_dir(char *dir)
{

	struct dirent *ep;
	MInfo Mnode;
	PInfo Pnode;

	DIR *dp = opendir(dir);
	if(dp == NULL)
	{
		perror("opendir() failed");
		return -1;
	}
	
	while((ep = readdir(dp)) != NULL)
	{
	
		if(ep->d_name[0] == '.')
			continue;
		else if(strstr(ep->d_name,".mp3") != NULL)
		{
			sprintf(Mnode.music_name,"%s/%s",dir, ep->d_name);
			if (SAVEFILE)
				pool_add_task(pthread2save, (void *)ep->d_name); 			
			Insert_MNode(&music_list,Mnode);
		}
		else if(strstr(ep->d_name,".jpg") != NULL)
		{
			sprintf(Pnode.pic_name,"%s/%s",dir, ep->d_name);
			if (SAVEFILE)
				pool_add_task(pthread2save, (void *)ep->d_name); 
			Insert_PNode(&pic_list,Pnode);
		}

	}

	return 0;
}


int Find_Prev_music(struct list_head *head,MInfo *current_minfo)
{
	MInfo *node,*p;
	struct list_head *pos;

	if(current_minfo->music_name[0] == '\0' )
	{
			list_for_each_entry(node, head, list)
			{
				strcpy(current_minfo->music_name ,node->music_name);
				return 1;
			}

	}

	list_for_each_prev(pos, head)
	{
		node = list_entry(pos, MInfo, list);
		if(strcmp(node->music_name,current_minfo->music_name) == 0)
				{
					p = list_entry(pos->prev, MInfo, list);
					strcpy(current_minfo->music_name ,p->music_name);
					
					if(pos->prev== &music_list )
					{
									list_for_each_entry(node, head, list)
									{
										strcpy(current_minfo->music_name ,node->music_name);
										return 1;
									}

									

					}
					return 1;
				}
	}


	return 0;
}



int Find_Next_music(struct list_head *head,MInfo *current_minfo)
{
	MInfo *node,*p;

	if(current_minfo->music_name[0] == '\0' )
	{
			list_for_each_entry(node, head, list)
			{
				strcpy(current_minfo->music_name ,node->music_name);
				return 1;
			}

	}
	
	list_for_each_entry(node, head, list)
	{
		if(strcmp(node->music_name,current_minfo->music_name) == 0)
				{
					p = list_entry((node->list).next, MInfo, list);
					strcpy(current_minfo->music_name ,p->music_name);
					
					if((node->list).next == &music_list )
					{
									list_for_each_entry(node, head, list)
									{
										strcpy(current_minfo->music_name ,node->music_name);
										return 1;
									}

					}
					return 1;
				}
	}
	return 0;
}


int Find_Prev_pic(struct list_head *head,PInfo *current_pinfo)
{
	PInfo *node,*p;
	struct list_head *pos;

	if(current_pinfo->pic_name[0] == '\0' )
	{
			list_for_each_entry(node, head, list)
			{
				strcpy(current_pinfo->pic_name ,node->pic_name);
				return 1;
			}

	}

	list_for_each_prev(pos, head)
	{
		node = list_entry(pos, PInfo, list);
		if(strcmp(node->pic_name,current_pinfo->pic_name) == 0)
				{
					p = list_entry(pos->prev, PInfo, list);
					strcpy(current_pinfo->pic_name ,p->pic_name);
					
					if(pos->prev== &pic_list )
					{
									list_for_each_entry(node, head, list)
									{
										strcpy(current_pinfo->pic_name ,node->pic_name);
										return 1;
									}

					}
					return 1;
				}
	}


	return 0;
}



int Find_Next_pic(struct list_head *head,PInfo *current_minfo)
{
	PInfo *node,*p;

	if(current_minfo->pic_name[0] == '\0' )
	{
			list_for_each_entry(node, head, list)
			{
				strcpy(current_minfo->pic_name ,node->pic_name);
				return 1;
			}

	}
	
	list_for_each_entry(node, head, list)
	{
		if(strcmp(node->pic_name,current_minfo->pic_name) == 0)
				{
					p = list_entry((node->list).next, PInfo, list);
					strcpy(current_minfo->pic_name ,p->pic_name);
					
					if((node->list).next == &pic_list )
					{
									list_for_each_entry(node, head, list)
									{
										strcpy(current_minfo->pic_name ,node->pic_name);
										return 1;
									}

					}
					return 1;
				}
	}


	return 0;
}


int show_micro_jpg(char *pic_name)
{

	int x,y,i=0,j=0;
	unsigned long micro_buf[SCREEN_WIDTH/2*SCREEN_HEIGH/2];
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;

        FILE * infile;	
	JSAMPARRAY  buffer;		/* Output row buffer */
	int row_stride;		/* physical row width in output buffer */

	/* Step 1: allocate and initialize JPEG decompression object */
	if ((infile = fopen(pic_name, "rb")) == NULL) {
		fprintf(stderr, "can't open %s\n", pic_name);
		return 0;
	}


	/* We set up the normal JPEG error routines, then override error_exit. */
	cinfo.err = jpeg_std_error(&jerr);

	/* Now we can initialize the JPEG decompression object. */
	jpeg_create_decompress(&cinfo);

	/* Step 2: specify data source (eg, a file) */

	jpeg_stdio_src(&cinfo, infile);

	/* Step 3: read file parameters with jpeg_read_header() */

	(void) jpeg_read_header(&cinfo, TRUE);

	/* Step 4: set parameters for decompression */


	/* Step 5: Start decompressor */

	(void) jpeg_start_decompress(&cinfo);

	/* JSAMPLEs per row in output buffer */
	row_stride = cinfo.output_width * cinfo.output_components;

	/* Make a one-row-high sample array that will go away when done with image */
	buffer = (*cinfo.mem->alloc_sarray)
		((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

	/* Step 6: while (scan lines remain to be read) */
	/*           jpeg_read_scanlines(...); */
    
	while (cinfo.output_scanline < cinfo.output_height) {
	/* jpeg_read_scanlines expects an array of pointers to scanlines.
	 * Here the array is only one element long, but you could ask for
	 * more than one scanline at a time if that's more convenient.
	 */
	(void) jpeg_read_scanlines(&cinfo, buffer, 1);
	/* Assume put_scanline_someplace wants a pointer and sample count. */
	// printf("  cinfo.output_scanline = %d \n  size = row_stride\n");
	// 将24bit RGB 刷新到LCD
	//将bmp_buf 24bit的图像数据写入32bit的fb_mem

    if( ((cinfo.output_scanline-1) % 2) == 0)
    {	
		for(i=0,x=0;x<cinfo.output_width;x+=2, i+=6)
				{
					micro_buf[j++] = ( buffer[0][i]<<16|buffer[0][i+1]<<8|buffer[0][i+2]<<0);
				}
		}
	}

	/* Step 7: Finish decompression */

	(void) jpeg_finish_decompress(&cinfo);
	/* We can ignore the return value since suspension is not possible
	* with the stdio data source.
	*/

	/* Step 8: Release JPEG decompression object */

	/* This is an important step since it will release a good deal of memory. */
	jpeg_destroy_decompress(&cinfo);
	fclose(infile);

	// take micro pic
	for(i=0,y=100;y<100+240;y++)
		for(x=200;x<200+400;x++)
		{
			  fb_mem[y*SCREEN_WIDTH+x]   =   micro_buf[i++];
		}

	return 0;
}


void alarmfuns(int signo)
{
	int ret;
	pthread_t view_id;

	if( Show == 1 )
		{
			ret =  pthread_create(&view_id, NULL,view_thread, NULL);
			if(ret == -1)
				printf(" pthread_create Err!\n ");
		}
}

void write2log(char * str)
{
	time_t t;
	struct tm *timeinfo;
	time(&t);
	timeinfo = localtime(&t);
	FILE *fp = fopen("/var/photo.log", "a+");
	fprintf(fp, "%d:%d:%d\t  this is a bad picture %s \n",timeinfo->tm_hour,timeinfo->tm_min,timeinfo->tm_sec,str);
	fclose(fp);
}

void view_prev(int signo)
{
	printf("view_prev  thread  ID :%d \n",pthread_self());
	Find_Prev_pic(&pic_list,&current_pinfo);
	printf("current   pic : %s \n",current_pinfo.pic_name);

	play();

	show_micro_jpg(current_pinfo.pic_name);
	if (pic_check(current_pinfo.pic_name))
	{
		write2log(current_pinfo.pic_name);
		sprintf(Pxnode.pic_x_name,"%s",current_pinfo.pic_name);
		printf(" the pic check is xxxx \n");
		Insert_P_x_Node(&pic_x_list,Pxnode);
	}
}

void view_next(int signo)
{
	printf(" view_next thread  ID :%d \n",pthread_self());
	Find_Next_pic(&pic_list,&current_pinfo);
	printf("current   pic : %s \n",current_pinfo.pic_name);

	play();

	show_micro_jpg(current_pinfo.pic_name);
	if (pic_check(current_pinfo.pic_name))
	{
		write2log(current_pinfo.pic_name);
		sprintf(Pxnode.pic_x_name,"%s",current_pinfo.pic_name);
		printf(" the pic check is xxxx \n");
		Insert_P_x_Node(&pic_x_list,Pxnode);
	}
	
}


void waitp(int signo)
{
	printf(" my child exit!\n");
	waitpid(-1,NULL,WNOHANG);
}

int start_audio(int dir)
{
	pid_t pid;
	char filename[100];

	if(dir == FORWARD)
		Find_Next_music(&music_list,&current_minfo);
	if(dir == BACKWARD)
		Find_Prev_music(&music_list,&current_minfo);
	
	pid = fork();
	if(pid == 0)
	{
		printf("listen to <%s>\n", current_minfo.music_name);
		execl("/bin/madplay","madplay","--no-tty-control",current_minfo.music_name,NULL);
	}	

	return pid;	
}

int start_picview(void)
{
	pid_t pid;

	pid = fork();
	if(pid == 0)
	{
		signal(SIGUSR1,view_prev);
		signal(SIGUSR2,view_next);

		while(1);
			pause();
		
		exit(0);
	}
	
	return pid;	
}



void *  picview_thread (void *arg)
{
	printf("picview_thread   thread  ID :%d \n",pthread_self());
	sleep(1);

	while(1)
		{

		}
   
	return NULL;
}


void *  view_thread (void *arg)
{

	while(Show)
		{
			Find_Next_pic(&pic_list,&current_pinfo);
			show_full_jpg(current_pinfo.pic_name);	
			if (pic_check(current_pinfo.pic_name))
			{
				write2log(current_pinfo.pic_name);
				sprintf(Pxnode.pic_x_name,"%s",current_pinfo.pic_name);
				printf(" the pic check is xxxx \n");
				Insert_P_x_Node(&pic_x_list,Pxnode);
			}
			sleep(3);
		}

	return NULL;
}


int  pic_check(char *pic_name)
{
	int count = 0;
	int x,y,i=0,j=0;
	unsigned long pic_buf[SCREEN_WIDTH*SCREEN_HEIGH];
	unsigned long micro_buf[SCREEN_WIDTH/2*SCREEN_HEIGH/2];
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;

        FILE * infile;	
	JSAMPARRAY  buffer;		/* Output row buffer */
	int row_stride;		/* physical row width in output buffer */

	/* Step 1: allocate and initialize JPEG decompression object */
	if ((infile = fopen(pic_name, "rb")) == NULL) {
		fprintf(stderr, "can't open %s\n", pic_name);
		return 0;
	}


	/* We set up the normal JPEG error routines, then override error_exit. */
	cinfo.err = jpeg_std_error(&jerr);

	/* Now we can initialize the JPEG decompression object. */
	jpeg_create_decompress(&cinfo);

	/* Step 2: specify data source (eg, a file) */

	jpeg_stdio_src(&cinfo, infile);

	/* Step 3: read file parameters with jpeg_read_header() */

	(void) jpeg_read_header(&cinfo, TRUE);

	/* Step 4: set parameters for decompression */


	/* Step 5: Start decompressor */

	(void) jpeg_start_decompress(&cinfo);

	/* JSAMPLEs per row in output buffer */
	row_stride = cinfo.output_width * cinfo.output_components;

	/* Make a one-row-high sample array that will go away when done with image */
	buffer = (*cinfo.mem->alloc_sarray)
		((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

	/* Step 6: while (scan lines remain to be read) */
	/*           jpeg_read_scanlines(...); */
    
	while (cinfo.output_scanline < cinfo.output_height) {
	/* jpeg_read_scanlines expects an array of pointers to scanlines.
	 * Here the array is only one element long, but you could ask for
	 * more than one scanline at a time if that's more convenient.
	 */
		(void) jpeg_read_scanlines(&cinfo, buffer, 1);
	/* Assume put_scanline_someplace wants a pointer and sample count. */
	// printf("  cinfo.output_scanline = %d \n  size = row_stride\n");
	// 将24bit RGB 刷新到LCD
	//将bmp_buf 24bit的图像数据写入32bit的fb_mem
   
	    if( ((cinfo.output_scanline-1) % 2) == 0)
	    {	
		for(i=0,x=0;x<cinfo.output_width;x+=2, i+=6)
				{
					micro_buf[j++] = ( buffer[0][i]<<16|buffer[0][i+1]<<8|buffer[0][i+2]<<0);
					if (  (buffer[0][i]) > 180  &&   (buffer[0][i+1]) > 100  )
						count++;
				}
		}
	}

	/* Step 7: Finish decompression */

	(void) jpeg_finish_decompress(&cinfo);
	/* We can ignore the return value since suspension is not possible
	* with the stdio data source.
	*/

	/* Step 8: Release JPEG decompression object */

	/* This is an important step since it will release a good deal of memory. */
	jpeg_destroy_decompress(&cinfo);
	fclose(infile);

	if (count > 200*120)
		return 1;

	return 0;
	
}




void write_my_JPEG_file (char * filename, int quality,unsigned long *my_buffer)
{
	JSAMPLE * image_buffer;
	int i,x,y;
	//unsigned long  use_buffer[800*480*3];
	image_buffer = (char*)malloc(800*480*3);
	printf("okokokoko\n");

	// for(i=0; i< 800*480; i++)
	// {
	// 	 image_buffer[i*3] = (fb_mem[i] & 0xff0000)>>16;//i*255;
	// 	 image_buffer[i*3+1] = (fb_mem[i] & 0xff00)>>8;// 128-(i*255)&0x7f;
	// 	 image_buffer[i*3+2] = fb_mem[i]  & 0xff;//255-(i*255)&0xff;
	// }


	for (i = 0,x=0; i<800*480; i++)
	{
		image_buffer[x+0]=((my_buffer[i]>>16)&0xFF);
		image_buffer[x+1]=((my_buffer[i]>>8)&0xFF);
		image_buffer[x+2]=((my_buffer[i]>>0)&0xFF);

		//printf("%lx -%lx-%lx-  %d  \n",use_buffer[x+0],use_buffer[x+1],use_buffer[x+2],i );
		x=x+3;
	}

	//image_buffer = (JSAMPLE *)use_buffer;
	printf("oko------oko\n");

  struct jpeg_compress_struct cinfo;
 
  struct jpeg_error_mgr jerr;
  /* More stuff */
  FILE * outfile;		/* target file */
  JSAMPROW row_pointer[1];	/* pointer to JSAMPLE row[s] */
  int row_stride;		/* physical row width in image buffer */

  cinfo.err = jpeg_std_error(&jerr);

  jpeg_create_compress(&cinfo);

  if ((outfile = fopen(filename, "wb")) == NULL) {
    fprintf(stderr, "can't open %s\n", filename);
    exit(1);
  }
  jpeg_stdio_dest(&cinfo, outfile);

  cinfo.image_width = 800; 
  cinfo.image_height = 480;
  cinfo.input_components = 3;		
  cinfo.in_color_space = JCS_RGB; 
 
  jpeg_set_defaults(&cinfo);
 
  jpeg_set_quality(&cinfo, quality, TRUE );

  jpeg_start_compress(&cinfo, TRUE);
  row_stride = 800 * 3;


 	
	

  while (cinfo.next_scanline < cinfo.image_height) {
    row_pointer[0] = & image_buffer[cinfo.next_scanline * row_stride];
    (void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
  }
  jpeg_finish_compress(&cinfo);
  fclose(outfile);
  jpeg_destroy_compress(&cinfo);
}



int get_micro_jpg(char *pic_name,unsigned long *pic_buf)
{

	int x,y,i=0,j=0;
	//unsigned long micro_buf[SCREEN_WIDTH/2*SCREEN_HEIGH/2];
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;
    FILE * infile;	
	JSAMPARRAY  buffer;	
	int row_stride;		

	if ((infile = fopen(pic_name, "rb")) == NULL) {
		fprintf(stderr, "can't open %s\n", pic_name);
		return 0;
	}

	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);

	jpeg_stdio_src(&cinfo, infile);

	(void) jpeg_read_header(&cinfo, TRUE);

	(void) jpeg_start_decompress(&cinfo);

	row_stride = cinfo.output_width * cinfo.output_components;

	buffer = (*cinfo.mem->alloc_sarray)
		((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

	while (cinfo.output_scanline < cinfo.output_height) {
	(void) jpeg_read_scanlines(&cinfo, buffer, 1);

    if( ((cinfo.output_scanline-1) % 2) == 0)
    {	
		for(i=0,x=0;x<cinfo.output_width;x+=2, i+=6)
				{
					*(pic_buf+j) = ( buffer[0][i]<<16|buffer[0][i+1]<<8|buffer[0][i+2]<<0);
					j++;
				}
		}
	}


	(void) jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
	fclose(infile);


	// for(i=0,y=100;y<100+240;y++)
	// 	for(x=200;x<200+400;x++)
	// 	{
	// 		 fb_mem[y*SCREEN_WIDTH+x]   =   micro_buf[i++];
	// 	}
}