/*
a piece of implention:
		else if(strlen(inputBuf)>=6&&strncmp(inputBuf,"exec ",5)==0)
		{
			char* others=inputBuf+5;
			if(strpbrk(others, "|&;<>`$\"\'\\/!*?[]{}()~") != NULL) {
				printf("Invalid syntax!\n");
				printf("[+1]%s terminated by SIGINOS(Invalid Operation Syntax)",inputBuf);
				goto agsh;
			}
			char file[130]="ROOTFS\\";
			strcat(file,others);
			if(progs>=maxProg)
			{
				printf("Error:No enough memory to execute other programs!Close at least one program to continue...\n");
				continue;
			}
			progs++;
			strncpy(fileloc[progs-1], file, sizeof(fileloc[0])-1);
			int ret=rbvm(progs-1);
			if(ret==3)
			{
				continue;
			}
			progs--;
		}
		else if(strcmp(inputBuf,"clear")==0)
		{
			system("cls");
		}
		else if(strcmp(inputBuf,"shutdown")==0)
		{
            system("shutdown s");
		}
		else if(strcmp(inputBuf,"poweroff")==0)
		{
			system("shutdown s");
		}
		else if(strcmp(inputBuf,"restart")==0)
		{
			system("shutdown r");
		}
		else if(strcmp(inputBuf,"reboot")==0)
		{
			system("shutdown r");
		}
		else if(strcmp(inputBuf,"pwd")==0)
		{
			printf("ROOTFS/ \n");
		}
		else if(strcmp(inputBuf,"ver")==0)
		{
            printf("AEOS v5.11.2 Beta 7.1 Build 8070 Kernel Patch 4\n");
			printf("root@AEOS51\n");
			printf("(C)Copyright 2022~2026 Lithium Project LLC\n");
			printf("By Lithium4141\n");
            printf("Release date:2026/3/8\n");
		}
		else if(strcmp(inputBuf,"ps")==0)
		{
			if(progs<=0)
			{
				printf("No process running\n");
				continue;
			}
			printf("Totally %d process running:\n");
			printf("PID | Process name\n");
			for(int i=1;i<=progs;i++)
			{
				printf("%d   | %s\n",i-1,fileloc[i-1]);
			}
		}
		else if(strcmp(inputBuf,"help")==0)
		{
			printf("Built in commands:\n");
			printf("exit       ls       echo       exec\n");
			printf("clear      shutdown poweroff   restart\n");
			printf("reboot     pwd      ver        ps\n");
			printf("help       kill     open       bgidm\n");
			printf("passwd     whoami   lsusr      arm\n");
            printf("syscfg     cdufetch\n");
			printf("Note:To stop a program,press Ctrl-\\\n");
			printf("     To leave a program in background,try press Ctrl-Z\n");
		}
		else if(strlen(inputBuf)>=6&&strncmp(inputBuf,"kill ",5)==0)
		{
			int pid=atoi(inputBuf+5);
			if(pid<0||pid>=progs)
			{
				printf("Invalid PID:%d\n",pid);
				continue;
			}
			printf("Killed %d:%s\n",pid,fileloc[pid]);
			memset(fileloc[pid],'\0',sizeof(fileloc[pid]));
			progs--;
			fclose(fin[pid]);
            if(pid<(progs))
			{
                for(int i=pid+1;i<=progs;i++)
				{
                    strcpy(fileloc[i-1],fileloc[i]);
					//note:debug codes are disabled
					//printf("[debug]Copying file pointer from %d-1(%d) to %d...\n",i,i-1,i);
					//printf("[debug]Address of fin[%d]:%d\n[debug]Address of fin[%d]:%d\n",i-1,&fin[i-1],i,&fin[i]);
                    //printf("[debug]Data of fin[%d]:%d\n[debug]Data of fin[%d]:%d\n",i-1,fin[i-1],i,fin[i]);
					//memcpy(fin[i-1],fin[i],sizeof(fin[i]));
					fin[i-1]=fin[i];
					memcpy(memi[i], memi[i+1], sizeof(memi[0]));
					memcpy(memf[i], memf[i+1], sizeof(memf[0]));
					memcpy(memc[i], memc[i+1], sizeof(memc[0]));
                    memcpy(mems[i], mems[i+1], sizeof(mems[0]));
					memcpy(sect[i], sect[i+1], sizeof(sect[0]));
					//printf("[debug]Operation done successfully!\n");
					//printf("[debug]Address of fin[%d]:%d\n[debug]Address of fin[%d]:%d\n",i-1,&fin[i-1],i,&fin[i]);
                    //printf("[debug]Data of fin[%d]:%d\n[debug]Data of fin[%d]:%d\n",i-1,fin[i-1],i,fin[i]);
				}
			}
*/
/*
Borland Graphics Interface Display Manager for AEOS v5.11.2
This version is still a beta version(not stable),which means there will be some bugs,and it will be fixed in future updates
Note:STOP ASKING ME TO FIX THE REPAINTING PROBLEM,BECAUSE I AM STILL WORKING ON IT,EVERY RELEASE!!!
Known issue:BGI's default double-buffered render system cannot work properly.Pixels under Y=360 cannot be rendered.Although I implented it,it is still disabled by default.If you have better solutions,commit on github or send to me by e-mail
Bug report:send an email to lithium-offical@outlook.com or commit in github
IMPORTANT NOTICE:Double-buffered Render System will NOT CONTINUE RECEIVE ANY UPDATES!THIS MEANS DBR IS STILL USING THE LEGACY DESKTOP NOT THE LATEST!!!DO NOT USE IT IF YOU WANT TO TRY BUGS AND ABANDONED DESKTOP!
*/
#ifndef bgidm
#define bgidm
#endif
#include <graphics.h>
#include <signal.h>
#include <conio.h>
#include <dos.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>


extern "C"{
	#include "A:\rbvm.h"
}

#define BT_LEFT 0xF1
#define BT_RIGHT 0xF2

int refRate=2;
int maxRefRate=60,windowsOpening=0;

union REGS inregs, outregs;

struct MOUSE_STATUS {
	int x;
	int y;
	int left_button;
	int right_button;
};

int mouse_init()
{
	inregs.x.ax = 0x0000;
	int86(0x33, &inregs, &outregs);
	return outregs.x.ax;
}

void mouse_show()
{
	inregs.x.ax = 0x0001;
	int86(0x33, &inregs, &outregs);
}

void mouse_hide()
{
	inregs.x.ax = 0x0002;
	int86(0x33, &inregs, &outregs);
}

struct MOUSE_STATUS get_mouse_position()
{
	struct MOUSE_STATUS ms;
	inregs.x.ax = 0x0003;
	int86(0x33, &inregs, &outregs);
	ms.x = outregs.x.cx;
	ms.y = outregs.x.dx;
	ms.left_button = (outregs.x.bx & 1) ? 1 : 0;
	ms.right_button = (outregs.x.bx & 2) ? 1 : 0;
	return ms;
}

int get_mouse_left()
{
	inregs.x.ax = 0x0003;
	int86(0x33, &inregs, &outregs);
	return (outregs.x.bx & 1) ? 1 : 0;
}

int get_mouse_right()
{
	inregs.x.ax = 0x0003;
	int86(0x33, &inregs, &outregs);
	return (outregs.x.bx & 2) ? 1 : 0;
}

struct MOUSE_STATUS prev_mouse;

int mouse_changed()
{
	struct MOUSE_STATUS current = get_mouse_position();
    if(abs(prev_mouse.x-current.x)>3) return 1;
    if(abs(prev_mouse.y-current.y)>3) return 1;
	if(prev_mouse.left_button != current.left_button) return 1;
	if(prev_mouse.right_button != current.right_button) return 1;
	prev_mouse = current;
	return 0;
}

