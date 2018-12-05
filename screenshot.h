#ifndef SCREENSHOT_H
#define SCREENSHOT_H
#include <CImg.h>

using namespace cimg_library;
using namespace std;

CImg<unsigned char> screenshot(int top_x, int top_y, int width, int height);
CImg<unsigned char> fullscreen_screenshot();
CImg<unsigned char> region_screenshot();
void save_as_file(CImg<unsigned char> img_to_save, string path);
void save_to_clipboard(CImg<unsigned char> img_to_save);

#endif
