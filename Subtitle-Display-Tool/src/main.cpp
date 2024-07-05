#include <iostream>
#include <vector>
#include "raylib.h"
#include "Window.h"
#include "Subtitle.h"

int main() {
	SetConfigFlags(FLAG_WINDOW_TRANSPARENT | FLAG_WINDOW_TOPMOST | FLAG_WINDOW_UNDECORATED | FLAG_WINDOW_UNFOCUSED | FLAG_WINDOW_MOUSE_PASSTHROUGH | FLAG_VSYNC_HINT);
	InitWindow(0, 0, "Subtitle Display Tool");
	
	//Temporary. Need to create a manager that takes input from the host program and creates windows dynamically,
	//probably adding them to a vector/map.
	std::vector<Window> windows{};
	Window test({ "Test String", 50, {100, 100}, {100, 200, 255} });
	Window test2({ "Significantly Longer String to Test Dynamic Window Sizes", 20, {100, 500}, {255, 100, 0} });
	windows.push_back(test);
	windows.push_back(test2);
	unsigned int i = 0;
	while (!WindowShouldClose()) {
		BeginDrawing();
		for (const auto& window : windows) {
			window.Draw();
		}
		ClearBackground(BLANK);
		EndDrawing();
	}
	return 0;
}