/*
AEOS v5.11.2 Kernel and TTy
// This version is still a beta version(not stable),which means there will be some bugs,and it will be fixed in future updates
Bug report:send an email to lithium-offical@outlook.com or commit in github
*/

/*
Future update implentions:
1. RbVM graphics supports
2. Pagefile(similar to Linux swapfile or swap parition and Windows pagefile)
3. Hibernate(Requires pagefile)
*/

//#define 51kern_agsh
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <conio.h>
#include <dos.h>
extern "C"{
	#include "A:\rbvm.h"
}
//FILE* fin[maxProg];
unsigned long get_disk_total_size(char drive) {
	//the asm code do not work and crashes the whole operating system
	//so i disabled it
	/*union REGS regs;
	regs.h.ah = 0x36;
	regs.h.dl = drive;
	int86(0x21, &regs, &regs);
	if(regs.x.ax == 0xFFFF) {
		return 0;
	}	unsigned long total_sectors = regs.x.dx * regs.x.ax;
	unsigned long total_bytes = total_sectors * regs.x.cx;
		return total_bytes;
	*/
	//a fixed standard floppy size
	return 1509950;
}
float get_total_mem_mb()
{
	//i tried but failed to get xms memory,and basic mamory is a fixed value,so directly returns the basic memory
    return 1.058;
}
float get_total_disk_mb()
{
	return get_disk_total_size(0)/1024/1024;
}int get_cpu_level() {
	//The origional code have some issue,so i choosed to disable it
	/*unsigned short flags1, flags2;
	asm {
        pushf
        pop ax
        mov flags1, ax
	}
    asm {
        mov ax, flags1
        xor ax, 0xF000
        push ax
        popf
        pushf
        pop ax
        mov flags2, ax
	}
    if ((flags1 ^ flags2) & 0xF000) {
		return 1;
	}
	return 0;*/
	return 1;
}

int SN_PORT=1,AGSH_STYLE=1;
int txspd=0;
void sernet_init(int port, int baud) {
	int divisor = 115200 / baud;
	outportb(port + 3, 0x80);
	outportb(port, divisor & 0xFF);
	outportb(port + 1, divisor >> 8);
	outportb(port + 3, 0x03);
	outportb(port + 4, 0x00);
	outportb(port + 1, 0x01);
}
int sernet_send_byte(int port, unsigned char data) {
	outportb(port, data);
	delay(1);
}
char sernet_recv_byte(int port) {
	if(inportb(port + 5) & 0x01) {
		return inportb(port);
	}
	return ERR_TIMEOUT;
}
int sernet_wtf(int port,char* url,char* filename)//wtf is write to file,not what the f**k
{
	FILE* fout=fopen(filename,"w");
	if(!fout)
	{
		return ERR_DISK_IO;
	}
    unsigned long cnt=0;
	sernet_send_byte(port,NET_SEG_INIT);
	while(sernet_recv_byte(port)!=NET_SEG_INIT)
	{
		cnt++;
        if(cnt%MAX_WAIT_TICK==0)
		{
            printf("\rWarning:Network card is not responding after %i seconds!Is the network card responding?Press Ctrl-\\ to forcely terminate this process.\n",cnt/MAX_WAIT_TICK);
		}
		if(getkey()==28)
		{
			return SYS_KILL;
		}
	}
	char buf[64];
resend_url:
	cnt=0;
	sprintf(buf,"GET:%s",url);
	for(int i=0;i<strlen(buf);i++)
	{
		sernet_send_byte(port,buf[i]);
	}
	while(1)
	{
		cnt++;
        if(cnt%MAX_WAIT_TICK==0)
		{
            printf("\rWarning:Network card is not responding after %i seconds!Is the network card responding?Press Ctrl-\\ to forcely terminate this process.\n",cnt/MAX_WAIT_TICK);
		}
		if(getkey()==28)
		{
			fclose(fout);
			return SYS_KILL;
		}
		int ret=sernet_recv_byte(port);
		if(ret==NET_SEG_ACK)
		{
			break;
		}
		else if(ret==NET_SEG_NAK)
		{
			goto resend_url;
		}
	}
	char buf1[64];
	memset(buf,0,sizeof(buf));
	memset(buf1,0,sizeof(buf));
	int sum=0,datacnt=0,datacnt_tr=0;
	cnt=0;
	while(1)
	{
		cnt++;
		datacnt++;
		if(getkey()==28)
		{
			fclose(fout);
			return SYS_KILL;
		}
		int ret=sernet_recv_byte(port);
		if(ret==NET_SEG_ETX)
		{
			fprintf(fout,"%s",buf);
			break;
		}
		else if(ret==ERR_NET_FAIL)
		{
			fclose(fout);
			return ERR_NET_FAIL;
		}
        memcpy(buf1,buf,sizeof(buf));
		sprintf(buf,"%s%c",buf1,ret);
		sum+=ret;
		if(cnt%RX_BUFFER_SIZE==0)
		{
			char sum_buf[10];
			sprintf(sum_buf,"%d",sum);
            unsigned long cnt1=0;
			for(int i=0;i<strlen(sum_buf);i++)
			{
				if(getkey()==28)
				{
					fclose(fout);
					return SYS_KILL;
				}
				sernet_send_byte(port,sum_buf[i]);
			}
			while(1)
			{
                char ret=sernet_recv_byte(port);
				cnt1++;
                if(cnt1%MAX_WAIT_TICK==0)
				{
                    printf("\rWarning:Network card is not responding after %i seconds!Is the network card responding?Press Ctrl-\\ to forcely terminate this process.\n",cnt1/MAX_WAIT_TICK);
				}
				if(getkey()==28)
				{
					fclose(fout);
					return SYS_KILL;
				}
				if(ret==NET_SEG_ACK)
				{
					fprintf(fout,"%s",buf);
					cnt=0;
					datacnt_tr=datacnt;
					sernet_send_byte(port,NET_SEG_ACK);
					memset(buf,0,sizeof(buf));
					memset(buf1,0,sizeof(buf1));
					break;
				}
				else if(ret==NET_SEG_NAK)
				{
					memset(buf,0,sizeof(buf));
					memset(buf1,0,sizeof(buf1));
					cnt=0;
					datacnt=datacnt_tr;
                    sernet_send_byte(port,NET_SEG_NAK);
					break;
				}
			}
			txspd=datacnt/256;
			printf("\rCurrent speed:%dByte/frame",txspd);
		}
	}
	printf("\nTransmittion Finish!\n");
	fclose(fout);
	return 0;
}


