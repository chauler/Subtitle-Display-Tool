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
            "fontSize": 54,
            "position": [250, 250],
			"fontColor": [255, 255, 255, 255],
			"bgColor": [0, 0, 0, 0],
			"fontPath": "C:\\Windows\\Fonts\\Calibri.ttf",
			"lifetime": 50.0,
			"outline": {
				"size": 0.0, 
				"color": [0.0, 0.0, 0.0, 0.0]
			}, 
			"dropShadow": {
				"color": [0, 0, 0], 
				"offset": [0, 0], 
				"blurStrength": 0
			}
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