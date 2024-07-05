#include <iostream>
#include "raylib.h"
#include "Window.h"

int main() {
	SetConfigFlags(FLAG_WINDOW_TRANSPARENT | FLAG_WINDOW_TOPMOST | FLAG_WINDOW_UNDECORATED | FLAG_WINDOW_UNFOCUSED | FLAG_WINDOW_MOUSE_PASSTHROUGH | FLAG_VSYNC_HINT);
	InitWindow(0, 0, "Subtitle Display Tool");
	Window test("Test String");
	Window test2("Significantly Longer String to Test Dynamic Window Sizes");
	unsigned int i = 0;
	while (!WindowShouldClose()) {
		Window& currentWindow = i % 2 ? test : test2;
		i++;
		BeginDrawing();
		currentWindow.Draw();
		ClearBackground(BLANK);
		EndDrawing();
	}
	return 0;
}