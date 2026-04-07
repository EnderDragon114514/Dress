/*
AEOS v5.11.2 Boot and kernel initilazer
Release date:26/2/7
*/
#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <conio.h>
int main()
{
    printf("Probing driver...\n");
    system("ctmouse.exe");
    printf("Initilazing kernel...\n");
    FILE* f=fopen("51kern.exe","r");
    if(!f)
    {
        printf("[CRITICAL ERROR]Operating system kernel not found!\n");
        printf("System halted\n");
        while(1)
        {
            getch();
        }
    }
    fclose(f);
    while(1)
    {
        int ret=system("51kern.exe");
        if(ret!=1)
        {
            system("cls");
            printf("[FATAL ERROR]Kernel panic!\n");
            printf("At:%d\n",ret);
            printf("Attempt to reboot in 10s...\r");
            delay(1000);
            printf("Attempt to reboot in 9s...\r");
            delay(1000);
            printf("Attempt to reboot in 8s...\r");
            delay(1000);
            printf("Attempt to reboot in 7s...\r");
            delay(1000);
            printf("Attempt to reboot in 6s...\r");
            delay(1000);
            printf("Attempt to reboot in 5s...\r");
            delay(1000);
            printf("Attempt to reboot in 4s...\r");
            delay(1000);
            printf("Attempt to reboot in 3s...\r");
            delay(1000);
            printf("Attempt to reboot in 2s...\r");
            delay(1000);
            printf("Attempt to reboot in 1s...\r");
            delay(1000);
            printf("Attempt to reboot in 0s...\r");
            delay(1000);
            printf("If auto-reboot fails,press Ctrl-Alt-Delete to reboot or manually reset your machine\n");
            system("shutdown r");
        }
    }
}
