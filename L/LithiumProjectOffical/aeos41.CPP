#include <graphics.h>
#include <signal.h>
#include <conio.h>
#include <dos.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
char buf[256];
int menuon=0,demo=0;
int memi[128];
float memf[128];
char memc[128];
char mems[128][128];
FILE *fin;
int gdriver=DETECT;
int gmode;
void disable_ctrl_c() {
	union REGS regs;
	regs.h.ah = 0x33;
	regs.h.al = 0x01;
	regs.h.dl = 0x00;
	int86(0x21, &regs, &regs);
}
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
int condition(int type, int id_a, const char *_cmd, int id_b) {
	if (type == 1) {
		if (strcmp(_cmd, "equ") == 0) {
			return memi[id_a] == memi[id_b] ? 1 : 0;
		} else if (strcmp(_cmd, "grt") == 0) {
			return memi[id_a] > memi[id_b] ? 1 : 0;
		} else if (strcmp(_cmd, "les") == 0) {
			return memi[id_a] < memi[id_b] ? 1 : 0;
		} else if (strcmp(_cmd, "neq") == 0) {
			return memi[id_a] != memi[id_b] ? 1 : 0;
		} else {
			printf("Operation error\n");
			return -1;
		}
	} else if (type == 2) {
		if (strcmp(_cmd, "equ") == 0) {
			return memc[id_a] == memc[id_b] ? 1 : 0;
		} else if (strcmp(_cmd, "grt") == 0) {
			return memc[id_a] > memc[id_b] ? 1 : 0;
		} else if (strcmp(_cmd, "les") == 0) {
			return memc[id_a] < memc[id_b] ? 1 : 0;
		} else if (strcmp(_cmd, "neq") == 0) {
			return memc[id_a] != memc[id_b] ? 1 : 0;
		} else {
			printf("Operation error\n");
			return -1;
		}
	} else if (type == 3) {
		if (strcmp(_cmd, "equ") == 0) {
			return memf[id_a] == memf[id_b] ? 1 : 0;
		} else if (strcmp(_cmd, "grt") == 0) {
			return memf[id_a] > memf[id_b] ? 1 : 0;
		} else if (strcmp(_cmd, "les") == 0) {
			return memf[id_a] < memf[id_b] ? 1 : 0;
		} else if (strcmp(_cmd, "neq") == 0) {
			return memf[id_a] != memf[id_b] ? 1 : 0;
		} else {
			printf("Operation error\n");
			return -1;
		}
	} else {
		printf("Type error\n");
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
	while (ch != EOF && ch != '\n' && ch != '\r' && i < 127) {
		buffer[i++] = (char)ch;
		ch = fgetc(f);
	}
	buffer[i] = '\0';
}
int run_graphics(const char *_cmd) {
	char buffer[128];
	int a, b, c, d;
	if (strcmp(_cmd, "nop") == 0) {
		read_string(fin, buffer);
		return 1;
	} else if (strcmp(_cmd, "end") == 0) {
		return 0;
	} else if (strcmp(_cmd, "set") == 0) {
		fscanf(fin, "%d %d %d", &a, &b, &c);
		if (a < 0 || a > 127) {
			printf("Memory error\n");
			return -1;
		}
		if (c == 1) {
			memi[a] = b;
		} else if (c == 2) {
			memc[a] = (char)b;
		} else if (c == 3) {
			memf[a] = (float)b;
		} else {
			printf("Type error\n");
			return -1;
		}
	} else if (strcmp(_cmd, "copy") == 0) {
		fscanf(fin, "%d %d %d", &a, &b, &c);
		if (a < 0 || a > 127 || b < 0 || b > 127) {
			printf("Memory error\n");
			return -1;
		}
		if (c == 1) {
			memi[b] = memi[a];
		} else if (c == 2) {
			memc[b] = memc[a];
		} else if (c == 3) {
			memf[b] = memf[a];
		} else {
			printf("Type error\n");
			return -1;
		}
	} else if (strcmp(_cmd, "convert") == 0) {
		fscanf(fin, "%d %d %d %d", &a, &b, &c, &d);
		if (a < 0 || a > 127 || c < 0 || c > 127) {
			printf("Memory error\n");
			return -1;
		}
		if (b == 1) {
			if (d == 1) {
				memi[c] = memi[a];
			} else if (d == 2) {
				memc[c] = (char)memi[a];
			} else if (d == 3) {
				memf[c] = (float)memi[a];
			} else {
				printf("Type error\n");
				return -1;
			}
		} else if (b == 2) {
			if (d == 1) {
				memi[c] = (int)memc[a];
			} else if (d == 2) {
				memc[c] = memc[a];
			} else if (d == 3) {
				memf[c] = (float)memc[a];
			} else {
				printf("Type error\n");
				return -1;
			}
		} else if (b == 3) {
			if (d == 1) {
				memi[c] = (int)memf[a];
			} else if (d == 2) {
				memc[c] = (char)memf[a];
			} else if (d == 3) {
				memf[c] = memf[a];
			} else {
				printf("Type error\n");
				return -1;
			}
		} else {
			printf("Type error\n");
			return -1;
		}
	} else if (strcmp(_cmd, "plus") == 0) {
		fscanf(fin, "%d %d %d %d", &a, &b, &c, &d);
		if (a < 0 || a > 127 || b < 0 || b > 127 || c < 0 || c > 127) {
			printf("Memory error\n");
			return -1;
		}
		if (d == 1) {
			memi[c] = memi[a] + memi[b];
		} else if (d == 2) {
			memc[c] = memc[a] + memc[b];
		} else if (d == 3) {
			memf[c] = memf[a] + memf[b];
		} else {
			printf("Type error\n");
			return -1;
		}
	} else if (strcmp(_cmd, "minus") == 0) {
		fscanf(fin, "%d %d %d %d", &a, &b, &c, &d);
		if (a < 0 || a > 127 || b < 0 || b > 127 || c < 0 || c > 127) {
			printf("Memory error\n");
			return -1;
		}
		if (d == 1) {
			memi[c] = memi[a] - memi[b];
		} else if (d == 2) {
			memc[c] = memc[a] - memc[b];
		} else if (d == 3) {
			memf[c] = memf[a] - memf[b];
		} else {
			printf("Type error\n");
			return -1;
		}
	} else if (strcmp(_cmd, "times") == 0) {
		fscanf(fin, "%d %d %d %d", &a, &b, &c, &d);
		if (a < 0 || a > 127 || b < 0 || b > 127 || c < 0 || c > 127) {
			printf("Memory error\n");
			return -1;
		}
		if (d == 1) {
			memi[c] = memi[a] * memi[b];
		} else if (d == 2) {
			memc[c] = memc[a] * memc[b];
		} else if (d == 3) {
			memf[c] = memf[a] * memf[b];
		} else {
			printf("Type error\n");
			return -1;
		}
	} else if (strcmp(_cmd, "divide") == 0) {
		fscanf(fin, "%d %d %d %d", &a, &b, &c, &d);
		if (a < 0 || a > 127 || b < 0 || b > 127 || c < 0 || c > 127) {
			printf("Memory error\n");
			return -1;
		}
		if (d == 1) {
			if (memi[b] != 0) {
				memi[c] = memi[a] / memi[b];
			} else {
				printf("Divide by zero error\n");
				return -1;
			}
		} else if (d == 2) {
			if (memc[b] != 0) {
				memc[c] = memc[a] / memc[b];
			} else {
				printf("Divide by zero error\n");
				return -1;
			}
		} else if (d == 3) {
			if (memf[b] != 0.0f) {
				memf[c] = memf[a] / memf[b];
			} else {
				printf("Divide by zero error\n");
				return -1;
			}
		} else {
			printf("Type error\n");
			return -1;
		}
	} else if (strcmp(_cmd, "mod") == 0) {
		fscanf(fin, "%d %d %d", &a, &b, &c);
		if (a < 0 || a > 127 || b < 0 || b > 127 || c < 0 || c > 127) {
			printf("Memory error\n");
			return -1;
		}
		if (memi[b] != 0) {
			memi[c] = memi[a] % memi[b];
		} else {
			printf("Mod by zero error\n");
			return -1;
		}
	} else if(strcmp(_cmd,"graph")==0) {
		char op[32];
		int a,b;
		int currentcolor=WHITE;
		fscanf(fin,"%s",op);
		if(strcmp(op,"drawpixel")==0) {
			fscanf(fin,"%d %d",&a,&b);
			putpixel(clamp(a,0,640),clamp(b,115,400),currentcolor);
		} else if(strcmp(op,"drawline")==0) {
			int c, d;
			fscanf(fin,"%d %d %d %d",&a,&b,&c,&d);
			line(clamp(a,0,640),clamp(b,115,400),clamp(c,0,640),clamp(d,115,400));
		} else if(strcmp(op,"drawrect")==0) {
			int c, d;
			fscanf(fin,"%d %d %d %d",&a,&b,&c,&d);
			rectangle(clamp(a,0,640),clamp(b,115,400),clamp(c,0,640),clamp(d,115,400));
		} else if(strcmp(op,"drawcirc")==0) {
			int c;
			fscanf(fin,"%d %d %d",&a,&b,&c);
			circle(clamp(a,0,640),clamp(b,115,400),clamp(c,115,400));
		} else if(strcmp(op,"fillbar")==0) {
			fscanf(fin,"%d,%d",&a,&b);
			floodfill(clamp(a,1,639),clamp(b,116,399),currentcolor);
		} else if(strcmp(op,"drawtext")==0) {
			int c,d,e,f;
			fscanf(fin,"%d %d %d %d",&c,&d,&e,&f);
			if(c<128||c>255) {
				printf("Memory error\n");
				return -1;
			}
			settextstyle(TRIPLEX_FONT,HORIZ_DIR,clamp(d,1,9));
			outtextxy(clamp(e,0,640),clamp(f,115,400),mems[c]);
			settextstyle(TRIPLEX_FONT,HORIZ_DIR,4);
		} else if(strcmp(op,"clearall")==0) {
			cleardevice();
		} else {
			printf("Operation error\n");
		}
	} else if (strcmp(_cmd, "sset") == 0) {
		fscanf(fin, "%d", &a);
		read_string(fin, buffer);
		if (a < 128 || a > 255) {
			printf("Memory error\n");
			return -1;
		}
		a -= 128;
		strncpy(mems[a], buffer, 127);
		mems[a][127] = '\0';
	} else if (strcmp(_cmd, "scopy") == 0) {
		fscanf(fin, "%d %d", &a, &b);
		if (a < 128 || a > 255 || b < 128 || b > 255) {
			printf("Memory error\n");
			return -1;
		}
		a -= 128;
		b -= 128;
		strncpy(mems[b], mems[a], 127);
		mems[b][127] = '\0';
	} else if (strcmp(_cmd, "splus") == 0) {
		fscanf(fin, "%d %d %d", &a, &b, &c);
		if (a < 128 || a > 255 || b < 128 || b > 255 || c < 128 || c > 255) {
			printf("Memory error\n");
			return -1;
		}
		a -= 128;
		b -= 128;
		c -= 128;
		strncpy(mems[c], mems[a], 127);
		strncat(mems[c], mems[b], 127 - strlen(mems[c]));
		mems[c][127] = '\0';
	} else if (strcmp(_cmd, "ssize") == 0) {
		fscanf(fin, "%d %d", &a, &b);
		if (a < 128 || a > 255 || b < 0 || b > 127) {
			printf("Memory error\n");
			return -1;
		}
		a -= 128;
		memi[b] = strlen(mems[a]);
	} else if (strcmp(_cmd, "if") == 0) {
		int type, id_a, id_b;
		char op[10], cmd1[128], cmd2[128];
		fscanf(fin, "%d %d %s %d", &type, &id_a, op, &id_b);
		if (id_a < 0 || id_a > 127 || id_b < 0 || id_b > 127) {
			printf("Memory error\n");
			return -1;
		}
		int ret = condition(type, id_a, op, id_b);
		if (ret == -1) {
			return -1;
		} else if (ret == 1) {
			fscanf(fin, "%s", cmd1);
			return run_graphics(cmd1);
		} else if (ret == 0) {
			fscanf(fin, "%s", cmd2);
			return run_graphics(cmd2);
		} else {
			printf("Unknown error\n");
			return -1;
		}
	} else if (strcmp(_cmd, "while") == 0) {
		int type, id_a, id_b;
		char op[10], cmd[128];
		fscanf(fin, "%d %d %s %d %s", &type, &id_a, op, &id_b, cmd);
		if (id_a < 0 || id_a > 127 || id_b < 0 || id_b > 127) {
			printf("Memory error\n");
			return -1;
		}
		while (1) {
			int con = condition(type, id_a, op, id_b);
			if (con == -1) {
				return -1;
			} else if (con == 0) {
				break;
			} else if (con == 1) {
				int ret = run_graphics(cmd);
				if (ret == -1) {
					return -1;
				} else if (ret == 0) {
					return 0;
				} else if (ret == 1) {
					continue;
				}
			}
		}
	} else if(strcmp(_cmd,"getkey")==0) {
		int id_a;
		fscanf(fin,"%d",id_a);
		if(id_a<0||id_a>127) {
			printf("Memory error\n");
			return -1;
		}
		memc[id_a]=getch();
	} else {
		printf("Invalid operation\n");
		return -1;
	}
	return 1;
}

int run_console(const char *_cmd) {
	char buffer[128];
	int a, b, c, d;
	if (strcmp(_cmd, "nop") == 0) {
		read_string(fin, buffer);
		return 1;
	} else if (strcmp(_cmd, "end") == 0) {
		return 0;
	} else if (strcmp(_cmd, "set") == 0) {
		fscanf(fin, "%d %d %d", &a, &b, &c);
		if (a < 0 || a > 127) {
			printf("Memory error\n");
			return -1;
		}
		if (c == 1) {
			memi[a] = b;
		} else if (c == 2) {
			memc[a] = (char)b;
		} else if (c == 3) {
			memf[a] = (float)b;
		} else {
			printf("Type error\n");
			return -1;
		}
	} else if (strcmp(_cmd, "copy") == 0) {
		fscanf(fin, "%d %d %d", &a, &b, &c);
		if (a < 0 || a > 127 || b < 0 || b > 127) {
			printf("Memory error\n");
			return -1;
		}
		if (c == 1) {
			memi[b] = memi[a];
		} else if (c == 2) {
			memc[b] = memc[a];
		} else if (c == 3) {
			memf[b] = memf[a];
		} else {
			printf("Type error\n");
			return -1;
		}
	} else if (strcmp(_cmd, "convert") == 0) {
		fscanf(fin, "%d %d %d %d", &a, &b, &c, &d);
		if (a < 0 || a > 127 || c < 0 || c > 127) {
			printf("Memory error\n");
			return -1;
		}
		if (b == 1) {
			if (d == 1) {
				memi[c] = memi[a];
			} else if (d == 2) {
				memc[c] = (char)memi[a];
			} else if (d == 3) {
				memf[c] = (float)memi[a];
			} else {
				printf("Type error\n");
				return -1;
			}
		} else if (b == 2) {
			if (d == 1) {
				memi[c] = (int)memc[a];
			} else if (d == 2) {
				memc[c] = memc[a];
			} else if (d == 3) {
				memf[c] = (float)memc[a];
			} else {
				printf("Type error\n");
				return -1;
			}
		} else if (b == 3) {
			if (d == 1) {
				memi[c] = (int)memf[a];
			} else if (d == 2) {
				memc[c] = (char)memf[a];
			} else if (d == 3) {
				memf[c] = memf[a];
			} else {
				printf("Type error\n");
				return -1;
			}
		} else {
			printf("Type error\n");
			return -1;
		}
	} else if (strcmp(_cmd, "plus") == 0) {
		fscanf(fin, "%d %d %d %d", &a, &b, &c, &d);
		if (a < 0 || a > 127 || b < 0 || b > 127 || c < 0 || c > 127) {
			printf("Memory error\n");
			return -1;
		}
		if (d == 1) {
			memi[c] = memi[a] + memi[b];
		} else if (d == 2) {
			memc[c] = memc[a] + memc[b];
		} else if (d == 3) {
			memf[c] = memf[a] + memf[b];
		} else {
			printf("Type error\n");
			return -1;
		}
	} else if (strcmp(_cmd, "minus") == 0) {
		fscanf(fin, "%d %d %d %d", &a, &b, &c, &d);
		if (a < 0 || a > 127 || b < 0 || b > 127 || c < 0 || c > 127) {
			printf("Memory error\n");
			return -1;
		}
		if (d == 1) {
			memi[c] = memi[a] - memi[b];
		} else if (d == 2) {
			memc[c] = memc[a] - memc[b];
		} else if (d == 3) {
			memf[c] = memf[a] - memf[b];
		} else {
			printf("Type error\n");
			return -1;
		}
	} else if (strcmp(_cmd, "times") == 0) {
		fscanf(fin, "%d %d %d %d", &a, &b, &c, &d);
		if (a < 0 || a > 127 || b < 0 || b > 127 || c < 0 || c > 127) {
			printf("Memory error\n");
			return -1;
		}
		if (d == 1) {
			memi[c] = memi[a] * memi[b];
		} else if (d == 2) {
			memc[c] = memc[a] * memc[b];
		} else if (d == 3) {
			memf[c] = memf[a] * memf[b];
		} else {
			printf("Type error\n");
			return -1;
		}
	} else if (strcmp(_cmd, "divide") == 0) {
		fscanf(fin, "%d %d %d %d", &a, &b, &c, &d);
		if (a < 0 || a > 127 || b < 0 || b > 127 || c < 0 || c > 127) {
			printf("Memory error\n");
			return -1;
		}
		if (d == 1) {
			if (memi[b] != 0) {
				memi[c] = memi[a] / memi[b];
			} else {
				printf("Divide by zero error\n");
				return -1;
			}
		} else if (d == 2) {
			if (memc[b] != 0) {
				memc[c] = memc[a] / memc[b];
			} else {
				printf("Divide by zero error\n");
				return -1;
			}
		} else if (d == 3) {
			if (memf[b] != 0.0f) {
				memf[c] = memf[a] / memf[b];
			} else {
				printf("Divide by zero error\n");
				return -1;
			}
		} else {
			printf("Type error\n");
			return -1;
		}
	} else if (strcmp(_cmd, "mod") == 0) {
		fscanf(fin, "%d %d %d", &a, &b, &c);
		if (a < 0 || a > 127 || b < 0 || b > 127 || c < 0 || c > 127) {
			printf("Memory error\n");
			return -1;
		}
		if (memi[b] != 0) {
			memi[c] = memi[a] % memi[b];
		} else {
			printf("Mod by zero error\n");
			return -1;
		}
	} else if (strcmp(_cmd, "get") == 0) {
		fscanf(fin, "%d %d", &a, &b);
		if (a < 0 || a > 127) {
			printf("Memory error\n");
			return -1;
		}
		if (b == 1) {
			scanf("%d", &memi[a]);
		} else if (b == 2) {
			scanf(" %c", &memc[a]);
		} else if (b == 3) {
			scanf("%f", &memf[a]);
		} else {
			printf("Type error\n");
			return -1;
		}
	} else if (strcmp(_cmd, "put") == 0) {
		fscanf(fin, "%d %d", &a, &b);
		if (a < 0 || a > 127) {
			printf("Memory error\n");
			return -1;
		}
		if (b == 1) {
			printf("%d\n", memi[a]);
		} else if (b == 2) {
			printf("%c\n", memc[a]);
		} else if (b == 3) {
			printf("%f\n", memf[a]);
		} else {
			printf("Type error\n");
			return -1;
		}
	} else if (strcmp(_cmd, "sset") == 0) {
		fscanf(fin, "%d", &a);
		read_string(fin, buffer);
		if (a < 128 || a > 255) {
			printf("Memory error\n");
			return -1;
		}
		a -= 128;
		strncpy(mems[a], buffer, 127);
		mems[a][127] = '\0';
	} else if (strcmp(_cmd, "scopy") == 0) {
		fscanf(fin, "%d %d", &a, &b);
		if (a < 128 || a > 255 || b < 128 || b > 255) {
			printf("Memory error\n");
			return -1;
		}
		a -= 128;
		b -= 128;
		strncpy(mems[b], mems[a], 127);
		mems[b][127] = '\0';
	} else if (strcmp(_cmd, "splus") == 0) {
		fscanf(fin, "%d %d %d", &a, &b, &c);
		if (a < 128 || a > 255 || b < 128 || b > 255 || c < 128 || c > 255) {
			printf("Memory error\n");
			return -1;
		}
		a -= 128;
		b -= 128;
		c -= 128;
		strncpy(mems[c], mems[a], 127);
		strncat(mems[c], mems[b], 127 - strlen(mems[c]));
		mems[c][127] = '\0';
	} else if (strcmp(_cmd, "sget") == 0) {
		fscanf(fin, "%d", &a);
		if (a < 128 || a > 255) {
			printf("Memory error\n");
			return -1;
		}
		a -= 128;
		fflush(stdin);
		fgets(mems[a], 127, stdin);
		mems[a][strcspn(mems[a], "\r\n")] = '\0';
	} else if (strcmp(_cmd, "sput") == 0) {
		fscanf(fin, "%d", &a);
		if (a < 128 || a > 255) {
			printf("Memory error\n");
			return -1;
		}
		a -= 128;
		printf("%s\n", mems[a]);
	} else if (strcmp(_cmd, "ssize") == 0) {
		fscanf(fin, "%d %d", &a, &b);
		if (a < 128 || a > 255 || b < 0 || b > 127) {
			printf("Memory error\n");
			return -1;
		}
		a -= 128;
		memi[b] = strlen(mems[a]);
	} else if (strcmp(_cmd, "if") == 0) {
		int type, id_a, id_b;
		char op[10], cmd1[128], cmd2[128];
		fscanf(fin, "%d %d %s %d", &type, &id_a, op, &id_b);
		if (id_a < 0 || id_a > 127 || id_b < 0 || id_b > 127) {
			printf("Memory error\n");
			return -1;
		}
		int ret = condition(type, id_a, op, id_b);
		if (ret == -1) {
			return -1;
		} else if (ret == 1) {
			fscanf(fin, "%s", cmd1);
			return run_console(cmd1);
		} else if (ret == 0) {
			fscanf(fin, "%s", cmd2);
			return run_console(cmd2);
		} else {
			printf("Unknown error\n");
			return -1;
		}
	} else if (strcmp(_cmd, "while") == 0) {
		int type, id_a, id_b;
		char op[10], cmd[128];
		fscanf(fin, "%d %d %s %d %s", &type, &id_a, op, &id_b, cmd);
		if (id_a < 0 || id_a > 127 || id_b < 0 || id_b > 127) {
			printf("Memory error\n");
			return -1;
		}
		while (1) {
			int con = condition(type, id_a, op, id_b);
			if (con == -1) {
				return -1;
			} else if (con == 0) {
				break;
			} else if (con == 1) {
				int ret = run_console(cmd);
				if (ret == -1) {
					return -1;
				} else if (ret == 0) {
					return 0;
				} else if (ret == 1) {
					continue;
				}
			}
		}
	} else if(strcmp(_cmd,"getkey")==0) {
		int id_a;
		fscanf(fin,"%d",&id_a);
		if(id_a<0||id_a>127) {
			printf("Memory error\n");
			return -1;
		}
		memc[id_a]=getch();
	} else if (strcmp(_cmd, "clear") == 0) {
		system("cls");
	} else {
		printf("Invalid operation\n");
		return -1;
	}
	return 1;
}
int rbvm(char fileloc[64]) {
	char command[128];
	printf("Rubidium Assembly on RbVM v0.1.1 Release 3\n");
	printf("By Lithium4141\n");
	printf("(C)2022~2025 Lithium Project LLC\n");
	fin = fopen(fileloc, "r");
	if (fin == NULL) {
		printf("Cannot open file: %s\n", fileloc);
		return -2;
	}
	char type;
	fscanf(fin,"TYPEDEF %c",&type);
	if(type=='c') {
	} else if(type=='g') {
	} else {
		printf("Argument error\n");
		printf("Unknown program type!\n");
		return -1;
	}
	while (!feof(fin)) {
		if (fscanf(fin, "%s", command) != 1) {
			break;
		}
		int ret=0;
		if(type=='g') {
			initgraph(&gdriver,&gmode,"A:\\BGI");
			ret = run_graphics(command);
		} else {
			ret = run_console(command);
		}
		if (ret == 1) {
			continue;
		} else if (ret == 0) {
			printf("RbVM Stopped\n");
			fclose(fin);
			return 0;
		} else if (ret == -1) {
			printf("RbVM Terminated by an error\n");
			fclose(fin);
			return -1;
		}
	}
	fclose(fin);
	return 0;
}
void thickrectangle(int x1, int y1, int x2, int y2, int thickness) {
	setcolor(WHITE);
	for (int i = 0; i < thickness; i++) {
		rectangle(x1 + i, y1 + i, x2 - i, y2 - i);
	}
}
void shutdownacpi(int sORr) {
	union REGS regs;
	regs.x.ax = 0x5301;
	regs.x.bx = 0x0000;
	int86(0x15, &regs, &regs);
	regs.x.ax = 0x5300;
	regs.x.bx = 0x0000;
	int86(0x15, &regs, &regs);
	regs.x.ax = 0x5307;
	regs.x.bx = 0x0001;
	if(sORr==0) {
		regs.x.cx = 0x0003;
	} else {
		regs.x.cx=0x0002;
	}
	int86(0x15, &regs, &regs);
}
void shutdownext(int sORr) {
	if(sORr==0) {
		system("shutdown s");
	} else {
		system("shutdown r");
	}
}
void clearall() {
	for(int i=0; i<=640; i++) {
		for(int j=0; j<=480; j++) {
			putpixel(i,j,BLACK);
		}
	}
}
void repaintdesktop() {
	cleardevice();
	for(int i=0; i<=640; i++) {
		for(int j=40; j<=50; j++) {
			putpixel(i,j,WHITE);
		}
	}
	settextstyle(TRIPLEX_FONT,HORIZ_DIR,4);
	setcolor(WHITE);
	outtextxy(0,0,"AEOS v4.1.1-rc1 BGI");
	if(demo==1) {
		settextstyle(TRIPLEX_FONT,HORIZ_DIR,1);
		outtextxy(530,370,"AEOS DEMO");
		outtextxy(0,370,"AEOS DEMO");
		outtextxy(530,60,"AEOS DEMO");
		outtextxy(0,60,"AEOS DEMO");
	}
	settextstyle(TRIPLEX_FONT,HORIZ_DIR,4);
	for(i=0; i<=640; i++) {
		for(int j=400; j<=410; j++) {
			putpixel(i,j,WHITE);
		}
	}
	setcolor(WHITE);
	for(i=420; i<=423; i++) {
		line(20,i,60,i);
	}
	for(i=426; i<=429; i++) {
		line(20,i,60,i);
	}
	for(i=432; i<=435; i++) {
		line(20,i,60,i);
	}
}
int main() {
	disable_ctrl_c();
	system("mkdir ROOTFS");
	printf("[INFO]Initilazing kernel...\n");
	delay(1000);
	printf("[INFO]Kernel initilazation success!\n");
	printf("[INFO]Initilazing logger...\n");
	printf("[INFO]Logger initilazition success!\n");
	system("echo [INFO]Logger initilazation success! > latest.log");
	printf("[INFO]Initilazing BGI driver...\n");
	system("echo [INFO]Initilazing BGI driver... >> latest.log");
	initgraph(&gdriver,&gmode,"A:\\BGI");
	int graphstatus=graphresult();
	if(graphstatus!=grOk) {
		closegraph();
		printf("BGI driver initilazation failed!Error:%s\n",grapherrormsg(graphstatus));
		sprintf(buf,"echo [FATAL]Initilazation failed!Error:%s >> latest.log",grapherrormsg(graphstatus));
		system(buf);
		printf("Downgrading to AEOS 4.1 CUI...\n");
		system("echo Downgrading to AEOS 4.1 CUI... >> latest.log");
		system("echo Restarting logger... >> latest.log");
		system("copy latest.log latest.log.old");
		return system("INIT.EXE");
	}
	while(1) {
	mainloop:
		cleardevice();
		for(int i=0; i<=640; i++) {
			for(int j=40; j<=50; j++) {
				putpixel(i,j,WHITE);
			}
		}
		settextstyle(TRIPLEX_FONT,HORIZ_DIR,4);
		setcolor(WHITE);
		outtextxy(0,0,"AEOS v4.1.1-rc1 BGI");
		if(demo==1) {
			settextstyle(TRIPLEX_FONT,HORIZ_DIR,1);
			outtextxy(530,370,"AEOS DEMO");
			outtextxy(0,370,"AEOS DEMO");
			outtextxy(530,60,"AEOS DEMO");
			outtextxy(0,60,"AEOS DEMO");
		}
		settextstyle(TRIPLEX_FONT,HORIZ_DIR,4);
		for(i=0; i<=640; i++) {
			for(int j=400; j<=410; j++) {
				putpixel(i,j,WHITE);
			}
		}
		setcolor(WHITE);
		for(i=420; i<=423; i++) {
			line(20,i,60,i);
		}
		for(i=426; i<=429; i++) {
			line(20,i,60,i);
		}
		for(i=432; i<=435; i++) {
			line(20,i,60,i);
		}
		int key=getch();
		if(key==0) {
			system("echo User triggered menu >> latest.log");
			menuon=1;
			thickrectangle(10,400,150,150,5);
			setfillstyle(SOLID_FILL, BLACK);
			setcolor(BLACK);
                        bar(20,395,145,155);
			setcolor(WHITE);
			settextstyle(TRIPLEX_FONT,HORIZ_DIR,2);
			outtextxy(20,150,"AEOS 4.1 BGI");
			line(10,200,150,200);
			line(10,205,150,205);
			outtextxy(20,210,"Terminal");
			line(10,250,150,250);
			//Other menu items
			line(10,350,150,350);
			outtextxy(20,350,"Power");
			while(menuon==1) {
				getch();
				key=getch();
				if(key=='T'||key=='t') {
					int terminalrun=1;
					closegraph();
					system("cls");
					printf("AEOS v4.1 BGI TTySimulator\n");
					printf("--------------------------------\n");
					printf("Now you are in AEOS TTyTerminal,Type 'help' for help\n");
					char directory[128]="\\ROOTFS",command[64],extexecbuf[64];
					while(terminalrun==1) {
						printf("%s>",directory);
						scanf("%s",command);
						if(strcmp(command,"dir")==0) {
							sprintf(extexecbuf,"dir %s",directory);
							system(extexecbuf);
						} else if(strcmp(command,"cd")==0) {
							scanf("%s",extexecbuf);
							char buf1[65];
							strcpy(buf1,"\\");
							strcat(directory,strcat(buf1,extexecbuf));
						} else if(strcmp(command,"help")==0) {
							printf("Available commands:\n");
							printf("<dir> Show files in current directory\n");
							printf("<cd> Change current directory\n");
							printf("<help> Display this page\n");
							printf("<about> Show Current Version\n");
							printf("<ver> Show Operating System Build\n");
							printf("<exit> Exit terminal\n");
							printf("<clear> Clear console\n");
							printf("<proc> Process manager\n");
							printf("<exec> Execute program in current directory\n");
							printf("Note:EXEC can only execute RBA files!\n");
							printf("     To return back to AEOS,type 'exit'\n");
						} else if(strcmp(command,"about")==0) {
							printf("AEOS TTySimulator v1.7.5\n");
							printf("(C)Copyright 2022~2025 Lithium Project LLC\n");
						} else if(strcmp(command,"ver")==0) {
							printf("AEOS v4.1.1 rc-1 BGI build 1123\n");
							printf("Release date:2025/11/23\n");
							printf("(C)Copyright 2022~2025 Lithium Project LLC\n");
						} else if(strcmp(command,"exit")==0) {
							terminalrun=0;
						} else if(strcmp(command,"clear")==0) {
							system("cls");
						} else if(strcmp(command,"proc")==0) {
							scanf("%s",command);
							if(strcmp(command,"list")==0) {
								printf("Active Progcess List:\n");
								printf("PROC      | GROUP | PID\n");
								printf("INIT.EXE  |  SU   |  1\n");
								printf("AEOS41.EXE|  SU   |  2\n");
								printf("RBVM.EXE  | SEVC  | 1000\n");
								printf("BGI.EXE   | USER  | 1001\n");
								printf("TTYSIM.EXE| USER  | 1002\n");
								printf("-------------------------\n");
								printf("MEMORY STAT:\n");
								system("mem.exe");
							} else if(strcmp(command,"killproc")==0) {
								scanf("%s",command);
								if(strcmp(command,"INIT.EXE")==0) {
									printf("ERROR:KERNEL PANIC!\n");
									system("echo [EMERG]KERNEL PROCESS KILLED! >> latest.log");
									printf("SYSTEM HALTED!\n");
									system("echo [FATAL ERROR]SYSTEM HALTED! >> latest.log");
									printf("PRESS CTRL-ALT-DELETE OR CTRL-\\ TO REBOOT!");
									while(1) {
										if(getch()==28) {
											system("echo [INFO]REBOOTING! >> latest.log");
											system("echo Logger is stopping... >> latest.log");
											system("copy latest.log latest.log.old");
											system("shutdown r");
										}
									}
								} else if(strcmp(command,"AEOS41.EXE")==0) {
									printf("ERROR:KERNEL PANIC!\n");
									system("echo [EMERG]KERNEL PROCESS KILLED! >> latest.log");
									printf("SYSTEM HALTED!\n");
									system("echo [FATAL ERROR]SYSTEM HALTED! >> latest.log");
									printf("PRESS CTRL-ALT-DELETE OR CTRL-\\ TO REBOOT!");
									while(1) {
										if(getch()==28) {
											system("echo [INFO]REBOOTING! >> latest.log");
											system("echo Logger is stopping... >> latest.log");
											system("copy latest.log latest.log.old");
											system("shutdown r");
										}
									}
								} else if(strcmp(command,"RBVM.EXE")==0) {
									printf("ERROR:RbVM Service Not Avaliable!\n");
									system("echo [ERROR]RbVM Service Failed! >> latest.log");
									printf("Press Ctrl-\\ to reload service, or no process can keep executing!\n");
									system("echo [FATAL ERROR]All Process Killed! >> latest.log");
									while(1) {
										if(getch()==28) {
											system("echo [INFO]RELOADING! >> latest.log");
											goto end;
										}
									}
								} else if(strcmp(command,"BGI.EXE")==0) {
									printf("ERROR:Process Denied!Rason:Process Handler Occupied by:RBVM.EXE,AEOS41.EXE\n");
									system("echo [ERROR]Process Denied!Rason:Process Handler Occupied by:RBVM.EXE,AEOS41.EXE >> latest.log");
								} else if(strcmp(command,"TTYSIM.EXE")==0) {
									terminalrun=0;
								} else {
									printf("Unknown process:%s\n",command);
								}
							} else if(strcmp(command,"killpid")==0) {
								scanf("%s",command);
								if(strcmp(command,"1")==0) {
									printf("ERROR:KERNEL PANIC!\n");
									system("echo [EMERG]KERNEL PROCESS KILLED! >> latest.log");
									printf("SYSTEM HALTED!\n");
									system("echo [FATAL ERROR]SYSTEM HALTED! >> latest.log");
									printf("PRESS CTRL-ALT-DELETE OR CTRL-\\ TO REBOOT!");
									while(1) {
										if(getch()==28) {
											system("echo [INFO]REBOOTING! >> latest.log");
											system("echo Logger is stopping... >> latest.log");
											system("copy latest.log latest.log.old");
											system("shutdown r");
										}
									}
								} else if(strcmp(command,"2")==0) {
									printf("ERROR:KERNEL PANIC!\n");
									system("echo [EMERG]KERNEL PROCESS KILLED! >> latest.log");
									printf("SYSTEM HALTED!\n");
									system("echo [FATAL ERROR]SYSTEM HALTED! >> latest.log");
									printf("PRESS CTRL-ALT-DELETE OR CTRL-\\ TO REBOOT!");
									while(1) {
										if(getch()==28) {
											system("echo [INFO]REBOOTING! >> latest.log");
											system("echo Logger is stopping... >> latest.log");
											system("copy latest.log latest.log.old");
											system("shutdown r");
										}
									}
								} else if(strcmp(command,"1000")==0) {
									printf("ERROR:RbVM Service Not Avaliable!\n");
									system("echo [ERROR]RbVM Service Failed! >> latest.log");
									printf("Press Ctrl-\\ to reload service, or no process can keep executing!\n");
									system("echo [FATAL ERROR]All Process Killed! >> latest.log");
									while(1) {
										if(getch()==28) {
											system("echo [INFO]RELOADING! >> latest.log");
											goto end;
										}
									}
								} else if(strcmp(command,"1001")==0) {
									printf("ERROR:Process Denied!Rason:Process Handler Occupied by:RBVM.EXE,AEOS41.EXE\n");
									system("echo [ERROR]Process Denied!Rason:Process Handler Occupied by:RBVM.EXE,AEOS41.EXE >> latest.log");
								} else if(strcmp(command,"1002")==0) {
									terminalrun=0;
								} else {
									printf("Unknown PID:%s\n",command);
								}
							} else {
								printf("Error:Invalid parameter: %s",command);
								printf("Usage:proc [list,killproc,killpid] [proc,pid]\n");
							}
						} else if(strcmp(command,"exec")==0) {
							scanf("%s",command);
							int ret=rbvm(command);
							printf("Program exited with value %d\n",ret);
						} else {
							printf("Error:Unknown command %s\n",command);
						}
					}
					initgraph(&gdriver,&gmode,"A:\\BGI");
				} else if(key=='P'||key=='p') {
					thickrectangle(100,95,410,200,5);
					setcolor(BLACK);
					setfillstyle(SOLID_FILL, BLACK);
                                        bar(105,100,405,195);
					setcolor(WHITE);
					outtextxy(100,100,"Shutdown");
					outtextxy(100,150,"S(hutdown),R(estart),C(ancel)?");
					system("echo User triggered power menu >> latest.log");
					key=getch();
					if(key=='S'||key=='s') {
						system("echo User requested shutdown >> latest.log");
						system("echo Stopping BGI... >> latest.log");
						closegraph();
						system("echo Killing all process... >> latest.log");
						printf("Killing all process...\n");
						delay(700);
						system("echo Stopping logger... >> latest.log");
						printf("Stopping logger...\n");
						system("copy latest.log latest.log.old");
						printf("Sending ACPI Signal...\n");
						shutdownext(0);
					} else if(key=='R'||key=='r') {
						system("echo User requested restart >> latest.log");
						system("echo Stopping BGI... >> latest.log");
						closegraph();
						system("echo Killing all process... >> latest.log");
						printf("Killing all process...\n");
						delay(700);
						system("echo Stopping logger... >> latest.log");
						printf("Stopping logger...\n");
						system("copy latest.log latest.log.old");
						printf("Sending ACPI Signal...\n");
						shutdownext(1);
					}
				} else {
					menuon=0;
					goto mainloop;
					break;
				}
			}
		}
		if(key==28) {
			settextstyle(TRIPLEX_FONT,HORIZ_DIR,4);
			outtextxy(0,0,"PRESS CTRL+\\ AGAIN TO FORCE RELOAD...");
			system("echo FRL Menu Triggered >> latest.log");
			key=getch();
			if(key==28) {
				system("echo User requested FRL >> latest.log");
				goto end;
			}
			system("echo User cancelled FRL >> latest.log");
		}
	}
end:
	closegraph();
	system("echo Logger is stopping... >> latest.log");
	system("copy latest.log latest.log.old");
	return 0;
}
