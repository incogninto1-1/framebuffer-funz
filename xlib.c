 
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
 
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
 
static int
    width  = 640,
    height = 480;
 
static Atom
    wm_protocols,
    wm_delete_window;
 
static Display *display;
static int screen;
static Window window;
static Visual *visual;
static GC gc;
static XImage *img;


void blankFb();
void genImage();

int main(int argc, char **argv)
{
    XSetWindowAttributes wattr;
    XSizeHints size;
    XWMHints wm;
    XClassHint class;
    XTextProperty prop;
 
    display = XOpenDisplay(NULL);
    if (!display)
        exit(1);
 
    screen = DefaultScreen(display);
    visual = DefaultVisual(display, screen);
 
    bzero(&wattr, sizeof(wattr));
    wattr.backing_store = Always;
    wattr.event_mask = ExposureMask | StructureNotifyMask;
    wattr.background_pixel = WhitePixel(display, screen);
 
    window = XCreateWindow(display, DefaultRootWindow(display),
        0, 0, width, height,
        0,
        CopyFromParent, InputOutput, CopyFromParent,
        CWBackingStore | CWEventMask | CWBackPixel,
        &wattr);
 
    XMapWindow(display, window);
 
    size.flags = PMinSize | PMaxSize;
    size.min_width  = size.max_width  = width;
    size.min_height = size.max_height = height;
 
    wm.flags = InputHint | StateHint | WindowGroupHint | IconWindowHint;
    wm.input = True;
    wm.initial_state = NormalState;
    wm.window_group = window;
    wm.icon_window = None;
 
    char *cp = "demo";
    class.res_name  = "demo";
    class.res_class = "Demo";
 
    XStringListToTextProperty(&cp, 1, &prop);
 
    XSetWMProperties(display, window,
        &prop, &prop, argv, argc, &size, &wm, &class);
 
    wm_protocols     = XInternAtom(display, "WM_PROTOCOLS", False);
    wm_delete_window = XInternAtom(display, "WM_DELETE_WINDOW", False);
 
    XSetWMProtocols(display, window, &wm_delete_window, 1);
 
    gc = DefaultGC(display, screen);
 
    img = XCreateImage(display, visual, 24,
        ZPixmap, 0, malloc(width * height * 4), width, height, 32, width * 4);
 
    for (;;) {
        XEvent e;
 
        XNextEvent(display, &e);
 
        switch (e.xany.type) {
 
        case ClientMessage:
            exit(0);
 
        case ConfigureNotify:
            width  = e.xconfigure.width;
            height = e.xconfigure.height;
            break;
 
        case Expose:
            if (e.xexpose.count)
                break;
 
            /* fill window with random pixels */
 
	    genImage();

            XPutImage(display, window, gc, img, 0, 0, 0, 0, width, height);
            break;
        }
    }
}

void blankFb(){
	for (int y = 0; y < height; ++y)
		for (int x = 0; x < width; ++x)
			XPutPixel(img, x, y, 0 );
}


void genImage(){
	for(;;){
		blankFb();
		//put a pixel on the screen
		for(int yOff=0; yOff<256; yOff=yOff+5){
			for(int y = 30; y< 60; y++){
				for(int x=30; x < 60; x++){
					XPutPixel(img, x, (y+yOff), 255);
				}
			}
			
            		XPutImage(display, window, gc, img, 0, 0, 0, 0, width, height);
			blankFb();
		}
	}
}
