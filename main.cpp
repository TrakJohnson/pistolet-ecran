#include <iostream>
#include <cstdlib>
#include <ctime>
#include <tclap/CmdLine.h>
#include <CImg.h>
#include "screenshot.h"

using namespace std;
using namespace cimg_library;

int main(int argc, char** argv) {
    // Handle command line arguments
    try {  
	TCLAP::CmdLine cmd("A screenshot utility", ' ', "0.1");
	
	TCLAP::ValueArg<string> mode_arg("m", "mode",
					 "Screenshot mode (fullscreen|region)",
					 false, "fullscreen", "string", cmd);

	TCLAP::ValueArg<string> directory_arg("d", "directory",
					      "Default is ~/",
					      false, getenv("HOME"), "string", cmd);
	
	TCLAP::ValueArg<string> filename_arg("f", "filename",
					     "Default is \"screenshot-<timestamp>.png\"",
					     false, "screenshot-" + to_string(time(0)) + ".png", "string", cmd);
	
	TCLAP::SwitchArg cliboard_arg("c", "clipboard",
				      "Copy image to clipboard",
				      cmd, false);
	cmd.parse(argc, argv);

	// Default behaviour is:
	// - Take a fullscreen screenshot
	// - Name it after the unix timestamp
	// - Store it in $HOME
	string mode = mode_arg.getValue();
	string path = directory_arg.getValue() + "/" + filename_arg.getValue();
	CImg<unsigned char> i;
	if (mode == "fullscreen") {
	    i = fullscreen_screenshot();
	} else {
	    i = region_screenshot();
	}

	if (cliboard_arg.getValue()) {
	    // save_to_clipboard(i);
	    // TODO 
	} else {
	    save_as_file(i, path);
	}


	cout << "Saved under " << path << endl;

    } catch (TCLAP::ArgException &e) {
	cerr << "Error: " << e.error() << " for arg " << e.argId() << endl;
    }
}
