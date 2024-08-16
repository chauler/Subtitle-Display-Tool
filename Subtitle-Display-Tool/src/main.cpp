#include <iostream>
#include <string>
#include <vector>
#include <asio/io_context.hpp>
#include "raylib.h"
#include "WindowManager.h"
#include "Window.h"
#include "Subtitle.h"
#include "Parser.h"
#include "InputManager.h"
#include <thread>

int main(int argc, char * argv[]) {
	SetConfigFlags(FLAG_WINDOW_TRANSPARENT | FLAG_WINDOW_TOPMOST | FLAG_WINDOW_UNDECORATED | FLAG_WINDOW_UNFOCUSED | FLAG_WINDOW_MOUSE_PASSTHROUGH | FLAG_VSYNC_HINT);
	InitWindow(0, 0, "Subtitle Display Tool");
	
	// Initialize the window manager and parser
	WindowManager wm{};
	Parser parser{wm};
	
	// check if port number is in the input and valid
	if (argc < 2) {
		std::cout << "Usage: " << argv[0] << "PORT" << std::endl;
		return 1;
	}
	int portNum = std::stoi(argv[1]);
	if (portNum >= 0 && portNum <= 1023) {
		std::cout << "Selected Port: " << argv[1] << " is not a valid TCP port" << std::endl;
		return 1;
	}
	//set up input manager and io_context with user-defined port
	asio::io_context io_context;
	InputManager im{portNum, io_context};
	//For async work, io_context has to be run in a separate thread
	//It returns if there is no work queued, so we queue idleWork to keep it active until real work arrives
	asio::io_context::work idleWork(io_context);
	std::thread ioThread = std::thread([&]() { io_context.run(); });
  
	//main loop for raylib
	while (!WindowShouldClose()) {
		std::string input_data = im.getData();
		if (input_data != "") {
			parser.Parse(input_data);
		}

		BeginDrawing();
		wm.DrawWindows();
		ClearBackground(BLANK);
		EndDrawing();
	}
	return 0;
}