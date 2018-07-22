#include <iostream>
#include <X11/Xlib.h>
#include <CImg.h>

using namespace cimg_library;
using namespace std;

static Display *display;
static Window root;

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

CImg<unsigned char> fullscreen_screenshot() {
    XWindowAttributes gwa;
    XGetWindowAttributes(display, root, &gwa);
    return screenshot(0, 0, gwa.width, gwa.height);
}

int main() {
    // Global variables
    display = XOpenDisplay(nullptr);
    root = DefaultRootWindow(display);

    CImg<unsigned char> pic = fullscreen_screenshot();    
    pic.save_png("/home/theo/Documents/blah.png");
    cout << "Screenshot saved" << endl;

    return 0;
}
