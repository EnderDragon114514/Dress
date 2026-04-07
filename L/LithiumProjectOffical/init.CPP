#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <dos.h>

#ifdef _WIN32
#define delay _sleep
#define getch _getch
#endif

#define NORMAL 0x0
#define MEM_OVERFLOW 0x10
#define STACK_OVERFLOW 0x11
#define FILE_NOT_FOUND 0x12
#define KERNEL_NOT_FOUND 0x13
#define KERNEL_CRITICAL_ERROR 0x14
#define VM_PIPE_BROKEN 0x15
#define CPU_OVERLOAD 0x16
#define UNKNOWN 0x17
#define DISK_ERROR 0x18
#define FILE_RO 0x19
#define CONFIG_RO 0x20

int errcode=NORMAL;

void beep() {
	printf("%c",7);
}

int main() {
start:
	beep();
	system("cls");
	system("echo =============================================== >> latest.log");
	system("echo AEOS Boot Loader v1.1 SE Beta 1 Starting... >> latest.log");
	system("echo =============================================== >> latest.log");
	system("echo Booting up operating system... >> latest.log");
	printf("AEOS Boot Loader v1.1 SE Beta 1\n");
	printf("===================================\n");
	printf("[1]AEOS v4.1-rc1d\n");
	printf("Booting this operating system after 3(S)...");
	delay(1000);
	system("cls");
	printf("AEOS Boot Loader v1.1 SE Beta 1\n");
	printf("===================================\n");
	printf("[1]AEOS v4.1-rc1d\n");
	printf("Booting this operating system after 2(S)...");
	delay(1000);
	system("cls");
	printf("AEOS Boot Loader v1.1 SE Beta 1\n");
	printf("===================================\n");
	printf("[1]AEOS v4.1-rc1d\n");
	printf("Booting this operating system after 1(S)...");
	delay(1000);
	system("cls");
	system("echo Operating System Selected:AEOS v4.1-rc1d >> latest.log");
	system("echo Launching AEOS v4.1-rc1d >> latest.log");
	printf("AEOS Boot Loader v1.1 SE Beta 1\n");
	printf("===================================\n");
	printf("[1]AEOS v4.1-rc1d\n");
	printf("Booting this operating system now...");
	delay(1000);
	beep();
	system("cls");
	system("echo [BOOTLDR]Mounting volume... >> latest.log");
	printf("[BOOTLDR]Mounting volume...\n");
	system("echo [BOOTLDR]Initilazing Kernel... >> latest.log");
	printf("[BOOTLDR]Loading Kernel...\n");
	printf("[BOOTLDR]Launching Kernel...\n");
	delay(500);
	system("echo [BOOTLDR]Kernel Initilaze failed!Retrying... >> latest.log");
	printf("[BOOTLDR]Kernel launch failed,retrying...\n");
	delay(125);
	system("echo [BOOTLDR]Kernel Initilaze Success! >> latest.log");
	printf("[BOOTLDR]Kernel launched successful!\n");
	delay(500);
	system("echo [System Kernel]Kernel launched as PID:1 >> latest.log");
	printf("[System Kernel]Kernel launched as PID 1\n");
	printf("[System Kernel]Loading System...\n");
	delay(750);
	system("echo [System Kernel]Initilazing GUI... >> latest.log");
	printf("[System Kernel]Initilazing Desktop...\n");
	delay(875);
	system("echo [Desktop GUI]Rendering Desktop... >> latest.log");
	printf("[Desktop GUI]Rendering Desktop...\n");
	delay(1000);
	while(1) {
	menu:
		system("echo [Desktop GUI:Render Thread]DesktopGUI >> latest.log");
		system("cls");
		printf("                   A E O S   D E S K T O P   G U I                        \n");
		printf("==========================================================================\n");
		printf("| O | F i l e | P r o g r a m | H e l p |                                |\n");
		printf("==========================================================================\n");
		char a=getch();
		if(a==24) {
			system("echo [DesktopGUI:Callback Thread]Call MainMenu To OSKernel >> latest.log");
			system("cls");
			printf("                   A E O S   D E S K T O P   G U I                        \n");
			printf("==========================================================================\n");
			printf("| O | F i l e | P r o g r a m | H e l p |                                |\n");
			printf("|----\\====================================================================\n");
			printf("|About|\n");
			printf("|Shell|\n");
			printf("|Power|\n");
			printf("|=====|\n");
			a=getch();
			if(a==24||a==27) {
				system("echo [DesktopGUI:Callback Thread]MainMenu cancelled by user >> latest.log");
				goto menu;
			} else if(a=='P'||a=='p') {
				system("echo [DesktopGUI:Callback Thread]Power option selected >> latest.log");
				break;
			} else if(a=='S'||a=='s') {
				system("echo [DesktopGUI:Callback Thread]Shell option selected >> latest.log");
			dosshell:
				system("echo [System Kernel:Executive thread]Launching DOS Terminal... >> latest.log");
				system("cls");
				printf("                   A E O S   D E S K T O P   G U I                        \n");
				printf("==========================================================================\n");
				printf("| O | F i l e | P r o g r a m | H e l p |                                |\n");
				printf("==========================================================================\n");
				printf("                      M S - D O S   T e r m i n a l                       \n");
				printf("--------------------------------------------------------------------------\n");
				printf("Type 'EXIT' to quit Shell\n");
				printf("Press [Ctrl]+[Alt]+[Delete] to terminate system\n");
				system("command.com");
				system("pause");
				system("echo [System Kernel:Executive thread]DOS Terminal exited with code %ERRORLEVEL% >> latest.log");
			} else if(a=='A'||a=='a') {
				system("echo [System Kernel:Executive thread]Launching About... >> latest.log");
				system("cls");
				printf("                   A E O S   D E S K T O P   G U I                        \n");
				printf("==========================================================================\n");
				printf("| O | F i l e | P r o g r a m | H e l p |                                |\n");
				printf("==========================================================================\n");
				printf("\n");
				printf("\n");
				printf("              ====================================[X]=\n");
				printf("              |                A b o u t             |\n");
				printf("              |--------------------------------------|\n");
				printf("              |    #     #####    ###         ####   |\n");
				printf("              |   # #            #   #       #       |\n");
				printf("              |  #####   #####   #   #       #       |\n");
				printf("              |  #   #           #   #       #       |\n");
				printf("              |  #   #   #####    ###    ####        |\n");
				printf("              |           AEOS v3.1-rc1d-GUI         |\n");
				printf("              |(C)Copyright 2022~2025 LithiumProject |\n");
				printf("              |                  [OK]                |\n");
				printf("              ========================================\n");
				while(1) {
					a=getch();
					if(a==27||a=='X'||a=='x'||a=='O'||a=='o'||a=='\r'||a=='\n') {
						system("echo [OSKernel]'About' exited with code 0 >> latest.log");
						break;
					}
				}
			} else {
				system("echo [DesktopGUI:Callback Thread]Unknown option in MainMenu >> latest.log");
				continue;
			}
		} else if(a==33) {
		fileloop:
			system("echo [OSKernel]Triggered menu:File >> latest.log");
			system("cls");
			printf("                   A E O S   D E S K T O P   G U I                        \n");
			printf("==========================================================================\n");
			printf("| O | F i l e | P r o g r a m | H e l p |                                |\n");
			printf("====-----------===========================================================\n");
			printf("    |Exit AEOS|\n");
			printf("    ===========\n");
			a=getch();
			if(a==33||a==27) {
				system("echo [OSKernel]File menu cancelled >> latest.log");
				goto menu;
			} else if(a=='E'||a=='e') {
				system("echo [OSKernel]Exit AEOS selected from File menu >> latest.log");
				goto shutdown;
			} else {
				system("echo [OSKernel]Unknown option in File menu >> latest.log");
				goto fileloop;
			}
		} else if(a==25) {
			system("echo [OSKernel]Triggered menu:Program >> latest.log");
		progloop:
			system("cls");
			printf("                   A E O S   D E S K T O P   G U I                        \n");
			printf("==========================================================================\n");
			printf("| O | F i l e | P r o g r a m | H e l p |                                |\n");
			printf("==============-----------------===========================================\n");
			printf("              | MS-DOS Shell  |\n");
			printf("              | Control Pannel|\n");
			printf("              | Sticky Notes  |\n");
			printf("              | RbVM Executive|\n");
			printf("              | DCP Executive |\n");
			printf("              =================\n");
			a=getch();
			if(a==25||a==27) {
				system("echo [OSKernel]Program menu cancelled >> latest.log");
				goto menu;
			} else if(a=='M'||a=='m') {
				system("echo [OSKernel]MS-DOS Shell selected from Program menu >> latest.log");
				goto dosshell;
			} else if(a=='C'||a=='c') {
				system("echo [OSKernel]Control Pannel selected from Program menu >> latest.log");
				system("cls");
				printf("                   A E O S   D E S K T O P   G U I                        \n");
				printf("==========================================================================\n");
				printf("| O | F i l e | P r o g r a m | H e l p |                                |\n");
				printf("==========================================================================\n");
				printf("                       C o n t r o l   P a n n e l                  [X]   \n");
				printf("--------------------------------------------------------------------------\n");
				printf("[$]Enable MS-DOS Shell(You cannot turn off DOS kernel in verD release)\n");
				printf("[ ]Enable Network and Sound System(Internet Interface Driver Not Found)\n");
				printf("[ ]Enable Mouse Pointer(Mouse Driver Failure:CuteMouse Driver Not Found\n");
				printf("Error:No settings can be edited,press any key to quit control pannel...");
				errcode=CONFIG_RO;
				system("echo [OSKernel]Control Pannel exited with CONFIG_RO error >> latest.log");
				getch();
			} else if(a=='S'||a=='s') {
				system("echo [OSKernel]Sticky Notes selected from Program menu >> latest.log");
				system("cls");
				printf("                   A E O S   D E S K T O P   G U I                        \n");
				printf("==========================================================================\n");
				printf("| O | F i l e | P r o g r a m | H e l p |                                |\n");
				printf("==========================================================================\n");
				printf("                         S t i c k y   N o t e s                    [X]   \n");
				printf("--------------------------------------------------------------------------\n");
				printf("Type Ctrl+Alt+C to quit Sticky Notes...\n");
				while(1) {
					a=getch();
					if(a==46) {
						system("echo [OSKernel]Sticky Notes exited normally >> latest.log");
						break;
					} else if(a==7) {
						system("echo [OSKernel]Sticky Notes triggered STACK_OVERFLOW error >> latest.log");
						errcode=STACK_OVERFLOW;
						goto kernelpanic;
					}
					printf("%c",a);
				}
			} else if(a=='R'||a=='r') {
				system("echo [OSKernel]RbVM Executive selected from Program menu >> latest.log");
			rbvme:
				system("cls");
				printf("                   A E O S   D E S K T O P   G U I                        \n");
				printf("==========================================================================\n");
				printf("| O | F i l e | P r o g r a m | H e l p |                                |\n");
				printf("==========================================================================\n");
				printf("\n");
				printf("                     =============================[X]=\n");
				printf("                     |           RbVM Executive      |\n");
				printf("                     |-------------------------------|\n");
				printf("                     |RbVM v0.2.0-rc0d for AEOS v4.1 |\n");
				printf("                     |   Insert Executable disk...   |\n");
				printf("                     |  And press Enter to execute   |\n");
				printf("                     =================================\n");
				while(1) {
					int a=getch();
					if(a=='\r'||a=='\n') {
						system("echo [RbVM Executive]Checking for executable disk... >> latest.log");
						system("echo @echo off > chkrbvm.bat");
						system("echo if exist \"A:\\rb_entry.rba\" >> chkrbvm.bat");
						system("echo ( >> chkrbvm.bat");
						system("echo 	exit /b 0 >> chkrbvm.bat");
						system("echo ) >> chkrbvm.bat");
						system("echo else >> chkrbvm.bat");
						system("echo ( >> chkrbvm.bat");
						system("echo 	exit /b 1 >> chkrbvm.bat");
						system("echo ) >> chkrbvm.bat");
						int ret=system("chkrbvm.bat");
						if(ret==0) {
							system("echo [RbVM Executive]Executable found, launching... >> latest.log");
							int ret1=system("rbvm A:\\rb_entry.rba");
							system("cls");
							printf("                   A E O S   D E S K T O P   G U I                        \n");
							printf("==========================================================================\n");
							printf("| O | F i l e | P r o g r a m | H e l p |                                |\n");
							printf("==========================================================================\n");
							printf("\n");
							printf("                     =============================[X]=\n");
							printf("                     |           RbVM Executive      |\n");
							printf("                     |-------------------------------|\n");
							printf("                     |RbVM v0.2.0-rc0d for AEOS v4.1 |\n");
							printf("                     |   Program exited with code %d  |\n",ret1);
							printf("                     |   Press any key to exit.. .   |\n");
							printf("                     =================================\n");
							system("echo [RbVM Executive]Program exited with code %ERRORLEVEL% >> latest.log");
							getch();
							system("echo [RbVM Executive]Returning to main menu >> latest.log");
							goto menu;
						} else {
							errcode=DISK_ERROR;
							system("echo [RbVM Executive]Disk error: Executable not found >> latest.log");
							system("cls");
							printf("                   A E O S   D E S K T O P   G U I                        \n");
							printf("==========================================================================\n");
							printf("| O | F i l e | P r o g r a m | H e l p |                                |\n");
							printf("==========================================================================\n");
							printf("\n");
							printf("                     =============================[X]=\n");
							printf("                     |           RbVM Executive      |\n");
							printf("                     |-------------------------------|\n");
							printf("                     |RbVM v0.2.0-rc0d for AEOS v4.1 |\n");
							printf("                     |   Inse==================...   |\n");
							printf("                     |  And p|      Error     |ute   |\n");
							printf("                     ========|----------------|=======\n");
							printf("                             |  Invalid Disk  |\n");
							printf("                             |  ErrCode:0x18  |\n");
							printf("                             |[Retry] [Cancel]|\n");
							printf("                             ==================\n");
							while(1)
							{
								int a=getch();
								if(a=='R'||a=='r')
								{
									system("echo [RbVM Executive]User selected Retry >> latest.log");
									goto rbvme;
								}
								else if(a=='C'||a=='c')
								{
									system("echo [RbVM Executive]User selected Cancel, returning to menu >> latest.log");
									goto menu;
								}
							}
						}
					}
				}
			}  else if(a=='D'||a=='d') {
				system("echo [OSKernel]DCP Executive selected from Program menu >> latest.log");
			dcevme:
				system("cls");
				printf("                   A E O S   D E S K T O P   G U I                        \n");
				printf("==========================================================================\n");
				printf("| O | F i l e | P r o g r a m | H e l p |                                |\n");
				printf("==========================================================================\n");
				printf("\n");
				printf("                     =============================[X]=\n");
				printf("                     |     DOS Compact Executive     |\n");
				printf("                     |-------------------------------|\n");
				printf("                     |DCEVM v1.2.02-v2d for AEOS v4.1|\n");
				printf("                     |   Insert Executable disk...   |\n");
				printf("                     |  And press Enter to execute   |\n");
				printf("                     =================================\n");
				while(1) {
					int a=getch();
					if(a=='\r'||a=='\n') {
						system("echo [DCEVM Executive]Checking for executable disk... >> latest.log");
						system("echo @echo off > chkdcevm.bat");
						system("echo if exist \"A:\\dce_entry.dce\" exit 0 >> chkdcevm.bat");
						system("echo exit 1 >> chkdcevm.bat");
						int ret=system("chkdcevm.bat");
						if(ret==0) {
							system("echo [DCP Executive]Executable found, launching... >> latest.log");
							int ret1=system("A:\\dce_entry.dce");
							system("cls");
							printf("                   A E O S   D E S K T O P   G U I                        \n");
							printf("==========================================================================\n");
							printf("| O | F i l e | P r o g r a m | H e l p |                                |\n");
							printf("==========================================================================\n");
							printf("\n");
							printf("                     =============================[X]=\n");
							printf("                     |     DOS Compact Executive     |\n");
							printf("                     |-------------------------------|\n");
							printf("                     |DCEVM v1.2.02-v2d for AEOS v4.1|\n");
							printf("                     |   Program exited with code %d  |\n",ret1);
							printf("                     |   Press any key to exit.. .   |\n");
							printf("                     =================================\n");
							system("echo [DCP Executive]Program exited with code %ERRORLEVEL% >> latest.log");
							getch();
							system("echo [DCP Executive]Returning to main menu >> latest.log");
							goto menu;
						} else {
                                                        errcode=DISK_ERROR;
							system("echo [DCP Executive]Disk error: Executable not found >> latest.log");
							system("cls");
							printf("                   A E O S   D E S K T O P   G U I                        \n");
							printf("==========================================================================\n");
							printf("| O | F i l e | P r o g r a m | H e l p |                                |\n");
							printf("==========================================================================\n");
							printf("\n");
							printf("                     =============================[X]=\n");
							printf("                     |     DOS Compact Executive     |\n");
							printf("                     |-------------------------------|\n");
							printf("                     |DCEVM v1.2.02-v2d for AEOS v4.1|\n");
							printf("                     |   Inse==================...   |\n");
							printf("                     |  And p|      Error     |ute   |\n");
							printf("                     ========|----------------|=======\n");
							printf("                             |  Invalid Disk  |\n");
							printf("                             |  ErrCode:0x18  |\n");
							printf("                             |[Retry] [Cancel]|\n");
							printf("                             ==================\n");
							while(1)
							{
								int a=getch();
								if(a=='R'||a=='r')
								{
									system("echo [DCP Executive]User selected Retry >> latest.log");
									goto dcevme;
								}
								else if(a=='C'||a=='c')
								{
									system("echo [DCP Executive]User selected Cancel, returning to menu >> latest.log");
									goto menu;
								}
							}
						}
					}
				}
			}else {
				goto progloop;
			}
		} else if(a==28||a==127||a==14) {
			system("echo [OSKernel]Kernel panic triggered>> latest.log");
		kernelpanic:
			beep();
			system("cls");
			printf("[BOOTLDR]Kernel quited with code -1\n");
			printf("[BOOTLDR]Dectected Kernel Panic Error\n");
			delay(500);
			printf("[BOOTLDR]Starting bug reporter\n");
			delay(937);
			printf("[Bug Reporter]Dumped crash info successful!\n");
			delay(375);
			printf("\n");
			printf("              =================================================\n");
			printf("              |            K e r n e l   P a n i c            |\n");
			printf("              |-----------------------------------------------|\n");
			printf("              |             Error code:%#x                   |\n",errcode);
			printf("              | Please hold on the power botton for a few sec |\n");
			printf("              |     Or click 'r' to reboot your computer      |\n");
			printf("              |  For this error,pls contact the support team  |\n");
			printf("              |                     [Reboot]                  |\n");
			printf("              =================================================\n");
			char cmd1[15]="exit /b ";
			char cmd2[7]="";
			itoa(errcode,cmd2,10);
			strcat(cmd1,cmd2);
			system("echo [Kernel Panic]System halted with error code %ERRORLEVEL% >> latest.log");
			while(1) {
				a=getch();
				if(a=='R'||a=='r') {
					system("echo [Kernel Panic]User selected reboot >> latest.log");
					system("echo =============================================== >> latest.log");
					system("echo System reboot initiated >> latest.log");
					system("echo =============================================== >> latest.log");
					goto start;
				}
			}
		} else if(a==35) {
			system("echo [OSKernel]Help menu triggered >> latest.log");
			beep();
			system("cls");
			printf("                   A E O S   D E S K T O P   G U I                        \n");
			printf("==========================================================================\n");
			printf("| O | F i l e | P r o g r a m | H e l p |                                |\n");
			printf("==========================================================================\n");
			printf("\n");
			printf("\n");
			printf("\n");
			printf("\n");
			printf("\n");
			printf("              ====================================[X]=\n");
			printf("              |               E r r o r              |\n");
			printf("              |--------------------------------------|\n");
			printf("              |  There is no help topic in this PC   |\n");
			printf("              |                  [OK]                |\n");
			printf("              ========================================\n");
			system("echo [Help System]No help topic available >> latest.log");
			while(1) {
				a=getch();
				if(a==27||a=='X'||a=='x') {
					system("echo [Help System]Help dialog closed >> latest.log");
					break;
				}
			}
		}
	}
shutdown:
	system("echo [OSKernel]Shutdown sequence initiated >> latest.log");
	system("cls");
	printf("AEOS is shutting down...");
	system("echo [OSKernel]AEOS is shutting down... >> latest.log");
	delay(3750);
	while(1) {
		system("cls");
		printf("                   A E O S   D E S K T O P   G U I                        \n");
		printf("==========================================================================\n");
		printf("| O | F i l e | P r o g r a m | H e l p |                                |\n");
		printf("==========================================================================\n");
		printf("\n");
		printf("\n");
		printf("\n");
		printf("\n");
		printf("\n");
		printf("              ========================================\n");
		printf("              |               P o w e r              |\n");
		printf("              |--------------------------------------|\n");
		printf("              | Now you can shut down your computer  |\n");
		printf("              |               [Restart]              |\n");
		printf("              ========================================\n");
		system("echo [Power Menu]Displaying power options >> latest.log");
		int b=getch();
		if(b=='R'||b=='r') {
			system("echo [Power Menu]User selected restart >> latest.log");
			system("echo =============================================== >> latest.log");
			system("echo System restart initiated from power menu >> latest.log");
			system("echo =============================================== >> latest.log");
			goto start;
		} else {
			system("echo [Power Menu]Unknown option selected, waiting for valid input >> latest.log");
		}
	}
	system("echo [OSKernel]System shutdown complete >> latest.log");
	system("echo =============================================== >> latest.log");
	return 0;
}