void datetime(int x, int y) {
	struct date d;
	struct time t;
	char datetime[30];
	getdate(&d);
	gettime(&t);
	sprintf(datetime, "%02d/%02d/%02d %02d:%02d",d.da_year % 100,d.da_mon, d.da_day, t.ti_hour,t.ti_min);
	setcolor(0);
	outtextxy(x, y, datetime);
}



void render_pixel_map_transparent(int map[8][8], int start_x, int start_y, int scale)
{
	int i, j, si, sj;
	for(i = 0; i < 8; i++) {
		for(j = 0; j < 8; j++) {
			if(map[i][j] != 0) {
				for(si = 0; si < scale; si++) {
					for(sj = 0; sj < scale; sj++) {
						putpixel(start_x + (j * scale) + sj, start_y + (i * scale) + si, map[i][j]);
					}
				}
			}
		}
	}
}

void render_pixel_map(int map[8][8], int start_x, int start_y, int scale)
{
	int i, j, si, sj;
	for(i = 0; i < 8; i++) {
		for(j = 0; j < 8; j++) {
			for(si = 0; si < scale; si++) {
				for(sj = 0; sj < scale; sj++) {
					putpixel(start_x + (j * scale) + sj, start_y + (i * scale) + si, map[i][j]);
				}
			}
		}
	}
}

void render_colorful_pixelmap(int map[8][8], int start_x, int start_y, int scale)
{
	int i, j, si, sj;
	for(i = 0; i < 8; i++) {
		for(j = 0; j < 8; j++) {
			if(map[i][j] != -1) {
				for(si = 0; si < scale; si++) {
					for(sj = 0; sj < scale; sj++) {
						setcolor(map[i][j]);
						putpixel(start_x + (j * scale) + sj, start_y + (i * scale) + si, map[i][j]);
					}
				}
			}
		}
	}
}
/*
void copy_page(int src_page, int dst_page) {
	unsigned char far *vga_memory = (unsigned char far *)0xA0000000L;
    long page_size = 640 * 480 / 2;
	long src_offset, dst_offset;
    src_offset = src_page * 0x10000L;
	dst_offset = dst_page * 0x10000L;
	movedata(FP_SEG(vga_memory) + (src_offset >> 4),
			 FP_OFF(vga_memory) + (src_offset & 0xF),
			 FP_SEG(vga_memory) + (dst_offset >> 4),
			 FP_OFF(vga_memory) + (dst_offset & 0xF),
			 page_size);
	setvisualpage(dst_page);
}*/
void copy_page(int src_page, int dst_page) {
	void *buffer;
	unsigned int size;
	size = imagesize(0, 0, 639, 479);
	buffer = malloc(size);
	if (buffer) {
		setactivepage(src_page);
		getimage(0, 0, 639, 479, buffer);
		setactivepage(dst_page);
		putimage(0, 0, buffer, COPY_PUT);
		free(buffer);
	}
	setvisualpage(dst_page);
}
void draw_thick_rectangle(int x1, int y1, int x2, int y2, int thickness, int bdcolor, int flcolor)
{
	int t;
	setfillstyle(SOLID_FILL, flcolor);
	bar(x1+thickness, y1+thickness, x2-thickness, y2-thickness);
	setcolor(bdcolor);
	for(t = 0; t < thickness; t++) {
		rectangle(x1+t, y1+t, x2-t, y2-t);
	}
}
int pongPlayerPos=30,pongCpuPlayerPos=30,playerScore=0,cpuScore=0,ballX=30,ballY=5;
char dispPix[78][52];
int mouse_cursor[8][8]={
	{1,0,0,0,0,0,0,0},
	{1,1,0,0,0,0,0,0},
	{1,1,1,0,0,0,0,0},
	{1,1,1,1,0,0,0,0},
	{1,1,1,0,0,0,0,0},
	{1,0,1,0,0,0,0,0},
	{0,0,0,1,0,0,0,0},
	{0,0,0,1,0,0,0,0}
};
/*
int pong[8][8]={
	{0,0,0,1,1,0,0,0},
	{0,0,0,1,1,0,0,0},
	{0,0,0,1,1,0,0,0},
	{0,0,0,1,1,0,0,0},
	{0,0,0,1,1,0,0,0},
	{0,0,0,1,1,0,0,0},
	{0,0,0,1,1,0,0,0},
	{0,0,0,1,1,0,0,0}
};
int ball[8][8]={
	{0,0,0,1,1,0,0,0},
	{0,0,0,1,1,0,0,0},
	{0,0,0,1,1,0,0,0},
	{0,0,0,1,1,0,0,0},
	{0,0,0,1,1,0,0,0},
	{0,0,0,1,1,0,0,0},
	{0,0,0,1,1,0,0,0},
	{0,0,0,1,1,0,0,0}
};
*/
int document_icon[8][8]={
	{0,3,2,2,2,2,2,0},
	{3,3,1,1,1,1,2,0},
	{2,2,2,2,2,2,2,0},
	{2,1,1,1,1,1,2,0},
	{2,2,2,2,2,2,2,0},
	{2,1,1,1,1,1,2,0},
	{2,2,2,2,2,2,2,0},
	{2,2,2,2,2,2,2,0}
};
int error_icon[8][8]={
	{0,0,5,5,5,5,0,0},
	{0,5,5,5,5,5,5,0},
	{5,5,5,5,5,5,5,5},
	{5,2,2,2,2,2,2,5},
	{5,2,2,2,2,2,2,5},
	{5,5,5,5,5,5,5,5},
	{0,5,5,5,5,5,5,0},
	{0,0,5,5,5,5,0,0}
};
int question_icon[8][8]={
	{0,0,6,6,6,6,0,0},
	{0,6,2,2,2,6,6,0},
	{6,2,6,6,6,2,6,6},
	{6,6,6,6,6,2,6,6},
	{6,6,6,6,2,6,6,6},
	{6,6,6,6,6,6,6,6},
	{0,6,6,6,2,6,6,0},
	{0,0,6,6,6,6,0,0}
};int fileman_icon[8][8]={
	{7,7,7,0,0,0,0,0},
	{7,7,7,7,7,7,7,7},
	{7,3,2,2,2,2,2,7},
	{3,3,1,1,1,1,2,7},
	{7,7,7,7,7,7,7,7},
	{7,7,7,7,7,7,7,7},
	{7,7,7,7,7,7,7,7},
	{7,7,7,7,7,7,7,7}
};
void def_color()
{
	setrgbpalette(0,0,0,0);
	setrgbpalette(1,60,60,60);
	setrgbpalette(2,63,63,63);
	setrgbpalette(3,45,45,45);
	setrgbpalette(4,15,15,15);
	setrgbpalette(5,63,0,0);
	setrgbpalette(6,0,0,63);
	setrgbpalette(7,60,60,0);
	setrgbpalette(8,55,55,0);
}

int startmenu=0,powermenu=0,debuginfo=0,aboutinfo=0,vtermon=0,filemanon=0,demoon=0,lines=1,currentprog=0;

int tkrt=0;
char files[32][14];

