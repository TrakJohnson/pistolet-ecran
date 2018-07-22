#include<iostream>
#include <unistd.h>
#include<X11/Xlib.h>
#include<X11/cursorfont.h>

using namespace std;

int main() {
    int rx, ry, rw, rh = 0;
    int rect_x, rect_y, rect_w, rect_h = 0;
    int btn_pressed = 0, done = 0;

    XEvent ev;
    Display *disp = XOpenDisplay(NULL);

    if(!disp) {
	return EXIT_FAILURE;
    }

    Screen *scr = ScreenOfDisplay(disp, DefaultScreen(disp));
    Window root = RootWindow(disp, XScreenNumberOfScreen(scr));

    Cursor cursor = XCreateFontCursor(disp, XC_crosshair);

    XGCValues gcval;
    gcval.foreground = XWhitePixel(disp, 0);
    gcval.function = GXxor;
    gcval.background = XBlackPixel(disp, 0);
    gcval.plane_mask = gcval.background ^ gcval.foreground;
    gcval.subwindow_mode = IncludeInferiors;

    GC gc;
    gc = XCreateGC(disp, root,
		   GCFunction | GCForeground | GCBackground | GCSubwindowMode,
		   &gcval);

    /* this XGrab* stuff makes XPending true ? */
    if (XGrabPointer(disp, root, False, ButtonMotionMask | ButtonPressMask | ButtonReleaseMask, GrabModeAsync,
	  GrabModeAsync, root, cursor, CurrentTime) != GrabSuccess)
	printf("couldn't grab pointer:");

    if (XGrabKeyboard(disp, root, False, GrabModeAsync,
		      GrabModeAsync, CurrentTime) != GrabSuccess) {
	cerr << "Couldn't grab keyboard" << endl;
    }

    // see also: http://stackoverflow.com/questions/19659486/xpending-cycle-is-making-cpu-100
    while (!done) {
	//~ while (!done && XPending(disp)) {
	//~ XNextEvent(disp, &ev);
	if (!XPending(disp)) { usleep(10); continue; } // fixes the 100% CPU hog issue in original code
	if ( (XNextEvent(disp, &ev) >= 0) ) {
	    switch (ev.type) {
	    case MotionNotify:
		/* this case is purely for drawing rect on screen */
		if (btn_pressed) {
		    if (rect_w) {
			/* re-draw the last rect to clear it */
			printf("Redrawing rectangle\n");
			XDrawRectangle(disp, root, gc, rect_x, rect_y, rect_w, rect_h);
		    }
		    rect_x = rx;
		    rect_y = ry;
		    rect_w = ev.xmotion.x - rect_x;
		    rect_h = ev.xmotion.y - rect_y;

		    if (rect_w < 0) {
			rect_x += rect_w;
			rect_w = 0 - rect_w;
		    }
		    if (rect_h < 0) {
			rect_y += rect_h;
			rect_h = 0 - rect_h;
		    }
		    /* draw rectangle */
		    XDrawRectangle(disp, root, gc, rect_x, rect_y, rect_w, rect_h);
		    // XFlush(disp);
		}
		break;
	    case ButtonPress:
		btn_pressed = 1;
		rx = ev.xbutton.x;
		ry = ev.xbutton.y;
		break;
	    case ButtonRelease:
		printf("Button release\n");
		done = 1;
		break;
	    }
	}
    }
    /* clear the drawn rectangle */
    if (rect_w) {
	XDrawRectangle(disp, root, gc, rect_x, rect_y, rect_w, rect_h);
	XFlush(disp);
    }
    rw = ev.xbutton.x - rx;
    rh = ev.xbutton.y - ry;
    /* cursor moves backwards */
    if (rw < 0) {
	rx += rw;
	rw = 0 - rw;
    }
    if (rh < 0) {
	ry += rh;
	rh = 0 - rh;
    }

    XCloseDisplay(disp);

    printf("%dx%d+%d+%d\n",rw,rh,rx,ry);

    return EXIT_SUCCESS;
}
