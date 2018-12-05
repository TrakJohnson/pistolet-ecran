#include <iostream>
#include <X11/Xlib.h>
#include <X11/cursorfont.h>
#include <CImg.h>
// #include <clip/clip.h> TODO

using namespace cimg_library;
using namespace std;

static Display *display = XOpenDisplay(nullptr);
static Window root = DefaultRootWindow(display);


CImg<unsigned char> screenshot(int top_x, int top_y, int width, int height) {
    XImage *image = XGetImage(display, root, top_x, top_y, width, height, AllPlanes, ZPixmap);

    unsigned long red_mask = image->red_mask;
    unsigned long green_mask = image->green_mask;
    unsigned long blue_mask = image->blue_mask;

    unsigned char *array = new unsigned char[width * height * 3];
    CImg<unsigned char> pic(array, width, height, 1, 3);
    
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height ; y++) {
	    unsigned long pixel = XGetPixel(image, x, y);
	    unsigned char blue = pixel & blue_mask;
	    unsigned char green = (pixel & green_mask) >> 8;
	    unsigned char red = (pixel & red_mask) >> 16;
	    
	    pic(x, y, 0) = red;
	    pic(x, y, 1) = green;
	    pic(x, y, 2) = blue;
        }
    }
    return pic;
}

int* capture_two_mice(int coords[4]) {
    /* This functions captures two mouse clicks, 
     * which will be used to define the boundary of the region screenshot
     */
    
    XEvent ev;
    Display *display = XOpenDisplay(NULL);

    if(!display) {
	return nullptr;
    }

    Screen *scr = ScreenOfDisplay(display, DefaultScreen(display));
    Window root = RootWindow(display, XScreenNumberOfScreen(scr));

    Cursor cursor = XCreateFontCursor(display, XC_crosshair);

    if (XGrabPointer(display, root, False,
		     ButtonMotionMask | ButtonPressMask | ButtonReleaseMask, GrabModeAsync,
		     GrabModeAsync, root, cursor, CurrentTime) != GrabSuccess) {
	printf("ouldn't grab pointer:");
    }

    int done = 0;
    int click_count = 0;
    // Main event loop
    while (!done) {
	if (!XPending(display)) {
	    usleep(10);
	    continue;
	}
	if ((XNextEvent(display, &ev) >= 0)) {
	    switch (ev.type) {
	    case ButtonRelease:
		if (ev.xbutton.button == 1) {
		    coords[click_count * 2] = ev.xmotion.x;
		    coords[click_count * 2 + 1] = ev.xmotion.y;
		    click_count += 1;
		    if (click_count == 2) {
			return coords;
		    }
		}
		break;
	    default:
		break;
	    }
	}
    }
    XCloseDisplay(display);
}

CImg<unsigned char> region_screenshot() {
    int coords [4];
    capture_two_mice(coords);
    printf("%d %d %d %d\n", coords[0], coords[1], coords[2], coords[3]);
    return screenshot(coords[0], coords[1], coords[2] - coords[0], coords[3] - coords[1]);
}

CImg<unsigned char> fullscreen_screenshot() {
    XWindowAttributes gwa;
    XGetWindowAttributes(display, root, &gwa);
    // cout << gwa.width << " " << gwa.height << endl;
    return screenshot(0, 0, gwa.width, gwa.height);
}

void save_as_file(CImg<unsigned char> img, string path) {
    img.save_png(path.c_str());  // necessary because save_png expects const char*
}

void save_to_clipboard() {
    // TODO finish that
    // uint32_t data[] = {0xffff0000, 0xff00ff00, 0xff0000ff,
    // 		       0x7fff0000, 0x7f00ff00, 0x7f0000ff};
    // clip::image_spec spec;
    // spec.width = 3;
    // spec.height = 2;
    // spec.bits_per_pixel = 32;
    // spec.bytes_per_row = spec.width*4;
    // spec.red_mask = 0xff;
    // spec.green_mask = 0xff00;
    // spec.blue_mask = 0xff0000;
    // spec.alpha_mask = 0xff000000;
    // spec.red_shift = 0;
    // spec.green_shift = 8;
    // spec.blue_shift = 16;
    // spec.alpha_shift = 24;
    // clip::image img(data, spec);
    // clip::set_image(img);
}

// int main() {
//     // Global variables
//     display = XOpenDisplay(nullptr);
//     root = DefaultRootWindow(display);
    
//     CImg<unsigned char> pic = region_screenshot();
//     pic.save_png("/home/theo/Pictures/blah.png");
//     cout << "Screenshot saved" << endl;
    
//     return 0;
// }
