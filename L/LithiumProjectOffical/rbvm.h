#include <stdlib.h>
#include "A:\memdef.h"
#include "A:\memdef1.h"
#include "A:\memdef2.h"
#include "A:\memdef3.h"
/*
 # ifdef bgidm
 #define maxProg 1
 #else
 #define maxProg 7
 #endif
 */
//int memi[maxProg][128];
//float memf[maxProg][128];
//char memc[maxProg][128];
//int sectline[maxProg][64];
//FILE* fHandle[maxProg][8];
int progs=0;
unsigned long int sysClockTick=0;
#define COM1 0x3F8
#define COM2 0x2F8
#define COM3 0x3E8
#define COM4 0x2E8
#define ERR_DISK_IO 0x3C
#define ERR_NET_FAIL 0x3A
#define ERR_TIMEOUT 0x3B
#define SYS_KILL -1
#define NET_SEG_INIT 0xAE
#define NET_SEG_ACK 0xFD
#define NET_SEG_NAK 0xFE
#define NET_SEG_ETX 0xFA
#define NET_SEG_RTX 0xFB
#define RX_BUFFER_SIZE 64
#define MAX_WAIT_TICK 524288

FILE* fin[maxProg];
int clamp(int x,int a,int b) {
    if(x<a) {
        return a;
    }
    if(x>b) {
        return b;
    } else {
        return x;
    }
}
int get_uint()
{
    char inputBuf[64];
    int key=0,currentChr=0;
    memset(inputBuf,0,sizeof(inputBuf));
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
                printc("\b \b");
                inputBuf[currentChr+1]=0;
            }

        }
        if(key>=48&&key<=57)
        {
            if(currentChr>=64)
            {
                currentChr=63;
                break;
            }
            inputBuf[currentChr++]=key;
            printc("%c",key);
        }
        if(key==3||key==9||key==10||key=='\r'||key=='\n')
        {
            inputBuf[currentChr]=0;
            break;
        }
        if(key==28)
        {
            return -2;
        }
    }
    return atoi(inputBuf);
}
int condition(int procid,int type, int id_a, const char *_cmd, int id_b) {
    if (type == 1) {
        if (strcmp(_cmd, "equ") == 0) {
            return memi[procid][id_a] == memi[procid][id_b] ? 1 : 0;
        } else if (strcmp(_cmd, "grt") == 0) {
            return memi[procid][id_a] > memi[procid][id_b] ? 1 : 0;
        } else if (strcmp(_cmd, "les") == 0) {
            return memi[procid][id_a] < memi[procid][id_b] ? 1 : 0;
        } else if (strcmp(_cmd, "neq") == 0) {
            return memi[procid][id_a] != memi[procid][id_b] ? 1 : 0;
        } else {
            printc("Operation error\n");
            return -1;
        }
    } else if (type == 2) {
        if (strcmp(_cmd, "equ") == 0) {
            return memc[procid][id_a] == memc[procid][id_b] ? 1 : 0;
        } else if (strcmp(_cmd, "grt") == 0) {
            return memc[procid][id_a] > memc[procid][id_b] ? 1 : 0;
        } else if (strcmp(_cmd, "les") == 0) {
            return memc[procid][id_a] < memc[procid][id_b] ? 1 : 0;
        } else if (strcmp(_cmd, "neq") == 0) {
            return memc[procid][id_a] != memc[procid][id_b] ? 1 : 0;
        } else {
            printc("Operation error\n");
            return -1;
        }
    } else if (type == 3) {
        if (strcmp(_cmd, "equ") == 0) {
            return memf[procid][id_a] == memf[procid][id_b] ? 1 : 0;
        } else if (strcmp(_cmd, "grt") == 0) {
            return memf[procid][id_a] > memf[procid][id_b] ? 1 : 0;
        } else if (strcmp(_cmd, "les") == 0) {
            return memf[procid][id_a] < memf[procid][id_b] ? 1 : 0;
        } else if (strcmp(_cmd, "neq") == 0) {
            return memf[procid][id_a] != memf[procid][id_b] ? 1 : 0;
        } else {
            printc("Operation error\n");
            return -1;
        }
    } else {
        printc("Type error\n");
        return -1;
    }
}
void read_string(FILE *f, char *buffer) {
    int i = 0;
    int ch;
    while ((ch = fgetc(f)) != EOF && (ch == ' ' || ch == '\t'));
    if (ch == EOF) {
        buffer[0] = '\0';
        return;
    }
    while (ch != EOF && ch != '\n' && ch != '\r' && i < 64) {
        buffer[i++] = (char)ch;
        ch = fgetc(f);
    }
    buffer[i] = '\0';
}
void read_string_32(FILE *f, char *buffer) {
    int i = 0;
    int ch;
    while ((ch = fgetc(f)) != EOF && (ch == ' ' || ch == '\t'));
    if (ch == EOF) {
        buffer[0] = '\0';
        return;
    }
    while (ch != EOF && ch != '\n' && ch != '\r' && i < 32) {
        buffer[i++] = (char)ch;
        ch = fgetc(f);
    }
    buffer[i] = '\0';
}
int encode_procid(int pid, int section) {
    return 4 + (pid * 4) + section;
}
void decode_procid(int procid, int *pid, int *section) {
    if (procid <= 4) {
        *pid = procid;
        *section = -1;
    } else {
        int temp = procid - 4;
        *pid = (temp - 1) / 4;
        *section = temp % 4;
    }
}
int waitTicks=0;
int run(int procid,const char *_cmd) {
    sysClockTick++;
    char buffer[128];
    int a, b, c, d;
    char type_str[10];
    char type2_str[10];
    if(waitTicks>0)
    {
        if(getkey() == 28)
        {
            return -2;
        }
        waitTicks--;
        return 1;
    }
    else if(procid>4&&procid<=20)
    {
        int procid_tr=0;
        int section_id=0;
        decode_procid(procid,&procid_tr,&section_id);
        if(section_id<=0)
        {
            printc("Stack error\n");
            return -1;
        }
        if(sectstate[procid_tr][section_id-1]==1)
        {
            if(sectline[procid_tr][section_id]<16)
            {
                read_string_32(fin[procid_tr],sect[procid_tr][section_id-1][sectline[procid_tr][section_id-1]]);
                sectline[procid_tr][section_id-1]++;
            }
            if(sectline[procid_tr][section_id-1]>=16||strcmp(sect[procid][section_id-1][clamp(sectline[procid_tr][section_id-1],0,31)],"section end")==0)
            {
                char *marker = ".SECTION_END";
                for(int i = 0; marker[i] != '\0' && i < 31; i++) {
                    sect[procid_tr][section_id-1][clamp(sectline[procid_tr][section_id-1],0,31)][i] = marker[i];
                }
                sect[procid_tr][section_id-1][clamp(sectline[procid_tr][section_id-1],0,31)][strlen(marker)] = '\0';
                sectstate[procid_tr][section_id-1]=0;
            }
        }
        else if(sectstate[procid_tr][section_id-1]==2)
        {
            if(strcmp(sect[procid][section_id-1][sectline[procid_tr][section_id-1]],".SECTION_END")!=0)
            {
                run(procid_tr,sect[procid][section_id-1][sectline[procid_tr][section_id-1]]);
            }
            else
            {
                sectstate[procid_tr][section_id-1]=0;
            }
        }
    }
    else if(strcmp(_cmd,"section")==0)
    {
        char* op;
        int a;
        fscanf(fin[procid], "%s %d", op, &a);
        if(strcmp(op,"set")==0)
        {
            if(a<1||a>4)
            {
                printc("Stack error\n");
                return -1;
            }
            sectstate[procid][a-1]=1;
        }
        else if(strcmp(op,"run")==0)
        {
            if(a<1||a>4)
            {
                printc("Stack error\n");
                return -1;
            }
            sectline[procid][a-1]=0;
            sectstate[procid][a-1]=2;
        }
        else
        {
            printc("Invalid operation\n");
        }
    }
    else if (strcmp(_cmd, "nop") == 0) {
        read_string(fin[procid], buffer);
        return 1;
    } else if (strcmp(_cmd, "end") == 0) {
        return 0;
    } else if (strcmp(_cmd, "sys") == 0) {
        char* op1, op2, type;
        int op3;
        fscanf(fin[procid], "%s", op1);
        if(strcmp((const char*)op1, "clock") == 0)
        {
            fscanf(fin[procid], "%s %s %d", op2, type, op3);
            if(strcmp((const char*)op2, "get") == 0)
            {
                if(strcmp((const char*)type, "int") == 0)
                {
                    if(op3 < 0 || op3 > 127)
                    {
                        printc("Memory error\n");
                        return -1;
                    }
                    memi[procid][op3] = sysClockTick;
                }
                else if(strcmp((const char*)type, "char") == 0)
                {
                    if(op3 < 0 || op3 > 127)
                    {
                        printc("Memory error\n");
                        return -1;
                    }
                    memc[procid][op3] = sysClockTick;
                }
                else if(strcmp((const char*)type, "float") == 0)
                {
                    if(op3 < 0 || op3 > 127)
                    {
                        printc("Memory error\n");
                        return -1;
                    }
                    memf[procid][op3] = sysClockTick;
                }
                else
                {
                    printc("Type error\n");
                    return -1;
                }
            }
            else if(strcmp((const char*)op2, "delay") == 0)
            {
                if(strcmp((const char*)type, "int") == 0)
                {
                    if(op3 < 0 || op3 > 127)
                    {
                        printc("Memory error\n");
                        return -1;
                    }
                    sysClockTick = memi[procid][op3];
                }
                else if(strcmp((const char*)type, "char") == 0)
                {
                    if(op3 < 0 || op3 > 127)
                    {
                        printc("Memory error\n");
                        return -1;
                    }
                    sysClockTick = memc[procid][op3];
                }
                else if(strcmp((const char*)type, "float") == 0)
                {
                    if(op3 < 0 || op3 > 127)
                    {
                        printc("Memory error\n");
                        return -1;
                    }
                    sysClockTick = memf[procid][op3];
                }
                else
                {
                    printc("Type error\n");
                    return -1;
                }
            }
            else
            {
                printc("Invalid parameter\n");
                return -1;
            }
        }
	//I AM NOT GOING TO IMPLENT THIS F**KING API
	/*
	else if(strcmp(_op1,"getage")==0)
	{
	    fscanf(fin[procid])
	}*/	
        else
        {
            printc("API Error\n");
            return -1;
        }
    } else if (strcmp(_cmd, "set") == 0) {
        fscanf(fin[procid], "%d %s", &a, type_str);
        if (a < 0 || a > 192) {
            printc("Memory error\n");
            return -1;
        }

        if (strcmp(type_str, "int") == 0) {
            if (a > 127) {
                printc("Memory error\n");
                return -1;
            }
            fscanf(fin[procid], "%d", &b);
            memi[procid][a] = b;
        } else if (strcmp(type_str, "char") == 0) {
            if (a > 127) {
                printc("Memory error\n");
                return -1;
            }
            fscanf(fin[procid], "%d", &b);
            memc[procid][a] = (char)b;
        } else if (strcmp(type_str, "float") == 0) {
            if (a > 127) {
                printc("Memory error\n");
                return -1;
            }
            fscanf(fin[procid], "%d", &b);
            memf[procid][a] = (float)b;
        } else if (strcmp(type_str, "string") == 0) {
            if (a < 128 || a > 192) {
                printc("Memory error\n");
                return -1;
            }
            read_string(fin[procid], buffer);
            a -= 128;
            strncpy(mems[procid][a], buffer, 127);
            mems[procid][a][127] = '\0';
        } else {
            printc("Type error\n");
            return -1;
        }
    } else if (strcmp(_cmd, "copy") == 0) {
        fscanf(fin[procid], "%d %d %s", &a, &b, type_str);
        if (a < 0 || a > 192 || b < 0 || b > 192) {
            printc("Memory error\n");
            return -1;
        }

        if (strcmp(type_str, "int") == 0) {
            if (a > 127 || b > 127) {
                printc("Memory error\n");
                return -1;
            }
            memi[procid][b] = memi[procid][a];
        } else if (strcmp(type_str, "char") == 0) {
            if (a > 127 || b > 127) {
                printc("Memory error\n");
                return -1;
            }
            memc[procid][b] = memc[procid][a];
        } else if (strcmp(type_str, "float") == 0) {
            if (a > 127 || b > 127) {
                printc("Memory error\n");
                return -1;
            }
            memf[procid][b] = memf[procid][a];
        } else if (strcmp(type_str, "string") == 0) {
            if (a < 128 || a > 192 || b < 128 || b > 192) {
                printc("Memory error\n");
                return -1;
            }
            a -= 128;
            b -= 128;
            strncpy(mems[procid][b], mems[procid][a], 127);
            mems[procid][b][127] = '\0';
        } else {
            printc("Type error\n");
            return -1;
        }
    } else if (strcmp(_cmd, "convert") == 0) {
        fscanf(fin[procid], "%d %s %d %s", &a, type_str, &c, type2_str);
        if (a < 0 || a > 127 || c < 0 || c > 127) {
            printc("Memory error\n");
            return -1;
        }
        if (strcmp(type_str, "int") == 0) {
            if (strcmp(type2_str, "int") == 0) {
                memi[procid][c] = memi[procid][a];
            } else if (strcmp(type2_str, "char") == 0) {
                memc[procid][c] = (char)memi[procid][a];
            } else if (strcmp(type2_str, "float") == 0) {
                memf[procid][c] = (float)memi[procid][a];
            } else if (strcmp(type2_str, "string") == 0) {
                sprintf(mems[procid][c],"%d",memi[procid][a]);
            } else {
                printc("Type error\n");
                return -1;
            }
        } else if (strcmp(type_str, "char") == 0) {
            if (strcmp(type2_str, "int") == 0) {
                memi[procid][c] = (int)memc[procid][a];
            } else if (strcmp(type2_str, "char") == 0) {
                memc[procid][c] = memc[procid][a];
            } else if (strcmp(type2_str, "float") == 0) {
                memf[procid][c] = (float)memc[procid][a];
            } else if (strcmp(type2_str, "string") == 0) {
                sprintf(mems[procid][c],"%d",(int)memc[procid][a]);
            } else {
                printc("Type error\n");
                return -1;
            }
        } else if (strcmp(type_str, "float") == 0) {
            if (strcmp(type2_str, "int") == 0) {
                memi[procid][c] = (int)memf[procid][a];
            } else if (strcmp(type2_str, "char") == 0) {
                memc[procid][c] = (char)memc[procid][a];
            } else if (strcmp(type2_str, "float") == 0) {
                memf[procid][c] = memf[procid][a];
            } else if (strcmp(type2_str, "string") == 0) {
                sprintf(mems[procid][c],"%f",memf[procid][a]);
            } else {
                printc("Type error\n");
                return -1;
            }
        } else if (strcmp(type_str, "string") == 0) {
            if (strcmp(type2_str, "int") == 0) {
                memi[procid][c] = atoi(mems[procid][a]);
            } else if (strcmp(type2_str, "char") == 0) {
                memc[procid][c] = atoi(mems[procid][a]);
            } else if (strcmp(type2_str, "float") == 0) {
                memf[procid][c] = atof(mems[procid][a]);
            } else if (strcmp(type2_str, "string") == 0) {
                sprintf(mems[procid][c],"%s",mems[procid][a]);
            } else {
                printc("Type error\n");
                return -1;
            }
        } else {
            printc("Type error\n");
            return -1;
        }
    } else if (strcmp(_cmd, "plus") == 0) {
        fscanf(fin[procid], "%d %d %s", &a, &b, type_str);
        if (a < 0 || a > 192 || b < 0 || b > 192) {
            printc("Memory error\n");
            return -1;
        }

        if (strcmp(type_str, "int") == 0) {
            fscanf(fin[procid], "%d", &c);
            if (c < 0 || c > 127) {
                printc("Memory error\n");
                return -1;
            }
            memi[procid][c] = memi[procid][a] + memi[procid][b];
        } else if (strcmp(type_str, "char") == 0) {
            fscanf(fin[procid], "%d", &c);
            if (c < 0 || c > 127) {
                printc("Memory error\n");
                return -1;
            }
            memc[procid][c] = memc[procid][a] + memc[procid][b];
        } else if (strcmp(type_str, "float") == 0) {
            fscanf(fin[procid], "%d", &c);
            if (c < 0 || c > 127) {
                printc("Memory error\n");
                return -1;
            }
            memf[procid][c] = memf[procid][a] + memf[procid][b];
        } else if (strcmp(type_str, "string") == 0) {
            fscanf(fin[procid], "%d", &c);
            if (a < 128 || a > 192 || b < 128 || b > 192 || c < 128 || c > 192) {
                printc("Memory error\n");
                return -1;
            }
            a -= 128;
            b -= 128;
            c -= 128;
            strncpy(mems[procid][c], mems[procid][a], 127);
            strncat(mems[procid][c], mems[procid][b], 127 - strlen(mems[procid][c]));
            mems[procid][c][127] = '\0';
        } else {
            printc("Type error\n");
            return -1;
        }
    } else if (strcmp(_cmd, "minus") == 0) {
        fscanf(fin[procid], "%d %d %s", &a, &b, type_str);
        if (a < 0 || a > 127 || b < 0 || b > 127) {
            printc("Memory error\n");
            return -1;
        }
        fscanf(fin[procid], "%d", &c);
        if (c < 0 || c > 127) {
            printc("Memory error\n");
            return -1;
        }

        if (strcmp(type_str, "int") == 0) {
            memi[procid][c] = memi[procid][a] - memi[procid][b];
        } else if (strcmp(type_str, "char") == 0) {
            memc[procid][c] = memc[procid][a] - memc[procid][b];
        } else if (strcmp(type_str, "float") == 0) {
            memf[procid][c] = memf[procid][a] - memf[procid][b];
        } else {
            printc("Type error\n");
            return -1;
        }
    } else if (strcmp(_cmd, "times") == 0) {
        fscanf(fin[procid], "%d %d %s", &a, &b, type_str);
        if (a < 0 || a > 127 || b < 0 || b > 127) {
            printc("Memory error\n");
            return -1;
        }
        fscanf(fin[procid], "%d", &c);
        if (c < 0 || c > 127) {
            printc("Memory error\n");
            return -1;
        }

        if (strcmp(type_str, "int") == 0) {
            memi[procid][c] = memi[procid][a] * memi[procid][b];
        } else if (strcmp(type_str, "char") == 0) {
            memc[procid][c] = memc[procid][a] * memc[procid][b];
        } else if (strcmp(type_str, "float") == 0) {
            memf[procid][c] = memf[procid][a] * memf[procid][b];
        } else {
            printc("Type error\n");
            return -1;
        }
    } else if (strcmp(_cmd, "divide") == 0) {
        fscanf(fin[procid], "%d %d %s", &a, &b, type_str);
        if (a < 0 || a > 127 || b < 0 || b > 127) {
            printc("Memory error\n");
            return -1;
        }
        fscanf(fin[procid], "%d", &c);
        if (c < 0 || c > 127) {
            printc("Memory error\n");
            return -1;
        }

        if (strcmp(type_str, "int") == 0) {
            if (memi[procid][b] != 0) {
                memi[procid][c] = memi[procid][a] / memi[procid][b];
            } else {
                printc("Divide by zero error\n");
                return -1;
            }
        } else if (strcmp(type_str, "char") == 0) {
            if (memc[procid][b] != 0) {
                memc[procid][c] = memc[procid][a] / memc[procid][b];
            } else {
                printc("Divide by zero error\n");
                return -1;
            }
        } else if (strcmp(type_str, "float") == 0) {
            if (memf[procid][b] != 0.0f) {
                memf[procid][c] = memf[procid][a] / memf[procid][b];
            } else {
                printc("Divide by zero error\n");
                return -1;
            }
        } else {
            printc("Type error\n");
            return -1;
        }
    } else if (strcmp(_cmd, "mod") == 0) {
        fscanf(fin[procid], "%d %d %s", &a, &b, type_str);
        if (a < 0 || a > 127 || b < 0 || b > 127) {
            printc("Memory error\n");
            return -1;
        }
        fscanf(fin[procid], "%d", &c);
        if (c < 0 || c > 127) {
            printc("Memory error\n");
            return -1;
        }

        if (strcmp(type_str, "int") == 0) {
            if (memi[procid][b] != 0) {
                memi[procid][c] = memi[procid][a] % memi[procid][b];
            } else {
                printc("Divide by zero error\n");
                return -1;
            }
        } else {
            printc("Type error\n");
            return -1;
        }
    } else if (strcmp(_cmd, "get") == 0) {
        fscanf(fin[procid], "%d %s", &a, type_str);
        if (a < 0 || a > 192) {
            printc("Memory error\n");
            return -1;
        }

        if (strcmp(type_str, "int") == 0) {
            if (a > 127) {
                printc("Memory error\n");
                return -1;
            }
            char inputBuf[64];
            int key=0,currentChr=0;
            memset(inputBuf,0,sizeof(inputBuf));
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
                        printc("\b \b");
                        inputBuf[currentChr+1]=0;
                    }
                }
                if((key>=48&&key<=57)||key=='-')
                {
                    if(currentChr>=64)
                    {
                        currentChr=63;
                        break;
                    }
                    inputBuf[currentChr++]=key;
                    printc("%c",key);
                }
                if(key==3||key==9||key==10||key=='\r'||key=='\n')
                {
                    inputBuf[currentChr]=0;
                    break;
                }
                if(key==26)
                {
                    return -3;
                }
                if(key==28)
                {
                    return -2;
                }
            }
            memi[procid][a]=atoi(inputBuf);
        } else if (strcmp(type_str, "char") == 0) {
            if (a > 127) {
                printc("Memory error\n");
                return -1;
            }
            int _a=getch();
            if(_a==26)
            {
                return -3;
            }
            if(_a==28)
            {
                return -2;
            }
            memc[procid][a]=_a;
        } else if (strcmp(type_str, "float") == 0) {
            if (a > 127) {
                printc("Memory error\n");
                return -1;
            }
            char inputBuf[64];
            int key=0,currentChr=0;
            memset(inputBuf,0,sizeof(inputBuf));
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
                        printc("\b \b");
                        inputBuf[currentChr+1]=0;
                    }
                }
                if((key>=48&&key<=57)||(key=='-')||(key=='.'))
                {
                    if(currentChr>=64)
                    {
                        currentChr=63;
                        break;
                    }
                    inputBuf[currentChr++]=key;
                    printc("%c",key);
                }
                if(key==3||key==9||key==10||key=='\r'||key=='\n')
                {
                    inputBuf[currentChr]=0;
                    break;
                }
                if(key==26)
                {
                    return -3;
                }
                if(key==28)
                {
                    return -2;
                }
            }
            memf[procid][a]=atof(inputBuf);
        } else if (strcmp(type_str, "string") == 0) {
            if (a < 128 || a > 192) {
                printc("Memory error\n");
                return -1;
            }
            a -= 128;
            fflush(stdin);
            char inputBuf[64];
            int key=0,currentChr=0;
            memset(inputBuf,0,sizeof(inputBuf));
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
                        printc("\b \b");
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
                    printc("%c",key);
                }
                if(key==3||key==9||key==10||key=='\r'||key=='\n')
                {
                    inputBuf[currentChr]=0;
                    break;
                }
                if(key==26)
                {
                    return -3;
                }
                if(key==28)
                {
                    return -2;
                }
            }
            strncpy(mems[procid][a], inputBuf, 127);
            mems[procid][a][127] = '\0';
        } else {
            printc("Type error\n");
            return -1;
        }
    } else if (strcmp(_cmd, "put") == 0) {
        fscanf(fin[procid], "%d %s", &a, type_str);
        if (a < 0 || a > 192) {
            printc("Memory error\n");
            return -1;
        }

        if (strcmp(type_str, "int") == 0) {
            if (a > 127) {
                printc("Memory error\n");
                return -1;
            }
            printc("%d\n", memi[procid][a]);
        } else if (strcmp(type_str, "char") == 0) {
            if (a > 127) {
                printc("Memory error\n");
                return -1;
            }
            printc("%c\n", memc[procid][a]);
        } else if (strcmp(type_str, "float") == 0) {
            if (a > 127) {
                printc("Memory error\n");
                return -1;
            }
            printc("%f\n", memf[procid][a]);
        } else if (strcmp(type_str, "string") == 0) {
            if (a < 128 || a > 192) {
                printc("Memory error\n");
                return -1;
            }
            a -= 128;
            printc("%s\n", mems[procid][a]);
        } else {
            printc("Type error\n");
            return -1;
        }
    } else if (strcmp(_cmd, "ssize") == 0) {
        fscanf(fin[procid], "%d %d", &a, &b);
        if (a < 128 || a > 192 || b < 0 || b > 127) {
            printc("Memory error\n");
            return -1;
        }
        a -= 128;
        memi[procid][b] = strlen(mems[procid][a]);
    } else if (strcmp(_cmd, "if") == 0) {
        int id_a, id_b;
        char type_str[10], op[10], cmd1[128], cmd2[128];
        fscanf(fin[procid], "%s %d %s %d", type_str, &id_a, op, &id_b);
        if (id_a < 0 || id_a > 127 || id_b < 0 || id_b > 127) {
            printc("Memory error\n");
            return -1;
        }

        int type;
        if (strcmp(type_str, "int") == 0) type = 1;
        else if (strcmp(type_str, "char") == 0) type = 2;
        else if (strcmp(type_str, "float") == 0) type = 3;
        else {
            printc("Type error\n");
            return -1;
        }

        int ret = condition(procid, type, id_a, op, id_b);
        if (ret == -1) {
            return -1;
        } else if (ret == 1) {
            fscanf(fin[procid], "%s", cmd1);
            return run(procid, cmd1);
        } else if (ret == 0) {
            fscanf(fin[procid], "%s", cmd2);
            return run(procid, cmd2);
        } else {
            printc("Unknown error\n");
            return -1;
        }
    } else if(strcmp(_cmd,"exec")==0){
        int a;
        fscanf(fin[procid],"%d",&a);
        if(a<128||a>192)
        {
            printc("Memory error");
            return -1;
        }
        int ret=run(procid,mems[procid][a-128]);
        return ret;
    } else if (strcmp(_cmd, "while") == 0) {
        int id_a, id_b;
        char type_str[10], op[10], cmd[128];
        fscanf(fin[procid], "%s %d %s %d %s", type_str, &id_a, op, &id_b, cmd);
        if (id_a < 0 || id_a > 127 || id_b < 0 || id_b > 127) {
            printc("Memory error\n");
            return -1;
        }

        int type;
        if (strcmp(type_str, "int") == 0) type = 1;
        else if (strcmp(type_str, "char") == 0) type = 2;
        else if (strcmp(type_str, "float") == 0) type = 3;
        else {
            printc("Type error\n");
            return -1;
        }

        while (1) {
            int con = condition(procid, type, id_a, op, id_b);
            if (con == 1) {
                int ret = run(procid, cmd);
                if (ret == 1) {
                    continue;
                } else {
		    return ret;
		} 
            } else {
	        return con;
	    }
        }
    } else if(strcmp(_cmd,"getkey")==0) {
        int id_a;
        fscanf(fin[procid],"%d",&id_a);
        if(id_a<0||id_a>127) {
            printc("Memory error\n");
            return -1;
        }
        memc[procid][id_a]=getch();
    } else if(strcmp(_cmd,"loadlib")==0) {
        char libname[32];
        fscanf(fin[procid],"%s", libname);
        if(strcmp(libname,"bgidm-gr.lib")==0)
        {
            if(graphAvail==1)
            {
                //do it next time~
            }
            else
            {
                printf("Runtime error:No Back-end for graphics\n");
                return -1;
            }
        }
        else if(strcmp(libname,"extmath.lib")==0)
        {
            char mathcmd[16];
            fscanf(fin[procid],"%s", mathcmd);

            if(strcmp(mathcmd,"sqrt")==0)
            {
                char type[8];
                int b,c;
                fscanf(fin[procid],"%s %d %d", type, &b, &c);
                if(b<0||b>127||c<0||c>127)
                {
                    printc("Memory error\n");
                    return -1;
                }
                if(strcmp(type,"int")==0)
                {
                    memi[procid][c]=sqrt(memi[procid][b]);
                }
                else if(strcmp(type,"char")==0)
                {
                    memc[procid][c]=sqrt(memc[procid][b]);
                }
                else if(strcmp(type,"float")==0)
                {
                    memf[procid][c]=sqrt(memf[procid][b]);
                }
                else
                {
                    printc("Type error\n");
                    return -1;
                }
            }
            else if(strcmp(mathcmd,"pow")==0)
            {
                char type[8];
                int b,c,d;
                fscanf(fin[procid],"%s %d %d %d", type, &b, &c, &d);
                if(b<0||b>127||c<0||c>127||d<0||d>127)
                {
                    printc("Memory error\n");
                    return -1;
                }
                if(strcmp(type,"int")==0)
                {
                    memi[procid][d]=pow(memi[procid][b],memi[procid][c]);
                }
                else if(strcmp(type,"char")==0)
                {
                    memc[procid][d]=pow(memc[procid][b],memc[procid][c]);
                }
                else if(strcmp(type,"float")==0)
                {
                    memf[procid][d]=pow(memf[procid][b],memf[procid][c]);
                }
                else
                {
                    printc("Type error\n");
                    return -1;
                }
            }
            else if(strcmp(mathcmd,"sin")==0)
            {
                char type[8];
                int b,c;
                fscanf(fin[procid],"%s %d %d", type, &b, &c);
                if(b<0||b>127||c<0||c>127)
                {
                    printc("Memory error\n");
                    return -1;
                }
                if(strcmp(type,"int")==0)
                {
                    memi[procid][c]=sin(memi[procid][b]);
                }
                else if(strcmp(type,"char")==0)
                {
                    memc[procid][c]=sin(memc[procid][b]);
                }
                else if(strcmp(type,"float")==0)
                {
                    memf[procid][c]=sin(memf[procid][b]);
                }
                else
                {
                    printc("Type error\n");
                    return -1;
                }
            }
            else if(strcmp(mathcmd,"cos")==0)
            {
                char type[8];
                int b,c;
                fscanf(fin[procid],"%s %d %d", type, &b, &c);
                if(b<0||b>127||c<0||c>127)
                {
                    printc("Memory error\n");
                    return -1;
                }
                if(strcmp(type,"int")==0)
                {
                    memi[procid][c]=cos(memi[procid][b]);
                }
                else if(strcmp(type,"char")==0)
                {
                    memc[procid][c]=cos(memc[procid][b]);
                }
                else if(strcmp(type,"float")==0)
                {
                    memf[procid][c]=cos(memf[procid][b]);
                }
                else
                {
                    printc("Type error\n");
                    return -1;
                }
            }
            else if(strcmp(mathcmd,"tan")==0)
            {
                char type[8];
                int b,c;
                fscanf(fin[procid],"%s %d %d", type, &b, &c);
                if(b<0||b>127||c<0||c>127)
                {
                    printc("Memory error\n");
                    return -1;
                }
                if(strcmp(type,"int")==0)
                {
                    if (abs(memi[procid][b]) > PI/2) {
                        float a1 = fmod(memi[procid][b],PI);
                        if (a1 > (PI/2)) a1 -= PI;
                        if (a1 < -1*(PI/2)) a1 += PI;
                        if (fabs(fabs(a1) - PI/2) < 1e-7) {
                            printc("Tan calculation error\n");
                            return -1;
                        }
                    }
                    memi[procid][c]=tan(memi[procid][b]);
                }
                else if(strcmp(type,"char")==0)
                {
                    if (abs(memc[procid][b]) > PI/2) {
                        float a1 = fmod(memc[procid][b],PI);
                        if (a1 > (PI/2)) a1 -= PI;
                        if (a1 < -1*(PI/2)) a1 += PI;
                        if (fabs(fabs(a1) - PI/2) < 1e-7) {
                            printc("Tan calculation error\n");
                            return -1;
                        }
                    }
                    memc[procid][c]=tan(memc[procid][b]);
                }
                else if(strcmp(type,"float")==0)
                {
                    if (fabs(memf[procid][b]) > PI/2) {
                        float a1 = fmod(memf[procid][b],PI);
                        if (a1 > (PI/2)) a1 -= PI;
                        if (a1 < -1*(PI/2)) a1 += PI;
                        if (fabs(fabs(a1) - PI/2) < 1e-7) {
                            printc("Tan calculation error\n");
                            return -1;
                        }
                    }
                    memf[procid][c]=tan(memf[procid][b]);
                }
                else
                {
                    printc("Type error\n");
                    return -1;
                }
            }
            else if(strcmp(mathcmd,"log")==0)
            {
                char type[8];
                int b,c;
                fscanf(fin[procid],"%s %d %d", type, &b, &c);
                if(b<0||b>127||c<0||c>127)
                {
                    printc("Memory error\n");
                    return -1;
                }
                if(strcmp(type,"int")==0)
                {
                    if(memi[procid][b]<=0)
                    {
                        printc("log invalid parameter\n");
                        return -1;
                    }
                    memi[procid][c]=log10(memi[procid][b]);
                }
                else if(strcmp(type,"char")==0)
                {
                    if(memc[procid][b]<=0)
                    {
                        printc("log invalid parameter\n");
                        return -1;
                    }
                    memc[procid][c]=log10(memc[procid][b]);
                }
                else if(strcmp(type,"float")==0)
                {
                    if(memf[procid][b]<=0)
                    {
                        printc("log invalid parameter\n");
                        return -1;
                    }
                    memf[procid][c]=log10(memf[procid][b]);
                }
                else
                {
                    printc("Type error\n");
                    return -1;
                }
            }
            else if(strcmp(mathcmd,"ln")==0)
            {
                char type[8];
                int b,c;
                fscanf(fin[procid],"%s %d %d", type, &b, &c);
                if(b<0||b>127||c<0||c>127)
                {
                    printc("Memory error\n");
                    return -1;
                }
                if(strcmp(type,"int")==0)
                {
                    if(memi[procid][b]<=0)
                    {
                        printc("log invalid parameter\n");
                        return -1;
                    }
                    memi[procid][c]=log(memi[procid][b]);
                }
                else if(strcmp(type,"char")==0)
                {
                    if(memc[procid][b]<=0)
                    {
                        printc("log invalid parameter\n");
                        return -1;
                    }
                    memc[procid][c]=log(memc[procid][b]);
                }
                else if(strcmp(type,"float")==0)
                {
                    if(memf[procid][b]<=0)
                    {
                        printc("log invalid parameter\n");
                        return -1;
                    }
                    memf[procid][c]=log(memf[procid][b]);
                }
                else
                {
                    printc("Type error\n");
                    return -1;
                }
            } else if(strcmp(mathcmd,"getpi")==0) {
                char type[8];
                int b;
                fscanf(fin[procid],"%s %d", type, &b);
                if(b<0||b>127)
                {
                    printc("Memory error\n");
                    return -1;
                }
                if(strcmp(type,"int")==0)
                {
                    memi[procid][b]=PI;
                }
                else if(strcmp(type,"char")==0)
                {
                    memc[procid][b]=PI;
                }
                else if(strcmp(type,"float")==0)
                {
                    memf[procid][b]=PI;
                }
                else
                {
                    printc("Type error\n");
                    return -1;
                }
            }
        }
    } else if (strcmp(_cmd, "clear") == 0) {
        system("cls");
    } else {
        printc("Invalid operation\n");
        return -1;
    }
    return 1;
}
int rbvm(int procid) {
    char command[128];/*
    printc("Rubidium Assembly on RbVM v1.11.4 Release 4\n");
    printc("By Lithium4141\n");
    printc("(C)2022~2025 Lithium Project LLC\n");*/
    fin[procid] = fopen(fileloc[procid], "r");
    if (fin[procid] == NULL) {
        printc("Cannot open file: %s\n", fileloc[procid]);
        return -2;
    }
    while (!feof(fin[procid])) {
        if (fscanf(fin[procid], "%s", command) != 1) {
            break;
        }
        int inte=getkey();
        if(inte==28)
        {
            printc("%s forcely terminated by Ctrl-\\",fileloc[procid]);
            return -1;
        }
        else if(inte==26)
        {
            printc("%s stopped by Ctrl-Z",fileloc[procid]);
        }
        sysClockTick++;
        int ret=run(procid,command);
        if (ret == 1) {
            continue;
        } else if (ret == 0) {
            //printc("RbVM Stopped\n");
            fclose(fin[procid]);
            return 0;
        } else if (ret == -1) {
            printc("RbVM Terminated by an error\n");
            fclose(fin[procid]);
            return -1;
        } else if (ret==-2){
            printc("%s forcely terminated by Ctrl-\\",fileloc[procid]);
            fclose(fin[procid]);
        } else if(ret==-3){
            printc("%s stopped by Ctrl-Z",fileloc[procid]);
            return 3;
        }
    }
    fclose(fin[procid]);
    return 0;
}
int getkey()
{
    if(kbhit())
    {
        return getch();
    }
    else
    {
        return -1;
    }
}
