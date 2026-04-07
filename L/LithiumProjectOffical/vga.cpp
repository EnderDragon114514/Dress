#include <graphics.h>
int main() {
	int gdriver = VGA, gmode = VGAHI;
        initgraph(&gdriver,&gmode,".\\BGI");
        //closegraph();
	return 0;
}
