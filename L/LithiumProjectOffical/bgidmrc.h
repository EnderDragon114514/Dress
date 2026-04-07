#ifndef BGIDM_RSC
#define BGIDM_RSC
#endif

void render_dbginfo()
{
	setcolor(2);
	settextstyle(TRIPLEX_FONT,HORIZ_DIR,2);
	outtextxy(10,10,"AEOS v5.11.2 Build 7611 Kernel Patch 1");
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
void render_aboutinfo()
{
	setcolor(1);
	draw_thick_rectangle(170,210,550,330,5,1,0);
	setcolor(2);
	line(170,240,550,240);
	settextstyle(TRIPLEX_FONT, HORIZ_DIR, 2);
	outtextxy(180,210,"[X]About AEOS");
	outtextxy(180,240,"AEOS v5.11.2 Build 7611 Patch 1");
	outtextxy(180,270,"AEOS BGIDM v1.10.3 Beta 6 LFR");
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
void render_powermenu()
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
        windowsOpening=clamp(windowsOpening-1,0,5);
        //system("del file.idx");
    }
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
            //system("del file.idx");
            system("dir ROOTFS /b > file.idx");
            FILE* fin=fopen("file.idx","r");
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
            system("del file.idx");
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
    line(,240,450,240);
    settextstyle(TRIPLEX_FONT, HORIZ_DIR, 2);
    outtextxy(30,30,"[X]AEOS v5.11.2 BGIDM Program Manager");
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
