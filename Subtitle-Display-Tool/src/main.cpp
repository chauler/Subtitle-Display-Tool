#include <iostream>
#include <string>
#include <vector>
#include "raylib.h"
#include "WindowManager.h"
#include "Window.h"
#include "Subtitle.h"
#include "Parser.h"
#include "InputManager.h"

std::string SAMPLE_JSON = R"({
    "mode": "advanced",
    "data": {
        "dialogue": "This is a test string for the parser's advanced mode - it will disappear after 5 seconds",
        "styles": {
            "fontSize": 36,
            "position": [250, 250],
			"fontColor": [51, 255, 255, 255],
			"bgColor": [102, 0, 51, 255],
			"fontPath": "C:\\Windows\\Fonts\\Calibri.ttf",
			"lifetime": 3.0
        }
    }
})";


int main(int argc, char * argv[]) {
	SetConfigFlags(FLAG_WINDOW_TRANSPARENT | FLAG_WINDOW_TOPMOST | FLAG_WINDOW_UNDECORATED | FLAG_WINDOW_UNFOCUSED | FLAG_WINDOW_MOUSE_PASSTHROUGH | FLAG_VSYNC_HINT);
	InitWindow(0, 0, "Subtitle Display Tool");
	
	//The below AddWindow() calls are temporary to showcase how to create Windows and add them to the manager. These should be located within the parser at some point.
	WindowManager wm{};
	Parser parser{wm};
	InputManager im{};
	
	//This is one standard way to create a window - the first set of braces is the initializer list for the Subtitle object. Everything inside of it are the arguments for that constructor.
	wm.AddWindow(Window({ "Test String", {50, {300, 300}, {0, 255, 0, 0}, {102, 0, 51, 255}, "", 0.}}));
	//This is the same idea, but note the second set of braces, after the string. You can initialize the Styles struct with an initializer list as well - and you can specify which styles you want to
	//provide values for. The rest will all be default initialized. The provided arguments do have to be in the same order as declared in the struct declaration.
	//wm.AddWindow(Window({ "Significantly Longer String to Test Dynamic Window Sizes", {.fontSize = 80, .position = {100, 500}, .bgColor = {0, 0, 0, 0}, .lifetime = 0.}}));
	//As mentioned above, all data members of the Styles object have default values.  The Subtitle constructor doesn't require you to provide styles at all, it'll just create a default Styles object.
	//wm.AddWindow(Window("Testing default styles"));
	//wm.AddWindow(Window({ "Testing custom fonts", {.position = {0, 0}, .fontPath = "C:\\Windows\\Fonts\\arial.ttf", .lifetime = 0.} }));
	if (argc < 2) {
		std::cout << "Usage: " << argv[0] << "PORT" << std::endl;
		return 1;
	}
	int portNum = std::stoi(argv[1]);

	std::string input_data = im.GetData(portNum);
	std::cout << input_data;

	while (!WindowShouldClose()) {
		//Just using this to test the parser - this should eventually be called by the Input Manager with whatever messages were received
		if (SAMPLE_JSON != "") {
			parser.Parse(input_data);
			SAMPLE_JSON = "";
		}

		BeginDrawing();
		wm.DrawWindows();
		ClearBackground(BLANK);
		EndDrawing();
	}
	return 0;
}