int main()
{
	system("vga.exe");
    FILE* verifyPkg=fopen("SYSCFG\\pkg.idx","r");
	if(!verifyPkg)
	{
		system("echo aeos-system-kernel >> SYSCFG\\pkg.idx");
		system("echo bgidm >> SYSCFG\\pkg.idx");
		system("echo agsh >> SYSCFG\\pkg.idx");
		system("echo infdrv >> SYSCFG\\pkg.idx");
		system("echo opensrv >> SYSCFG\\pkg.idx");
		system("echo bgidm-dos-video-driver >> SYSCFG\\pkg.idx");
		system("echo cutemouse-dos-mouse-driver >> SYSCFG\\pkg.idx");
        system("echo msdos-system-initilazer >> SYSCFG\\pkg.idx");
	}
    else
    {
        fclose(verifyPkg);
    }
retry_getagshcfg:
	FILE* _fc=fopen("SYSCFG\\agsh.cfg","r");
	if(!_fc)
	{
		printf("[AGeTTy:FATAL]Unable to read shell configuration file:Disk I/O Error\n");
		printf("              Resetting to default configuration...\n");
		char* __buf;
        //*debug*/system("pause");
		system("echo 1 1 > SYSCFG\\agsh.cfg");
		free(__buf);
		goto retry_getagshcfg;
	}
    fscanf(_fc,"%d %d",&SN_PORT,&AGSH_STYLE);
	if(SN_PORT==1)
	{
		SN_PORT=COM1;
	}
	else if(SN_PORT==2)
	{
		SN_PORT=COM2;
	}
	else if(SN_PORT==3)
	{
		SN_PORT=COM3;
	}
	else if(SN_PORT==4)
	{
		SN_PORT=COM4;
	}
	else
	{
		SN_PORT=COM1;
	}
	fclose(_fc);
    system("mkdir ROOTFS > NUL");
    system("mkdir SYSCFG > NUL");
    system("cls");
    printf("      A         EEEEEEEE     OOOOO            SSSS   555555       11      11  \n");
    printf("     A A        .           O     O          S       5          ..11    ..11  \n");
    printf("    A   A       .          O       O        S        5            11      11  \n");
    printf("   A.....A      EEEEEEEE   O       O        S        555555       11      11  \n");
    printf("  A       A     .          O       O        S             5       11      11  \n");
    printf(" A         A    .           O     O        S              5  OO   11      11  \n");
    printf("A           A   EEEEEEEE     OOOOO     SSSS          555555  OO ..11..  ..11..\n");
    printf("\n");
    printf("||===\\\\                       ||      //||      \n");
    printf("||   ||            ||         ||        ||       \n");
    printf("||===//  //==\\\\   ====  //==  ||==\\\\    ||                    _\n");
    printf("||       ||  ||    ||   ||    ||   ||   ||             |_|   | |\n");
    printf("||       \\\\==\\\\     \\\\  \\\\==  ||   || ======       \\/    | . |_|\n");
    printf("\n");
    printf("AEOS v5.11.2 Beta 7.1 Build 8070 Kernel Patch 4 Community Edition\n");
    printf("Starting up...[                                               ]");
    printf("\rStarting up...[");
    for(int i=1;i<=47;i++)
	{
		sysClockTick++;
        printf("#");
        if(i>=12&&i<=31)
        {
            delay(155);
        }
        if(i==20)
		{
			system("ctmouse.exe > NUL");
		}
        if(i==8)
        {
            sernet_init(SN_PORT,115200);
        }   
        if(i<30)
        {
            delay(40);
        }
        delay(25);
    }
    delay(300);
    system("cls");
    printf("Initilazing AEOS v5.11\n");
	printf("OpenSRV is intilazing...\n");
	system("mode con cols=80 lines=25");
	printf("[ OK ] TTyDM Initilazation Success!\n");
    printf("[ OK ] SCM Initilazation Success!\n");
	printf("[ OK ] DMS Intilazation Success!\n");
	getkey();
	/*
	 *	   for(int i=1;i<=5;i++)
	 *	   {
	 *			   printf("\r[*   ] Initilazing BGIDM...");
	 *			   getkey();
	 *			   delay(500);
	 *			   printf("\r[#*  ] Initilazing BGIDM...");
	 *			   getkey();
	 *			   delay(500);
	 *			   printf("\r[*#* ] Initilazing BGIDM...");
	 *			   getkey();
	 *			   delay(500);
	 *			   printf("\r[ *#*] Initilazing BGIDM...");
	 *			   getkey();
	 *			   delay(500);
	 *			   printf("\r[  *#] Initilazing BGIDM...");
	 *			   getkey();
	 *			   delay(500);
	 *			   printf("\r[   *] Initilazing BGIDM...");
	 *			   getkey();
	 *			   delay(500);
     *             printf("\r[    ] Initilazing BGIDM...");
	 *			   getkey();
	 *			   delay(500);
}
*/
	printf("[ OK ] BGIDM Initilazation Success!\n");
	printf("[ OK ] ARM Initilazation Success!\n");
	printf("[ OK ] AGeTTy Initilazation Success!\n");
	printf("[ OK ] InfDrv Service Started!\n");
	getkey();
	//delay(2000);
	getkey();
	printf("[ OK ] CuteMouse v1.6 DOS Initilazation Success!\n");
	printf("[FAIL] No Sound card driver found!\n");
    //sernet_init(SN_PORT,115200);
	printf("[ OK ] Network card driver Initilazation Success!\n");
	getkey();
    //system("mkdir ROOTFS > NUL");
    //system("mkdir SYSCFG > NUL");
	printf("[ OK ] RTFS Driver Initilazed Successfully!\n");
    printf("[ OK ] Mounted /SYSCFG\n");
    printf("[ OK ] Mounted /ROOTFS\n");
shell:
	printf("Welcome to AEOS 5.11!\n");
retry_getpwd:
    FILE* _usrcfg=fopen("SYSCFG\\passwd.cfg","r");
	char plen=0;
    char pswd[65];
	if(!_usrcfg)
	{
        printf("[AGeTTy:FATAL]Unable to read user configuration file:Disk I/O Error\n");
		printf("               Resetting to default configuration...\n");
		char* __buf;
        sprintf(__buf,"echo %c >SYSCFG\\passwd.cfg",1);
		system(__buf);
		free(__buf);
		goto retry_getpwd;
	}
	fscanf(_usrcfg,"%c",&plen);
	plen--;
	if(plen>0)
	{
		for(int i=0;i<((char)plen);i++)
		{
            fscanf(_usrcfg,"%c",&pswd[i]);
			pswd[i]+=31;
		}
	}
	fclose(_usrcfg);
    //printf("%d : %s",plen,pswd);
    //*debug_agshstyle*/printf("%d %d ",SN_PORT,AGSH_STYLE);
	while(1)
	{
		sysClockTick++;
		printf("AEOS51 login:");
		int key=0,currentChr=0;
		char inputBuf[65];
		memset(inputBuf,0,sizeof(inputBuf));
		while(1)
		{
			sysClockTick++;
			key=getch();
			if(key==8||key==127)
			{
				currentChr--;
				if(currentChr<0)
				{
					currentChr=0;
				}
				else
				{
					printf("\b \b");
					inputBuf[currentChr+1]=0;
				}
            }
			if(key>=32&&key<=126)
			{
				if(currentChr>=64)
				{
					currentChr=63;
					break;
				}
				inputBuf[currentChr++]=key;
				printf("%c",key);
			}
			if(key==3||key==9||key==10||key=='\r'||key=='\n')
			{
				inputBuf[currentChr]=0;
				break;
			}
		}
        if(strcmp(inputBuf,"root")==0&&plen<=0)
        {            
            printf("\n[AGeTTy:Warning]Password not set!Use \"passwd\" to set a new password!\n");
            goto loginok;
        }
        printf("\nPassword:");
        key=0,currentChr=0;
		char inputBuf1[65];
		memset(inputBuf1,0,sizeof(inputBuf1));
		while(1)
		{
			sysClockTick++;
			key=getch();
			if(key==8||key==127)
			{
				currentChr--;
				if(currentChr<0)
				{
					currentChr=0;
				}
				else
				{
					//printf("\b \b"); //no need for this line
					inputBuf1[currentChr+1]=0;
				}

			}
			if(key>=32&&key<=126)
			{
				if(currentChr>=64)
				{
					currentChr=63;
					break;
				}
				inputBuf1[currentChr++]=key;
				//printf("%c",key); //passwords should not be shown
			}
			if(key==3||key==9||key==10||key=='\r'||key=='\n')
			{
				inputBuf1[currentChr]=0;
				break;
			}
		}
		if(strcmp(inputBuf,"root")==0 && strcmp(inputBuf1,pswd)==0)
		{
			goto loginok;
		}
		printf("\nLogin incorrect\n");
	}
loginok:
	system("bgidm.exe");
    printf("\n");
	while(1)
	{
	agsh:
		sysClockTick++;
		if(AGSH_STYLE==1)
		{
            printf("root@AEOS51:ROOTFS/ # ");
		}
		else if(AGSH_STYLE==2)
		{
            printf("\x1B[47mroot\x1B[40m>ROOTFS/> ");
		}
		else if(AGSH_STYLE==3)
		{
            printf("ROOTFS/ # ");
		}
		else if(AGSH_STYLE==4)
		{
            printf("ROOTFS:\\>");
		}
		else if(AGSH_STYLE==5)
		{
			printf("----(root@AEOS51)-[ROOTFS/]\n");
			printf("|--# ");
		}
		else
		{
			printf("root@AEOS51:ROOTFS/ # ");
		}
		int key=0,currentChr=0;
		char inputBuf[65];
		memset(inputBuf,0,sizeof(inputBuf));
		while(1)
		{
			sysClockTick++;
			key=getch();
			if(key==8||key==127)
			{
				currentChr--;
				if(currentChr<0)
				{
					currentChr=0;
				}
				else
				{
					printf("\b \b");
					inputBuf[currentChr+1]=0;
				}

			}
			if(key>=32&&key<=126)
			{
				if(currentChr>=64)
				{
					currentChr=63;
					break;
				}
				inputBuf[currentChr++]=key;
				printf("%c",key);
			}
			if(key==3||key==9||key==10||key=='\r'||key=='\n')
			{
				inputBuf[currentChr]=0;
				break;
			}
		}
		printf("\n");
		if(strcmp(inputBuf,"exit")==0)
		{
			system("cls");
			goto shell;
		}
		else if(strcmp(inputBuf,"ls")==0)
		{
            system("dir ROOTFS /b");
		}
		else if(strcmp(inputBuf,"ls -l")==0)
		{
            system("dir ROOTFS /p /b");
		}
		else if(strlen(inputBuf)>=6&&strncmp(inputBuf,"echo ",5)==0)
		{
			char* others=inputBuf+5;
			printf("%s\n",others);
		}
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
		else if(strcmp(inputBuf,"panic")==0)
		{
			printf("User triggered kernel panic\n");
			return -1;
		}
		else if(strcmp(inputBuf,"help")==0)
		{
			printf("Built in commands:\n");
			printf("exit       ls       echo       exec\n");
			printf("clear      shutdown poweroff   restart\n");
			printf("reboot     pwd      ver        ps\n");
			printf("help       kill     open       bgidm\n");
			printf("passwd     whoami   lsusr      arm\n");
            printf("syscfg     cdufetch cat        panic\n");
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
		}
		else if(strlen(inputBuf)>=6&&strncmp(inputBuf,"open ",5)==0)
		{
			int pid=atoi(inputBuf+5);
			if(pid<0||pid>=progs)
			{
				printf("Invalid PID:%d\n",pid);
				continue;
			}
			printf("Openning %d:%s\n",pid,fileloc[pid]);
			rbvm(pid);
		}
        else if(strcmp(inputBuf,"passwd")==0)
		{
            FILE* _file=fopen("SYSCFG\\passwd.cfg","w");
			if(!_file)
			{
				printf("Failed to open user database file:Disk I/O Error\n");
			}
			else
			{
                char inputBuf1[65],inputBuf2[65];
                memset(inputBuf1,0,sizeof(inputBuf1));
                memset(inputBuf2,0,sizeof(inputBuf2));    
				while(1)
				{
					printf("Input new password:");
					int key=0,currentChr=0;
                    while(1)         
					{
						key=getch();
						if(key==8||key==127)
						{
							currentChr--;
							if(currentChr<0)
							{
								currentChr=0;
							}
							else
							{
								//printf("\b \b"); //no need for this line
								inputBuf1[currentChr+1]=0;
							}
						}
						if(key>=32&&key<=126)
						{
							if(currentChr>=64)
							{
								currentChr=63;
								break;
							}
							inputBuf1[currentChr++]=key;
							//printf("%c",key); //passwords should not be shown
						}
						if(key==3||key==9||key==10||key=='\r'||key=='\n')
						{
							inputBuf1[currentChr]=0;
							break;
						}
					}
					printf("\nRetype new password:");
                    int key1=0,currentChr1=0;
					while(1)
					{
                        key1=getch();
                        if(key1==8||key1==127)
						{
                            currentChr1--;
                            if(currentChr1<0)
							{
                                currentChr1=0;
							}
							else
							{
								//printf("\b \b"); //no need for this line
                                inputBuf2[currentChr1+1]=0;
							}
						}
                        if(key1==28)
						{
							printf("\"passwd\" forcely terminated by Ctrl-\\\n");
							goto agsh;
						}
                        if(key1>=32&&key1<=126)
						{
                            if(currentChr1>=64)
							{
                                currentChr1=63;
								break;
							}
                            inputBuf2[currentChr1++]=key1;
                            //printf("%c",key1); //passwords should not be shown
						}
                        if(key1==3||key1==9||key1==10||key1=='\r'||key1=='\n')
						{
                            inputBuf2[currentChr1]=0;
							break;
						}
					}
					if(strcmp(inputBuf1,inputBuf2)==0)
					{
						break;
					}
					else
					{
						printf("\nInput disagree\n");
					}
				}
                fclose(_file);
				char* __buf;
				if(strpbrk(inputBuf1, "|&;<>`$\"\'\\/!*?[]{}()~") != NULL) {
					printf("Invalid syntax!\n");
                    printf("\n[+1]%s terminated by SIGINOS(Invalid Operation Syntax)",inputBuf);
					goto agsh;
				}
				for(int i=0;i<strlen(inputBuf1);i++)
                {            
                    inputBuf1[i]-=31;
				}
                sprintf(__buf,"echo %c%s > SYSCFG\\passwd.cfg",((char)strlen(inputBuf1)+1),inputBuf1);
                //printf("%s\n",__buf);
                printf("Reboot to take effects.\n");
                system(__buf);
                //fclose(_file);
			}
		}
		else if(strlen(inputBuf)>=4&&strncmp(inputBuf,"cat ",4)==0){
			char* others=inputBuf+4;
			char* execBuf;
			if(strpbrk(others, "|&;<>`$\"\'\\/!*?[]{}()~") != NULL) {
				printf("Invalid syntax!\n");
				printf("\n[+1]%s terminated by SIGINOS(Invalid Operation Syntax)",inputBuf);
				goto agsh;
			}
			sprintf(execBuf,"type ROOTFS\\%s",others);
			system(execBuf);
		}
		else if(strcmp(inputBuf,"whoami")==0)
		{
			printf("root\n");
		}
		else if(strcmp(inputBuf,"lsusr")==0)
		{
			printf("Users   |UID\n");
			printf("51INIT  |0\n");
			printf("OPENSRV |1\n");
			printf("root    |2\n");
		}
		else if(strlen(inputBuf)>=4&&strncmp(inputBuf,"arm ",4)==0)
		{
			char* others=inputBuf+4;
			//printf("%s\n",others);
			if(strncmp(others,"install ",8)==0)
			{
				char* otherz=others+8;
				printf("Reading package database... Done\n");
				printf("Searching package in database... ");
				char* tmpbuf;
				if(strpbrk(otherz, "|&;<>`$\"\'\\/!*?[]{}()~") != NULL) {
					printf("Invalid syntax!\n");
                    printf("\n%s terminated by SIGINOS(Invalid Operation Syntax)",inputBuf);
					goto agsh;
				}
				sprintf(tmpbuf,"type SYSCFG\\repo.idx|findstr %s",otherz);
				if(system(tmpbuf)!=0)
				{
					printf("Error:Package not found in package database\n");
					printf("Perhaps updating repository tree?\n");
					free(tmpbuf);
					goto agsh;
				}
				free(tmpbuf);
				printf("Downloading package from\"http://lithiumproj.ink/arm-repo/%s\"...\n",otherz);
				char* urlbuf;
				char* locbuf;
				if(strcmp(otherz,"aeos-system-kernel")==0)
				{
					sprintf(locbuf,"51kern.exe");
					system("copy 51kern.exe 51kern.bak");
					sprintf(urlbuf,"http://lithiumproj.ink/arm-repo/51kern.exe");
				}
				else if(strcmp(otherz,"bgidm")==0)
				{
					sprintf(locbuf,"bgidm.exe");
					system("copy bgidm.exe bgidm.bak");
					sprintf(urlbuf,"http://lithiumproj.ink/arm-repo/bgidm.exe");
				}
				else
				{
					sprintf(locbuf,"ROOTFS\\%s.rba",otherz);
					sprintf(urlbuf,"http://lithiumproj.ink/arm-repo/%s.rba",otherz);
				}
				int ret=sernet_wtf(SN_PORT,urlbuf,locbuf);
				free(urlbuf);
				free(locbuf);
				if(ret==ERR_DISK_IO)
				{
					printf("Error:Disk I/O Error\n");
					goto agsh;
				}
				if(ret==SYS_KILL)
				{
					printf("\"%s\" terminated by Ctrl-\\\n",inputBuf);
					goto agsh;
				}
				if(ret==ERR_NET_FAIL)
				{
					printf("Error:Network failure\n");
					goto agsh;
				}
				if(ret==ERR_TIMEOUT)
				{
					printf("Error:Timeout\n");
					goto agsh;
				}
				char* idxbuf;
				sprintf(idxbuf,"type SYSCFG\\pkg.idx|findstr %s",otherz);
				if(system(idxbuf)!=0)
				{
                    sprintf(idxbuf,"echo %s >> pkg.idx",otherz);
                    system(idxbuf);
                }
            }
			else if(strncmp(others,"update-repo",11)==0)
			{
				printf("Downloading repository index from\"http://lithiumproj.ink/arm-repo/repo.idx\"...\n");
				int ret=sernet_wtf(SN_PORT,"http://lithiumproj.ink/arm-repo/repo.idx","SYSCFG\\repo.idx");
				if(ret==ERR_DISK_IO)
				{
					printf("Error:Disk I/O Error\n");
					goto agsh;
				}
				if(ret==SYS_KILL)
				{
					printf("\"arm update-repo\" terminated by Ctrl-\\\n");
					goto agsh;
				}
				if(ret==ERR_NET_FAIL)
				{
					printf("Error:Network failure\n");
					system("type NUL > SYSCFG\\repo.idx");
				}
				if(ret==ERR_TIMEOUT)
				{
					printf("Error:Timeout\n");
					system("type NUL > SYSCFG\\repo.idx");
				}
				printf("Building local package databse....\n");
				system("echo aeos-system-kernel >> SYSCFG\\repo.idx");
				system("echo bgidm >> SYSCFG\\repo.idx");
				system("echo agsh >> SYSCFG\\repo.idx");
				system("echo infdrv >> SYSCFG\\repo.idx");
				system("echo opensrv >> SYSCFG\\repo.idx");
				system("echo bgidm-dos-video-driver >> SYSCFG\\repo.idx");
				system("echo cutemouse-dos-mouse-driver >> SYSCFG\\repo.idx");
                system("echo msdos-system-initilazer >> SYSCFG\\repo.idx");
				printf("Success!\n");
			}
			else if(strncmp(others,"update",6)==0)
			{
				printf("Reading package database... Done\n");
				printf("Getting version information from databse... Done\n");
				printf("Error:all packages up to date\n");
			}
			else if(strncmp(others,"remove ",7)==0)
			{
				printf("Reading package database... Done\n");
				char* otherz=others+7;
				//printf("%s",otherz);
				if(strpbrk(otherz, "|&;<>`$\"\'\\/!*?[]{}()~") != NULL) {
					printf("Invalid syntax!\n");
                    printf("\n[+1]%s terminated by SIGINOS(Invalid Operation Syntax)",inputBuf);
					goto agsh;
				}
				if(strcmp(otherz,"aeos-system-kernel")==0)
				{
					 printf("Warning:This is your operating system kernel.Removing it can lead your operating system unable to boot!Confirm operation?[y/n]\n");
					 int k=getch();
					 if(k==28)
					 {
						 printf("\"arm remove aeos-system-kernel\" Terminated by Ctrl-\\\n");
						 goto agsh;
					 }
					 else if(k=='y'||k=='Y')
					 {
						 printf("Removing package binaries...\n");
						 int ret=system("del 51kern.exe");
						 if(ret==0)
						 {
							 printf("Success!\n");
						 }
						 else
						 {
							 printf("Error:Failed to remove package binaries.\n");
							 printf("Error code:%d",ret);
                         }
					 }
					 else
					 {
						 printf("Aborted.\n");
						 goto agsh;
					 }
				}
                else if(strcmp(otherz,"rbvm-runtime")==0||strcmp(otherz,"bgidm")==0||strcmp(otherz,"agsh")==0||strcmp(otherz,"infdrv")==0||strcmp(otherz,"opensrv")==0||strcmp(otherz,"cutemouse-dos-mouse-driver")==0||strcmp(otherz,"bgidm-dos-video-driver")==0)
				{
                    printf("Warning:This package is not an independent package but a dependency and requirement of \"aeos-system-kernel\".\n");
                    printf("Error:Package not found!\n");
				}
                else if(strcmp(otherz,"msdos-system-initilazer")==0)
				{
					printf("Warning:This is your operating system kernel.Removing it can lead your operating system unable to boot!Confirm operation?[y/n]\n");
					int k=getch();
					if(k==28)
					{
						printf("\"arm remove msdos-system-initilazer\" Terminated by Ctrl-\\\n");
						goto agsh;
					}
					else if(k=='y'||k=='Y')
					{
						printf("Removing package binaries...\n");
						int ret=system("del command.exe io.sys msdos.sys");
						if(ret==0)
						{
							printf("Success!\n");
						}
						else
						{
							printf("Error:Failed to remove package binaries.\n");
							printf("Error code:%d",ret);
						}
					}
					else
					{
						printf("Aborted.\n");
						goto agsh;
					}
                }
				else
				{
					char* idxbuf;
					sprintf(idxbuf,"type SYSCFG\\pkg.idx|findstr %s",otherz);
					if(system(idxbuf)!=0)
					{
                        sprintf(idxbuf,"findstr /V \"%s\" pkg.idx > SYSCFG\\pkg.idx",otherz);
					}
					else
					{
						printf("Error:package not found!\n");
					}
				}
			}
			else if(strncmp(others,"clean-cache",11)==0)
			{
				printf("Reading package database... Done\n");
				printf("Scanning unused package...  Done\n");
				printf("Scanning cache files...     Done\n");
				printf("Successs!\n");
			}
			else if(strncmp(others,"list",4)==0)
			{
				system("type SYSCFG\\pkg.idx");
			}
			else
			{
				printf("Error:Invalid parameter\n");
				printf("Usage:arm [install/update/update-repo/remove/clean-cache/list] <package-name> \n");
			}
		}
		else if(strlen(inputBuf)>=7&&strncmp(inputBuf,"syscfg ",7)==0)
		{
			char* others=inputBuf+7;
			if(strcmp(others,"bgidm")==0)
			{
                int a=1,b=0,c=60,d=1;
                printf("Select Rendering mode:1. Legacy frame rendering  2. Double-buffered rendering");
				while(1)
				{
					int k=getch();
					if(k=='1')
					{
						a=1;
						break;
					}
					else if(k=='2')
					{
						a=0;
						break;
					}
					else if(k==28)
					{
						printf("\"syscfg bgidm\" terminated by Ctrl-\\\n");
						goto agsh;
					}
				}
				if(a==0)
				{
                    printf("\nCopy mode for double-buffered rendering?[y/n]");
					while(1)
					{
						int k=getch();
						if(k=='y'||k=='Y')
						{
							b=1;
							break;
						}
						else if(k=='n'||k=='N')
						{
							b=0;
							break;
						}
						else if(k==28)
						{
							printf("\"syscfg bgidm\" terminated by Ctrl-\\\n");
							goto agsh;
						}
					}
				}
				if(a==1)
				{
					printf("\nUse Experimental Window Manager?[y/n]");
					while(1)
					{
						int k=getch();
						if(k=='y'||k=='Y')
						{
							a=2;
							break;
						}
						else if(k=='n'||k=='N')
						{
							a=1;
							break;
						}
						else if(k==28)
						{
							printf("\"syscfg bgidm\" terminated by Ctrl-\\\n");
							goto agsh;
						}
					}
				}
            redo1:
                printf("\nMaximum Render FPS(Minimum 1):");
				c=get_uint();
				if(c==-2)
				{
					printf("\"syscfg bgidm\" terminated by Ctrl-\\\n");
					goto agsh;
				}
                if(c==0)
                {
                    printf("Error:Invalid configuration\n");
                    goto redo1;
                }
			redo:
				printf("\nRender tick delay(Minimum 1):");
				d=get_uint();
				if(c==0)
				{
					printf("Error:Invalid configuration\n");
					goto redo;
				}
				if(c==-2)
				{
					printf("\"syscfg bgidm\" terminated by Ctrl-\\\n");
					goto agsh;
				}
				char* buf;
				sprintf(buf,"echo %d %d %d %d > SYSCFG\\bgidm.cfg",a,b,c,d);
                system(buf);
				free(buf);
                printf("\nOperation done!Reboot to take effects.\n");
			}
			else if(strcmp(others,"agetty")==0)
			{
				printf("Note:password can only modified by \"passwd\"\n");
			}
			else if(strcmp(others,"agsh")==0)
			{
                printf("Select Network serial port:1. COM1   2. COM2   3. COM3  4. COM4\n");
				int serialmode=1;
				while(1)
				{
					int k=getch();
					if(k=='1')
					{
						serialmode=1;
						break;
					}
					else if(k=='2')
					{
						serialmode=2;
						break;
					}
					else if(k=='3')
					{
						serialmode=3;
						break;
					}
					else if(k=='4')
					{
                        serialmode=4;
						break;
					}
					else if(k==28)
					{
						printf("\"syscfg agsh\" terminated by Ctrl-\\\n");
						goto agsh;
					}
				}
				printf("Select a shell style:\n");
				printf("1. Default:   root@AEOS51:ROOTFS/ # \n");
				printf("2. Posh:      \x1B[47mroot\x1B[40m>ROOTFS/> \n");
                printf("3. Linux SH:  ROOTFS/ # \n");
                printf("4. DOS:       ROOTFS:\\>\n");
				printf("5. Kali:      ----(root@AEOS51)-[ROOTFS/]\n");
                printf("              |--# \n");
				int shellstyle=1;
				while(1)
				{
					int k=getch();
					if(k=='1')
					{
						shellstyle=1;
						break;
					}
					else if(k=='2')
					{
						shellstyle=2;
						break;
					}
					else if(k=='3')
					{
						shellstyle=3;
						break;
					}
					else if(k=='4')
					{
                        shellstyle=4;
						break;
					}
					else if(k=='5')
					{
						shellstyle=5;
						break;
					}
					else if(k==28)
					{
						printf("\"syscfg agsh\" terminated by Ctrl-\\\n");
						goto agsh;
					}
				}
				char* tmp1;
				sprintf(tmp1,"echo %d %d > SYSCFG\\agsh.cfg",serialmode,shellstyle);
				system(tmp1);
				free(tmp1);
				printf("Operation done!Reboot to take effects.\n");
			}
			else
			{
				printf("Usage:syscfg [bgidm/agetty/agsh]\n");
			}
		}
		else if(strcmp(inputBuf,"bgidm")==0)
		{
			system("bgidm.exe");
		}
		else if(strcmp(inputBuf,"cdufetch")==0)
		{
			float _m=get_total_mem_mb();
            float _d=get_total_disk_mb();
            char* cpuType;
            if(get_cpu_level()==1)
            {
                strcpy(cpuType,"Intel IA-32 i386+");
            }
            else
            {
                strcpy(cpuType,"Intel 8086 Series");
            }
            char* netIf;
            if(SN_PORT==COM1)
            {
                strcpy(netIf,"COM1");
            }
            else if(SN_PORT==COM2)
            {
                strcpy(netIf,"COM2");
            }
            else if(SN_PORT==COM3)
            {
                strcpy(netIf,"COM3");
            }
            else if(SN_PORT==COM4)
            {
                strcpy(netIf,"COM4");
            }
            else
            {
                strcpy(netIf,"Unknown Serial port");
            }
            char* themeType;
            if(AGSH_STYLE==1)
            {
                strcpy(themeType,"AGSH");
            }
            else if(AGSH_STYLE==2)
            {
                strcpy(themeType,"Posh");
            }
            else if(AGSH_STYLE==3)
            {
                strcpy(themeType,"Linux SH");
            }
            else if(AGSH_STYLE==4)
            {
                strcpy(themeType,"DOS");
            }
            else if(AGSH_STYLE==5)
            {
                strcpy(themeType,"Kali");
            }
            else
            {
                strcpy(netIf,"Unknown");
            }
            printf("                                   |  CPU:%s\n",cpuType);
            printf("    /\\     -----    /---\\   /----- |  RAM:%fMiB\n",_m);
            printf("   /  \\    .        |   |   |      |  RTFS:%fMiB\n",_d);
            printf("  /....\\   -----    |   |   \\----\\ |  GPU:Standard VGA/SVGA 640*480*6\n");
            printf(" /      \\  .        |   |        | |  OS:AEOS v5.11.2 Beta 7.1 Kernel Patch 4(Build 8070)\n");
			printf("/        \\ -----    \\---/   -----/ |  WM&DE:BGIDM\n");
			printf(" _   _                      _      |  Shell:AGSH v0.9.5\n");
            printf("|_| |_| |_  _ |_       |_| | |     |  Shell theme:%s\n",themeType);
			printf("|   | | |_ |_ | |   \\/   |.|_|     |  Kernel:MS-DOS 8.0(4.90.3000)\n");
            printf("                                   |  Network:SNet eX1000 on %s\n",netIf);
            free(themeType);
            free(netIf);
            free(cpuType);
		}
		else
		{
			printf("AGSH:Unknown Command:%s\n",inputBuf);
		}
	}
}