int keyboard_control = 0,key=0;
void fix_bgi_page1_mapping() {
	union REGS regs;
	regs.h.ah = 0x13;
	regs.x.cx = 0;
	regs.x.dx = 0;
	int86(0x10, &regs, &regs);

	// int gmode = getgraphmode();
	// setgraphmode(gmode);
}
struct MOUSE_STATUS mouse={0,0,0,0};
void render_dbginfo()
{
	setcolor(0);
	bar(10,10,630,27);
	setcolor(2);
	settextstyle(TRIPLEX_FONT,HORIZ_DIR,2);
	outtextxy(10,10,"BGIDM v1.11.0 on AEOS v5.11.2 Build 8070 Kernel Patch 4");
	char buf[128];
	sprintf(buf,"X:%d Y:%d Left key:%s Right key:%s",mouse.x,mouse.y,(mouse.left_button==1?"ON":"OFF"),(mouse.right_button==1?"ON":"OFF"));
	outtextxy(10,27,buf);
	if(key==-1)
	{
		sprintf(buf,"Keyboard Character:  Keyboard ASCII: ");
	}
	else
	{
		sprintf(buf,"Keyboard Character:%c Keyboard ASCII:%d",(char)key,(int)key);
	}
	outtextxy(10,44,buf);
	sprintf(buf,"Maximum Refresh Rate:%d Render Tick Delay:%d",maxRefRate,refRate);
	outtextxy(10,61,buf);
	sprintf(buf,"Current Tick:%d",tkrt);
	outtextxy(10,78,buf);
}
void render_aboutinfo()
{
	setcolor(1);
	draw_thick_rectangle(170,210,550,330,5,1,0);
	setcolor(2);
	line(170,240,550,240);
	settextstyle(TRIPLEX_FONT, HORIZ_DIR, 2);
	outtextxy(180,210,"[X]About AEOS");
	outtextxy(180,240,"AEOS v5.11.2 Build 8070 Patch 1");
	outtextxy(180,270,"AEOS BGIDM v1.11.0 Beta 7.1 LFR");
	outtextxy(190,300,"By Lithium4141");
	if(key==27)
	{
		aboutinfo=0;
	}
	if((mouse.x>=170&&mouse.x<=200)&&(mouse.y>=210&&mouse.y<=240)&&(mouse.left_button==1||key=='\r'||key=='\n'))
	{
		while((mouse.x>=170&&mouse.x<=200)&&(mouse.y>=210&&mouse.y<=240)&&(mouse.left_button==1))
		{
			mouse = get_mouse_position();
		}
		aboutinfo=0;
		windowsOpening=clamp(windowsOpening-1,0,5);
	}
	while(!kbhit()&&!mouse_changed());
}
int render_powermenu()
{
	draw_thick_rectangle(170,210,450,280,5,1,0);
	setcolor(2);
	line(170,240,450,240);
	settextstyle(TRIPLEX_FONT, HORIZ_DIR, 2);
	outtextxy(180,210,"Power Options");
	outtextxy(180,240,"S        |R     |E   |C      ");
	settextstyle(TRIPLEX_FONT, HORIZ_DIR,1);
	outtextxy(190,243,"hutdown  eboot  xit  ancel");
	if(key==27)
	{
		powermenu=0;
	}
	if((mouse.x>=180&&mouse.x<=210)&&(mouse.y>=240&&mouse.y<=280)&&(mouse.left_button==1||key=='\r'||key=='\n'))
	{
		while((mouse.x>=180&&mouse.x<=210)&&(mouse.y>=240&&mouse.y<=280)&&(mouse.left_button==1))
		{
			mouse = get_mouse_position();
		}
		system("shutdown s");
	}
	else if((mouse.x>=210&&mouse.x<=340)&&(mouse.y>=240&&mouse.y<=280)&&(mouse.left_button==1||key=='\r'||key=='\n'))
	{
		while((mouse.x>=210&&mouse.x<=340)&&(mouse.y>=240&&mouse.y<=280)&&(mouse.left_button==1))
		{
			mouse = get_mouse_position();
		}
		system("shutdown r");
	}
	else if((mouse.x>=340&&mouse.x<=380)&&(mouse.y>=240&&mouse.y<=280)&&(mouse.left_button==1||key=='\r'||key=='\n'))
	{
		while((mouse.x>=340&&mouse.x<=380)&&(mouse.y>=240&&mouse.y<=280)&&(mouse.left_button==1))
		{
			mouse = get_mouse_position();
		}
		return 1;
	}
	else if((mouse.x>=380&&mouse.x<=450)&&(mouse.y>=240&&mouse.y<=280)&&(mouse.left_button==1||key=='\r'||key=='\n'))
	{
		while((mouse.x>=380&&mouse.x<=450)&&(mouse.y>=240&&mouse.y<=280)&&(mouse.left_button==1))
		{
			mouse = get_mouse_position();
		}
		powermenu=0;
		windowsOpening=clamp(windowsOpening-1,0,5);
	}
	while(!kbhit()&&!mouse_changed());
	return 0;
}
void render_demo()
{
	if(demoon==1)
	{
		setcolor(1);
		draw_thick_rectangle(0,0,640,454,5,1,0);
		line(0,30,640,30);
		line(0,70,640,70);
		line(0,100,640,100);
		line(0,130,640,130);
		setcolor(2);
		outtextxy(0,0,"[X]AEOS Demos");
		settextstyle(TRIPLEX_FONT,HORIZ_DIR,3);
		outtextxy(5,30,"Select one program:");
		settextstyle(TRIPLEX_FONT,HORIZ_DIR,2);
		outtextxy(5,70,"1.Paint");
		outtextxy(5,100,"2.Ping-Pong");
		if((mouse.x>=5&&mouse.x<=635)&&(mouse.y>=70&&mouse.y<=100)&&(mouse.left_button==1||key=='\r'||key=='\n'))
		{
			while((mouse.x>=5&&mouse.x<=635)&&(mouse.y>=70&&mouse.y<=100)&&(mouse.left_button==1))
			{
				mouse = get_mouse_position();
			}
			demoon=2;
		}
		if((mouse.x>=5&&mouse.x<=635)&&(mouse.y>=100&&mouse.y<=130)&&(mouse.left_button==1||key=='\r'||key=='\n'))
		{
			while((mouse.x>=5&&mouse.x<=635)&&(mouse.y>=100&&mouse.y<=130)&&(mouse.left_button==1))
			{
				mouse = get_mouse_position();
			}
			demoon=3;
		}
		if((mouse.x>=0&&mouse.x<=30)&&(mouse.y>=0&&mouse.y<=30)&&(mouse.left_button==1||key=='\r'||key=='\n'))
		{
			while((mouse.x>=0&&mouse.x<=30)&&(mouse.y>=0&&mouse.y<=30)&&(mouse.left_button==1))
			{
				mouse = get_mouse_position();
			}
			demoon=0;
			windowsOpening=clamp(windowsOpening-1,0,5);
		}
	}
	else if(demoon==2)
	{
		setcolor(1);
		draw_thick_rectangle(0,0,640,454,5,1,0);
		line(0,30,640,30);
		setcolor(2);
		outtextxy(0,0,"[<]Paint - AEOS Demos");
		if((mouse.x>=0&&mouse.x<=30)&&(mouse.y>=0&&mouse.y<=30)&&(mouse.left_button==1||key=='\r'||key=='\n'))
		{
			while((mouse.x>=0&&mouse.x<=30)&&(mouse.y>=0&&mouse.y<=30)&&(mouse.left_button==1))
			{
				mouse = get_mouse_position();
			}
			demoon=1;
		}
		if((mouse.x>=5&&mouse.x<=635)&&(mouse.y>=30&&mouse.y<=450)&&(mouse.left_button==1||key=='\r'))
		{
			dispPix[(mouse.x-5)/8][(mouse.y-30)/8]=1;
		}
		if((mouse.x>=5&&mouse.x<=635)&&(mouse.y>=30&&mouse.y<=450)&&(mouse.left_button==2||key=='\n'))
		{
			dispPix[(mouse.x-5)/8][(mouse.y-30)/8]=0;
		}
		for(int i=1;i<=78;i+=1)
		{
			for(int j=1;j<=52;j+=1)
			{
				if(dispPix[i][j]==1)
				{
					for(int _i=(i*8)+5;_i<=(i*8)+8+5;_i++)
					{
						for(int _j=(j*8)+30;_j<=(j*8)+8+30;_j++)
						{
							putpixel(_i,_j,2);
						}
					}
				}
			}
		}
	}
	else if(demoon==3)
	{
		draw_thick_rectangle(170,210,450,290,5,1,0);
		setcolor(2);
		line(170,240,450,240);
		settextstyle(TRIPLEX_FONT, HORIZ_DIR, 2);
		outtextxy(180,210,"[X]Error");
		render_colorful_pixelmap(error_icon,185,246,3);
		setcolor(2);
		outtextxy(213,240,"File not found");
		if(key==27)
		{
			demoon=1;
		}
		if((mouse.x>=180&&mouse.x<=210)&&(mouse.y>=210&&mouse.y<=240)&&(mouse.left_button==1||key=='\r'||key=='\n'))
		{
			while((mouse.x>=180&&mouse.x<=210)&&(mouse.y>=210&&mouse.y<=240)&&(mouse.left_button==1))
			{
				mouse = get_mouse_position();
			}
			demoon=1;
		}
		while(!kbhit()&&!mouse_changed());
	}
}
void render_fileman()
{
	setcolor(1);
	rectangle(35,60,150,130);
	draw_thick_rectangle(0,0,640,454,5,1,0);
	line(0,30,640,30);
	setcolor(2);
	outtextxy(0,0,"[X]File Manager");
	if(files==NULL)
	{
		setcolor(2);
		outtextxy(5,30,"File manager cannot display any file");
	}
	else if(lines<=0)
	{
		setcolor(2);
		outtextxy(5,30,"File manager cannot find any file");
	}
	else
	{
		for(int i=0;i<lines;i++) {
			if(i!=0)
			{
				render_colorful_pixelmap(document_icon,5,i*30+2,2);
			}
			setcolor(2);
			outtextxy(22,i*30,files[i]);
		}
	}
	if((mouse.x>=0&&mouse.x<=30)&&(mouse.y>=0&&mouse.y<=30)&&(mouse.left_button==1||key=='\r'||key=='\n'))
	{
		while((mouse.x>=0&&mouse.x<=30)&&(mouse.y>=0&&mouse.y<=30)&&(mouse.left_button==1))
		{
			mouse = get_mouse_position();
		}
		filemanon=0;
		currentprog=0;
		windowsOpening=clamp(windowsOpening-1,0,5);
		//system("del SYSCFG\\file.idx");
	}
}
int isMouseOn(int x1,int x2,int y1,int y2,int _key)
{
	if(_key==BT_LEFT)
	{
		if((mouse.x>=x1&&mouse.x<=x2)&&(mouse.y>=y1&&mouse.y<=y2)&&(mouse.left_button==1||key=='\r'||key=='\n'))
		{
			while((mouse.x>=x1&&mouse.x<=x2)&&(mouse.y>=y1&&mouse.y<=y2)&&(mouse.left_button==1))
			{
				mouse = get_mouse_position();
			}
			return 1;
		}
		return 0;
	}
	else if(_key==BT_RIGHT)
	{
		if((mouse.x>=x1&&mouse.x<=x2)&&(mouse.y>=y1&&mouse.y<=y2)&&(mouse.right_button==1||key==' '))
		{
			while((mouse.x>=x1&&mouse.x<=x2)&&(mouse.y>=y1&&mouse.y<=y2)&&(mouse.right_button==1))
			{
				mouse = get_mouse_position();
			}
			return 1;
		}
		return 0;
	}
	return -1;
}
void render_startmenu()
{

	draw_thick_rectangle(5,244,180,454,5,1,0);
	setcolor(1);
	line(30,244,30,454);
	line(30,430,180,430);
	line(30,406,180,406);
	line(30,380,180,380);
	line(30,356,180,356);
	line(30,332,180,332);
	line(30,308,180,308);
	setcolor(2);
	settextstyle(TRIPLEX_FONT, VERT_DIR, 2);
	outtextxy(6,245,"AEOS v5.11.2 BGIDM");
	settextstyle(TRIPLEX_FONT, HORIZ_DIR, 2);
	outtextxy(30,428,"Power Options");
	char* buf1;
	sprintf(buf1,"Debug info %s",(debuginfo==1?"ON":"OFF"));
	outtextxy(30,404,buf1);
	outtextxy(30,380,"About AEOS");
	outtextxy(30,356,"VTerm");
	outtextxy(30,332,"FileMan");
	outtextxy(30,308,"Demos");
	if(key==27)
	{
		startmenu=0;
	}
	if((mouse.x>=30&&mouse.x<=180)&&(mouse.y>=430&&mouse.y<=454)&&(mouse.left_button==1||key=='\r'||key=='\n'))
	{
		if(powermenu==1)
		{
			powermenu=0;
			windowsOpening=clamp(windowsOpening-1,0,5);
		}
		else
		{
			powermenu=1;
			windowsOpening=clamp(windowsOpening+1,0,5);
		}
		while((mouse.x>=30&&mouse.x<=180)&&(mouse.y>=430&&mouse.y<=454)&&(mouse.left_button==1))
		{
			mouse = get_mouse_position();
		}
	}
	else if((mouse.x>=30&&mouse.x<=180)&&(mouse.y>=404&&mouse.y<=454)&&(mouse.left_button==1||key=='\r'||key=='\n'))
	{
		if(debuginfo==1)
		{
			debuginfo=0;
			windowsOpening=clamp(windowsOpening-1,0,5);
		}
		else
		{
			debuginfo=1;
			windowsOpening=clamp(windowsOpening+1,0,5);
		}
		while((mouse.x>=30&&mouse.x<=180)&&(mouse.y>=404&&mouse.y<=454)&&(mouse.left_button==1))
		{
			mouse = get_mouse_position();
		}
	}
	else if((mouse.x>=30&&mouse.x<=180)&&(mouse.y>=380&&mouse.y<=404)&&(mouse.left_button==1||key=='\r'||key=='\n'))
	{
		if(aboutinfo==1)
		{
			aboutinfo=0;
			windowsOpening=clamp(windowsOpening-1,0,5);
		}
		else
		{
			aboutinfo=1;
			windowsOpening=clamp(windowsOpening+1,0,5);
		}
		while((mouse.x>=30&&mouse.x<=180)&&(mouse.y>=380&&mouse.y<=404)&&(mouse.left_button==1))
		{
			mouse = get_mouse_position();
		}
	}
	else if((mouse.x>=30&&mouse.x<=180)&&(mouse.y>=356&&mouse.y<=380)&&(mouse.left_button==1||key=='\r'||key=='\n'))
	{
		if(vtermon==1)
		{
			vtermon=0;
			windowsOpening=clamp(windowsOpening-1,0,5);
		}
		else
		{
			vtermon=1;
			windowsOpening=clamp(windowsOpening+1,0,5);
		}
		while((mouse.x>=30&&mouse.x<=180)&&(mouse.y>=356&&mouse.y<=380)&&(mouse.left_button==1))
		{
			mouse = get_mouse_position();
		}
	}
	else if((mouse.x>=30&&mouse.x<=180)&&(mouse.y>=332&&mouse.y<=356)&&(mouse.left_button==1||key=='\r'||key=='\n'))
	{
		if(filemanon==1)
		{
			filemanon=0;
			windowsOpening=clamp(windowsOpening-1,0,5);
		}
		else
		{
			filemanon=1;
			windowsOpening=clamp(windowsOpening+1,0,5);
			//system("del SYSCFG\\file.idx");
			system("dir ROOTFS /b > SYSCFG\\file.idx");
			FILE* fin=fopen("SYSCFG\\file.idx","r");
			lines=1;
			if(fin!=NULL)
			{
				while(fscanf(fin,"%s",files[lines])==1)
				{
					lines++;
					if(lines>14)
					{
						break;
					}
				}
			}
			fclose(fin);
			system("del SYSCFG\\file.idx");
		}
		while((mouse.x>=30&&mouse.x<=180)&&(mouse.y>=332&&mouse.y<=356)&&(mouse.left_button==1))
		{
			mouse = get_mouse_position();
		}
	}
	else if((mouse.x>=30&&mouse.x<=180)&&(mouse.y>=308&&mouse.y<=332)&&(mouse.left_button==1||key=='\r'||key=='\n'))
	{
		if(demoon!=0)
		{
			demoon=0;
			windowsOpening=clamp(windowsOpening-1,0,5);
		}
		else
		{
			demoon=1;
			windowsOpening=clamp(windowsOpening+1,0,5);
		}
		while((mouse.x>=30&&mouse.x<=180)&&(mouse.y>=308&&mouse.y<=332)&&(mouse.left_button==1))
		{
			mouse = get_mouse_position();
		}
	}
	while(!kbhit()&&!mouse_changed());
}
void render_vterm()
{

	//Actually I gave up developing this temporaly,so there is no actual function for this,just a error window
	draw_thick_rectangle(170,210,450,290,5,1,0);
	setcolor(2);
	line(170,240,450,240);
	settextstyle(TRIPLEX_FONT, HORIZ_DIR, 2);
	outtextxy(180,210,"[X]Error");
	render_colorful_pixelmap(error_icon,185,246,3);
	setcolor(2);
	outtextxy(213,240,"File not found");
	if(key==27)
	{
		vtermon=0;
	}
	if((mouse.x>=180&&mouse.x<=210)&&(mouse.y>=210&&mouse.y<=240)&&(mouse.left_button==1||key=='\r'||key=='\n'))
	{
		while((mouse.x>=180&&mouse.x<=210)&&(mouse.y>=210&&mouse.y<=240)&&(mouse.left_button==1))
		{
			mouse = get_mouse_position();
		}
		vtermon=0;
	}
	while(!kbhit()&&!mouse_changed());
}
int render_exitconfirm()
{
	draw_thick_rectangle(170,210,450,310,5,1,0);
	setcolor(2);
	line(170,240,450,240);
	settextstyle(TRIPLEX_FONT, HORIZ_DIR, 2);
	outtextxy(180,210,"End Session");
	render_colorful_pixelmap(question_icon,185,246,3);
	setcolor(2);
	outtextxy(213,240,"Exit BGIDM?");
	line(170,270,450,270);
	outtextxy(213,270,"Yes");
	outtextxy(330,270,"No");
	if((mouse.x>=213&&mouse.x<=329)&&(mouse.y>=270&&mouse.y<=300)&&(mouse.left_button==1||key=='\r'||key=='\n'))
	{
		while((mouse.x>=213&&mouse.x<=329)&&(mouse.y>=270&&mouse.y<=300)&&(mouse.left_button==1))
		{
			mouse = get_mouse_position();
		}
		return 1;
	}
	else if((mouse.x>=330&&mouse.x<=450)&&(mouse.y>=270&&mouse.y<=300)&&(mouse.left_button==1||key=='\r'||key=='\n'))
	{
		while((mouse.x>=330&&mouse.x<=450)&&(mouse.y>=270&&mouse.y<=300)&&(mouse.left_button==1))
		{
			mouse = get_mouse_position();
		}
		return 0;
	}
	else
	{
		return -1;
	}
	while(!kbhit()&&!mouse_changed());
}
void render_mainmenu()
{
	draw_thick_rectangle(30,30,500,400,5,1,0);
	setcolor(2);
	line(30,60,500,60);
	settextstyle(TRIPLEX_FONT, HORIZ_DIR, 2);
	outtextxy(30,30,"[X]AEOS v5.11.2 BGIDM Program Manager");
	render_colorful_pixelmap(fileman_icon,50,61,4);
	settextstyle(TRIPLEX_FONT, HORIZ_DIR, 1);
	setcolor(2);
	outtextxy(37,95,"File Manager");
}
void render_taskbar()
{

	setcolor(1);
	setfillstyle(SOLID_FILL,1);
	bar(5,477,635,457);
	setcolor(0);
	putpixel(5, 476,0);
	putpixel(6, 475,0);
	putpixel(7, 477,0);
	putpixel(633, 476,0);
	putpixel(632, 475,0);
	putpixel(631, 477,0);
	putpixel(633, 459,0);
	putpixel(632, 460,0);
	putpixel(631, 458,0);
	putpixel(5, 459,0);
	putpixel(6, 460,0);
	putpixel(7, 458,0);
	settextstyle(TRIPLEX_FONT, HORIZ_DIR, 2);
	setcolor(0);
	outtextxy(5, 455, "AEOS|");
	datetime(455,455);
}

