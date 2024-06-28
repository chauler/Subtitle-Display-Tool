#include <iostream>
#include "raylib.h"

int main() {
	const int screenWidth = 575;
	const int screenHeight = 100;

	SetConfigFlags(FLAG_WINDOW_TRANSPARENT | FLAG_WINDOW_TOPMOST | FLAG_WINDOW_UNDECORATED | FLAG_WINDOW_UNFOCUSED | FLAG_WINDOW_MOUSE_PASSTHROUGH | FLAG_VSYNC_HINT);
	InitWindow(screenWidth, screenHeight, "Subtitle Display Tool");
	unsigned int i = 0;
	RenderTexture2D target = LoadRenderTexture(screenWidth, screenHeight);
	while (!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(BLANK);
		DrawText("Hello World", 0, 0, 100, LIGHTGRAY);
		EndDrawing();
	}
	return 0;
}