int copy_mode=0;
int main_dbr() {
	int gdriver = VGA, gmode = VGAHI;
	initgraph(&gdriver,&gmode,".\\BGI");
	int graphstatus=graphresult();
	if(graphstatus!=grOk) {
		closegraph();
		printf("BGI driver initilazation failed!Error:%s\n",grapherrormsg(graphstatus));
		return -1;
	}
    int back_buffer=1;
    int front_buffer=0;
	int render_page = 1;
	int display_page = 0;
	setactivepage(back_buffer);
	setvisualpage(front_buffer);
	def_color();
	mouse.x=320;
	mouse.y=240;
	int realMouse=0;
	if(mouse_init()!=0)
	{
		realMouse=1;
	}
	int tkrt=0;

    render_start:
	while(1)
	{
		tkrt++;
		fix_bgi_page1_mapping();
        setvisualpage(display_page);
        setactivepage(render_page);
        //settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
        //settextjustify(LEFT_TEXT, TOP_TEXT);
        //setlinestyle(SOLID_LINE, 0, NORM_WIDTH);
        //setfillstyle(SOLID_FILL, 1);
        setactivepage(0);
        setviewport(0, 0, 639, 479, 1);
        setactivepage(1);
        setviewport(0, 0, 639, 479, 1);
        setactivepage(back_buffer);
		def_color();
		setcolor(1);
		setbkcolor(0);
		cleardevice();

		input_process:
		if(realMouse==1 && keyboard_control == 0)
		{
			mouse=get_mouse_position();
		}

		if(tkrt>=16777216)
		{
			tkrt=0;
		}

		if(realMouse==1)
		{
			struct MOUSE_STATUS real_mouse = get_mouse_position();
			if(real_mouse.x != mouse.x || real_mouse.y != mouse.y)
			{
				keyboard_control = 0;
				mouse = real_mouse;
			}
		}

		key=getkey();
		if(key == 28)
		{
			goto end;
		}
		else if(key == 'A')
		{
			keyboard_control = 1;
			mouse.x=clamp(mouse.x-5,0,639);
		}
		else if(key == 'D')
		{
			keyboard_control = 1;
			mouse.x=clamp(mouse.x+5,0,639);
		}
		else if(key == 'S')
		{
			keyboard_control = 1;
			mouse.y=clamp(mouse.y+5,0,479);
		}
		else if(key == 'W')
		{
			keyboard_control = 1;
			mouse.y=clamp(mouse.y-5,0,479);
		}
		else if(key != -1)
		{
			keyboard_control = 1;
		}

		draw_background:
		setcolor(1);
		setfillstyle(SOLID_FILL,1);
		bar(5,477,635,457);
		setcolor(0);
		putpixel(5, 476,0);
		putpixel(6, 475,0);
		putpixel(7, 477,0);
		putpixel(633, 476,0);
		putpixel(632, 475,0);
		putpixel(631, 477,0);
		putpixel(633, 459,0);
		putpixel(632, 460,0);
		putpixel(631, 458,0);
		putpixel(5, 459,0);
		putpixel(6, 460,0);
		putpixel(7, 458,0);
		settextstyle(TRIPLEX_FONT, HORIZ_DIR, 2);
		setcolor(0);
		outtextxy(5, 455, "AEOS|");
		datetime(455,455);

		draw_interface:
		if((mouse.x>=5&&mouse.x<=53)&&(mouse.y>=457&&mouse.y<=477)&&(mouse.left_button==1||key=='\r'||key=='\n'))
		{
			if(startmenu==1)
			{
				startmenu=0;
				windowsOpening=clamp(windowsOpening-1,0,5);
			}
			else
			{
				startmenu=1;
				windowsOpening=clamp(windowsOpening-1,0,5);
			}
			while((mouse.x>=5&&mouse.x<=53)&&(mouse.y>=457&&mouse.y<=477)&&(mouse.left_button==1))
			{
				mouse = get_mouse_position();
			}
		}

		if(demoon!=0&&filemanon==0&&tkrt%(refRate/2)==0)
		{
			render_demo();
		}
		if(filemanon==1&&tkrt%(refRate/2)==0)
		{
			setcolor(1);
			draw_thick_rectangle(0,0,640,454,5,1,0);
			line(0,30,640,30);
			setcolor(2);
			outtextxy(0,0,"[X]File Manager");
			if(files==NULL)
			{
				setcolor(2);
				outtextxy(5,30,"File manager cannot display any file");
			}
			else if(lines<=0)
			{
				setcolor(2);
				outtextxy(5,30,"File manager cannot find any file");
			}
			else
			{
				for(int i=0;i<lines;i++) {
					if(i!=0)
					{
						render_colorful_pixelmap(document_icon,5,i*30+2,2);
					}
					setcolor(2);
					outtextxy(22,i*30,files[i]);
				}
			}
			if((mouse.x>=0&&mouse.x<=30)&&(mouse.y>=0&&mouse.y<=30)&&(mouse.left_button==1||key=='\r'||key=='\n'))
			{
				while((mouse.x>=0&&mouse.x<=30)&&(mouse.y>=0&&mouse.y<=30)&&(mouse.left_button==1))
				{
					mouse = get_mouse_position();
				}
				filemanon=0;
				currentprog=0;
				windowsOpening=clamp(windowsOpening-1,0,5);
			}
		}

		if(startmenu==1&&tkrt%refRate==0)
		{
			draw_thick_rectangle(5,244,180,454,5,1,0);
			setcolor(1);
			line(30,244,30,454);
			line(30,430,180,430);
			line(30,406,180,406);
			line(30,380,180,380);
			line(30,356,180,356);
			line(30,332,180,332);
			line(30,308,180,308);
			setcolor(2);
			settextstyle(TRIPLEX_FONT, VERT_DIR, 2);
			outtextxy(6,245,"AEOS v5.11.2 BGIDM");
			settextstyle(TRIPLEX_FONT, HORIZ_DIR, 2);
			outtextxy(30,428,"Power Options");
			char* buf1;
			sprintf(buf1,"Debug info %s",(debuginfo==1?"ON":"OFF"));
			outtextxy(30,404,buf1);
			outtextxy(30,380,"About AEOS");
			outtextxy(30,356,"VTerm");
			outtextxy(30,332,"FileMan");
			outtextxy(30,308,"Demos");
			if(key==27)
			{
				startmenu=0;
			}
			if((mouse.x>=30&&mouse.x<=180)&&(mouse.y>=430&&mouse.y<=454)&&(mouse.left_button==1||key=='\r'||key=='\n'))
			{
				if(powermenu==1)
				{
					powermenu=0;
					windowsOpening=clamp(windowsOpening-1,0,5);
				}
				else
				{
					powermenu=1;
					windowsOpening=clamp(windowsOpening+1,0,5);
				}
				while((mouse.x>=30&&mouse.x<=180)&&(mouse.y>=430&&mouse.y<=454)&&(mouse.left_button==1))
				{
					mouse = get_mouse_position();
				}
			}
			else if((mouse.x>=30&&mouse.x<=180)&&(mouse.y>=404&&mouse.y<=454)&&(mouse.left_button==1||key=='\r'||key=='\n'))
			{
				if(debuginfo==1)
				{
					debuginfo=0;
					windowsOpening=clamp(windowsOpening-1,0,5);
				}
				else
				{
					debuginfo=1;
					windowsOpening=clamp(windowsOpening+1,0,5);
				}
				while((mouse.x>=30&&mouse.x<=180)&&(mouse.y>=404&&mouse.y<=454)&&(mouse.left_button==1))
				{
					mouse = get_mouse_position();
				}
			}
			else if((mouse.x>=30&&mouse.x<=180)&&(mouse.y>=380&&mouse.y<=404)&&(mouse.left_button==1||key=='\r'||key=='\n'))
			{
				if(aboutinfo==1)
				{
					aboutinfo=0;
					windowsOpening=clamp(windowsOpening-1,0,5);
				}
				else
				{
					aboutinfo=1;
					windowsOpening=clamp(windowsOpening+1,0,5);
				}
				while((mouse.x>=30&&mouse.x<=180)&&(mouse.y>=380&&mouse.y<=404)&&(mouse.left_button==1))
				{
					mouse = get_mouse_position();
				}
			}
			else if((mouse.x>=30&&mouse.x<=180)&&(mouse.y>=356&&mouse.y<=380)&&(mouse.left_button==1||key=='\r'||key=='\n'))
			{
				if(vtermon==1)
				{
					vtermon=0;
					windowsOpening=clamp(windowsOpening-1,0,5);
				}
				else
				{
					vtermon=1;
					windowsOpening=clamp(windowsOpening+1,0,5);
				}
				while((mouse.x>=30&&mouse.x<=180)&&(mouse.y>=356&&mouse.y<=380)&&(mouse.left_button==1))
				{
					mouse = get_mouse_position();
				}
			}
			else if((mouse.x>=30&&mouse.x<=180)&&(mouse.y>=332&&mouse.y<=356)&&(mouse.left_button==1||key=='\r'||key=='\n'))
			{
				if(filemanon==1)
				{
					filemanon=0;
					currentprog=0;
					windowsOpening=clamp(windowsOpening-1,0,5);
				}
				else
				{
					filemanon=1;
					windowsOpening=clamp(windowsOpening+1,0,5);
					system("dir ROOTFS /b > SYSCFG\\file.idx");
					FILE* fin=fopen("SYSCFG\\file.idx","r");
					lines=1;
					if(fin==NULL)
					{
						continue;
					}
					while(fscanf(fin,"%s",files[lines])==1)
					{
						lines++;
						if(lines>14)
						{
							break;
						}
					}
					fclose(fin);
					system("del SYSCFG\\file.idx");
				}
				while((mouse.x>=30&&mouse.x<=180)&&(mouse.y>=332&&mouse.y<=356)&&(mouse.left_button==1))
				{
					mouse = get_mouse_position();
				}
			}
			else if((mouse.x>=30&&mouse.x<=180)&&(mouse.y>=308&&mouse.y<=332)&&(mouse.left_button==1||key=='\r'||key=='\n'))
			{
				if(demoon!=0)
				{
					demoon=0;
					windowsOpening=clamp(windowsOpening-1,0,5);
				}
				else
				{
					demoon=1;
					windowsOpening=clamp(windowsOpening+1,0,5);
				}
				while((mouse.x>=30&&mouse.x<=180)&&(mouse.y>=308&&mouse.y<=332)&&(mouse.left_button==1))
				{
					mouse = get_mouse_position();
				}
			}
			while(!kbhit()&&!mouse_changed());
		}

		if(vtermon==1&&powermenu==0&&aboutinfo==0&&tkrt%refRate==0)
		{
			draw_thick_rectangle(170,210,450,290,5,1,0);
			setcolor(2);
			line(170,240,450,240);
			settextstyle(TRIPLEX_FONT, HORIZ_DIR, 2);
			outtextxy(180,210,"[X]Error");
			render_colorful_pixelmap(error_icon,185,246,3);
			setcolor(2);
			outtextxy(213,240,"File not found");
			if(key==27)
			{
				vtermon=0;
			}
			if((mouse.x>=180&&mouse.x<=210)&&(mouse.y>=210&&mouse.y<=240)&&(mouse.left_button==1||key=='\r'||key=='\n'))
			{
				while((mouse.x>=180&&mouse.x<=210)&&(mouse.y>=210&&mouse.y<=240)&&(mouse.left_button==1))
				{
					mouse = get_mouse_position();
				}
				vtermon=0;
			}
			while(!kbhit()&&!mouse_changed());
		}

		if(powermenu==1&&aboutinfo==0&&tkrt%refRate==0)
		{
			draw_thick_rectangle(170,210,450,280,5,1,0);
			setcolor(2);
			line(170,240,450,240);
			settextstyle(TRIPLEX_FONT, HORIZ_DIR, 2);
			outtextxy(180,210,"Power Options");
			outtextxy(180,240,"S        |R     |E   |C      ");
			settextstyle(TRIPLEX_FONT, HORIZ_DIR,1);
			outtextxy(190,243,"hutdown  eboot  xit  ancel");
			if(key==27)
			{
				powermenu=0;
			}
			if((mouse.x>=180&&mouse.x<=210)&&(mouse.y>=240&&mouse.y<=280)&&(mouse.left_button==1||key=='\r'||key=='\n'))
			{
				while((mouse.x>=180&&mouse.x<=210)&&(mouse.y>=240&&mouse.y<=280)&&(mouse.left_button==1))
				{
					mouse = get_mouse_position();
				}
				system("shutdown s");
			}
			else if((mouse.x>=210&&mouse.x<=340)&&(mouse.y>=240&&mouse.y<=280)&&(mouse.left_button==1||key=='\r'||key=='\n'))
			{
				while((mouse.x>=210&&mouse.x<=340)&&(mouse.y>=240&&mouse.y<=280)&&(mouse.left_button==1))
				{
					mouse = get_mouse_position();
				}
				system("shutdown r");
			}
			else if((mouse.x>=340&&mouse.x<=380)&&(mouse.y>=240&&mouse.y<=280)&&(mouse.left_button==1||key=='\r'||key=='\n'))
			{
				while((mouse.x>=340&&mouse.x<=380)&&(mouse.y>=240&&mouse.y<=280)&&(mouse.left_button==1))
				{
					mouse = get_mouse_position();
				}
				goto end;
			}
			else if((mouse.x>=380&&mouse.x<=450)&&(mouse.y>=240&&mouse.y<=280)&&(mouse.left_button==1||key=='\r'||key=='\n'))
			{
				while((mouse.x>=380&&mouse.x<=450)&&(mouse.y>=240&&mouse.y<=280)&&(mouse.left_button==1))
				{
					mouse = get_mouse_position();
				}
				powermenu=0;
				windowsOpening=clamp(windowsOpening-1,0,5);
			}
			while(!kbhit()&&!mouse_changed());
		}

		if(aboutinfo==1&&tkrt%refRate==0)
		{
			draw_thick_rectangle(170,210,490,330,5,1,0);
			setcolor(2);
			line(170,240,490,240);
			settextstyle(TRIPLEX_FONT, HORIZ_DIR, 2);
			outtextxy(180,210,"[X]About AEOS");
			outtextxy(180,240,"AEOS v5.11.2 Build 8070");
			outtextxy(180,270,"AEOS BGIDM v1.11.0 Beta 7.1 DBR");
			outtextxy(190,300,"By Lithium4141");
			if(key==27)
			{
				aboutinfo=0;
			}
			if((mouse.x>=170&&mouse.x<=200)&&(mouse.y>=210&&mouse.y<=240)&&(mouse.left_button==1||key=='\r'||key=='\n'))
			{
				while((mouse.x>=170&&mouse.x<=200)&&(mouse.y>=210&&mouse.y<=240)&&(mouse.left_button==1))
				{
					mouse = get_mouse_position();
				}
				aboutinfo=0;
				windowsOpening=clamp(windowsOpening-1,0,5);
			}
			while(!kbhit()&&!mouse_changed());
		}

		if(debuginfo==1)
		{
			setcolor(2);
			settextstyle(TRIPLEX_FONT,HORIZ_DIR,2);
			outtextxy(10,10,"AEOS v5.11.2 Build 8070");
			char* buf;
			sprintf(buf,"X:%d Y:%d Left key:%s Right key:%s",mouse.x,mouse.y,(mouse.left_button==1?"ON":"OFF"),(mouse.right_button==1?"ON":"OFF"));
			outtextxy(10,27,buf);
			if(key==-1)
			{
				sprintf(buf,"Keyboard Character:  Keyboard ASCII: ");
			}
			else
			{
				sprintf(buf,"Keyboard Character:%c Keyboard ASCII:%d",(char)key,(int)key);
			}
			outtextxy(10,44,buf);
			sprintf(buf,"Maximum Refresh Rate:%d Render Tick Delay:%d",maxRefRate,refRate);
			outtextxy(10,61,buf);
			sprintf(buf,"Current Tick:%d",tkrt);
			outtextxy(10,78,buf);
		}

	draw_cursor:
		setcolor(2);
		render_pixel_map(mouse_cursor,mouse.x,mouse.y,2);

	swap_buffers:
        //setvisualpage(back_buffer);
        if (copy_mode) {
			copy_page(render_page, display_page);
		} else {
			setvisualpage(render_page);
			int temp = render_page;
			render_page = display_page;
			display_page = temp;
		}
	frame_sync:
		while(getkey()==-1&&!mouse_changed());
		maxRefRate=clamp(windowsOpening*20,20,100);
		delay(1000/maxRefRate);
	}

end:
	closegraph();
	return 0;
}
int main_lfr() {
	int gdriver = VGA, gmode = VGAHI;
	initgraph(&gdriver,&gmode,".\\BGI");
	int graphstatus=graphresult();
	if(graphstatus!=grOk) {
		closegraph();
		printf("BGI driver initilazation failed!Error:%s\n",grapherrormsg(graphstatus));
		return -1;
	}
    //setactivepage(1);
    //setvisualpage(1);
	def_color();
	mouse.x=320;
	mouse.y=240;
	int realMouse=0;
	if(mouse_init()!=0)
	{
		realMouse=1;
	}
	while(1)
	{
		tkrt++;
		if(tkrt%refRate==0)
		{
			cleardevice();
		}
		render_taskbar();
		if(realMouse==1 && keyboard_control == 0)
		{
			mouse=get_mouse_position();
		}
		if(tkrt>=16777216)
		{
			tkrt=0;
		}
		if(realMouse==1)
		{
			struct MOUSE_STATUS real_mouse = get_mouse_position();
			if(real_mouse.x != mouse.x || real_mouse.y != mouse.y)
			{
				keyboard_control = 0;
				mouse = real_mouse;
			}
		}
		key=getkey();
		if(key == 28)
		{
			goto end;
		}
		else if(key == 'A')
		{
			keyboard_control = 1;
			mouse.x=clamp(mouse.x-5,0,639);
		}
		else if(key == 'D')
		{
			keyboard_control = 1;
			mouse.x=clamp(mouse.x+5,0,639);
		}
		else if(key == 'S')
		{
			keyboard_control = 1;
			mouse.y=clamp(mouse.y+5,0,479);
		}
		else if(key == 'W')
		{
			keyboard_control = 1;
			mouse.y=clamp(mouse.y-5,0,479);
		}
		else if(key != -1)
		{
			keyboard_control = 1;
		}
		if((mouse.x>=5&&mouse.x<=53)&&(mouse.y>=457&&mouse.y<=477)&&(mouse.left_button==1||key=='\r'||key=='\n'))
		{
			if(startmenu==1)
			{
				startmenu=0;
				windowsOpening=clamp(windowsOpening-1,0,5);
			}
			else
			{
				startmenu=1;
				windowsOpening=clamp(windowsOpening-1,0,5);
			}
			while((mouse.x>=5&&mouse.x<=53)&&(mouse.y>=457&&mouse.y<=477)&&(mouse.left_button==1))
			{
				mouse = get_mouse_position();
			}
		}
        if(demoon!=0&&filemanon==0&&tkrt%refRate==0)
		{
			render_demo();
		}
        if(filemanon==1&&tkrt%refRate==0)
		{
			render_fileman();
		}
        if(startmenu==1&&tkrt%refRate==0)
		{
			render_startmenu();
		}
        if(vtermon==1&&powermenu==0&&aboutinfo==0&&tkrt%refRate==0)
		{
			render_vterm();
		}
        if(powermenu==1&&aboutinfo==0&&tkrt%refRate==0)
		{
			int ret=render_powermenu();
			if(ret==1)
			{
				goto end;
			}
		}
        if(aboutinfo==1&&tkrt%refRate==0)
		{
			render_aboutinfo();
		}
		if(debuginfo==1)
		{
			render_dbginfo();
		}
		setcolor(2);
		render_pixel_map(mouse_cursor,mouse.x,mouse.y,2);
		while(getkey()==-1&&!mouse_changed());
		maxRefRate=clamp(windowsOpening*20,20,100);
		delay(1000/maxRefRate);
	}
	end:
	closegraph();
	return 0;
}
int main_lfr_wm()
{

	int gdriver = VGA, gmode = VGAHI;
	initgraph(&gdriver,&gmode,".\\BGI");
	int graphstatus=graphresult();
	if(graphstatus!=grOk) {
		closegraph();
		printf("BGI driver initilazation failed!Error:%s\n",grapherrormsg(graphstatus));
		return -1;
	}
	//setactivepage(1);
	//setvisualpage(1);
	def_color();
	mouse.x=320;
	mouse.y=240;
	int realMouse=0;
	if(mouse_init()!=0)
	{
		realMouse=1;
	}
	int tkrt=0;
	while(1)
	{
		tkrt++;
		if(tkrt%refRate==0)
		{
			cleardevice();
		}
		if(realMouse==1 && keyboard_control == 0)
		{
			mouse=get_mouse_position();
		}
		if(tkrt>=16777216)
		{
			tkrt=0;
		}
		if(realMouse==1)
		{
			struct MOUSE_STATUS real_mouse = get_mouse_position();
			if(real_mouse.x != mouse.x || real_mouse.y != mouse.y)
			{
				keyboard_control = 0;
				mouse = real_mouse;
			}
		}
		key=getkey();
		if(key == 28)
		{
			goto end;
		}
		else if(key == 'A')
		{
			keyboard_control = 1;
			mouse.x=clamp(mouse.x-5,0,639);
		}
		else if(key == 'D')
		{
			keyboard_control = 1;
			mouse.x=clamp(mouse.x+5,0,639);
		}
		else if(key == 'S')
		{
			keyboard_control = 1;
			mouse.y=clamp(mouse.y+5,0,479);
		}
		else if(key == 'W')
		{
			keyboard_control = 1;
			mouse.y=clamp(mouse.y-5,0,479);
		}
		else if(key != -1)
		{
			keyboard_control = 1;
		}
		if(currentprog==0)
		{
			render_mainmenu();
		}
		if((mouse.x>=30&&mouse.x<=60)&&(mouse.y>=30&&mouse.y<=60)&&(mouse.left_button==1||key=='\r'||key=='\n'))
		{
			while((mouse.x>=30&&mouse.x<=60)&&(mouse.y>=30&&mouse.y<=60)&&(mouse.left_button==1))
			{
				mouse = get_mouse_position();
			}
			powermenu=1;
		}
		if((mouse.x>=35&&mouse.x<=150)&&(mouse.y>=60&&mouse.y<=130)&&(mouse.left_button==1||key=='\r'||key=='\n'))
		{
			while((mouse.x>=35&&mouse.x<=150)&&(mouse.y>=60&&mouse.y<=130)&&(mouse.left_button==1))
			{
				mouse = get_mouse_position();
			}
			currentprog=1;
			system("dir ROOTFS /b > SYSCFG\\file.idx");
			FILE* fin=fopen("SYSCFG\\file.idx","r");
			lines=1;
			if(fin!=NULL)
			{
				while(fscanf(fin,"%s",files[lines])==1)
				{
					lines++;
					if(lines>14)
					{
						break;
					}
				}
			}
			fclose(fin);
			system("del SYSCFG\\file.idx");
		}
		while(!kbhit()&&!mouse_changed());
		if((currentprog<0||demoon!=0)&&tkrt%refRate==0)
		{
			if(demoon>1)
			{
				currentprog=-1*demoon;
			}
			else
			{
				demoon=-1*currentprog;
			}
			render_demo();
		}
		else if((currentprog==1||filemanon==1)&&tkrt%refRate==0)
		{
			render_fileman();
		}
		else if((currentprog==1||(vtermon==1&&powermenu==0&&aboutinfo==0))&&tkrt%refRate==0)
		{
			render_vterm();
		}
		else if(powermenu==1&&tkrt%refRate==0)
		{
			int ret=render_exitconfirm();
			if(ret==1)
			{
				goto end;
			}
			else if(ret==0)
			{
				powermenu=0;
			}
		}
		else if((currentprog==3||aboutinfo==1)&&tkrt%refRate==0)
		{
			render_aboutinfo();
		}
		else if((currentprog==4||debuginfo==1))
		{
			render_dbginfo();
		}
		setcolor(2);
		render_pixel_map(mouse_cursor,mouse.x,mouse.y,2);
		while(getkey()==-1&&!mouse_changed());
		maxRefRate=clamp(windowsOpening*20,20,100);
		delay(1000/maxRefRate);
	}
	end:
	closegraph();
	return 0;
}
int main()
{
retry_again:
    FILE* fin=fopen("SYSCFG\\bgidm.cfg","r");
    int a1=1,b1=0;
    if(fin==NULL)
    {
        fclose(fin);
        printf("Error:Cannot open BGIDM Configuration file\n");
        printf("Resetting configuratioh to default...\n");
        system("echo 1 0 240 1> SYSCFG\\bgidm.cfg");
        printf("Retrying...\n");
        goto retry_again;
    }
    else
    {
        fscanf(fin,"%d %d %d %d",&a1,&b1,&maxRefRate,&refRate);
    }
    copy_mode=b1;
    if(a1==0)
    {
        return main_dbr();
    }
    else if(a1==1)
    {
        return main_lfr();
    }
    else if(a1==2)
	{
		return main_lfr_wm();
	}
    else
    {
        return -1;
    }
}
