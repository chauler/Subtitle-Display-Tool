#include <iostream>
#include <string>
#include <vector>
#include "raylib.h"
#include "WindowManager.h"
#include "Window.h"
#include "Subtitle.h"

int main() {
	SetConfigFlags(FLAG_WINDOW_TRANSPARENT | FLAG_WINDOW_TOPMOST | FLAG_WINDOW_UNDECORATED | FLAG_WINDOW_UNFOCUSED | FLAG_WINDOW_MOUSE_PASSTHROUGH | FLAG_VSYNC_HINT);
	InitWindow(0, 0, "Subtitle Display Tool");
	
	//The below AddWindow() calls are temporary to showcase how to create Windows and add them to the manager. These should be located within the parser at some point.
	WindowManager wm{};
	//This is one standard way to create a window - the first set of braces is the initializer list for the Subtitle object. Everything inside of it are the arguments for that constructor.
	wm.AddWindow(Window({ "Test String", {50, {300, 300}, {100, 200, 255}, {255, 100, 100, 200}} }));
	//This is the same idea, but note the second set of braces, after the string. You can initialize the Styles struct with an initializer list as well - and you can specify which styles you want to
	//provide values for. The rest will all be default initialized. The provided arguments do have to be in the same order as declared in the struct declaration.
	wm.AddWindow(Window({ "Significantly Longer String to Test Dynamic Window Sizes", {.fontSize = 80, .position = {100, 500}, .bgColor = {0, 0, 0, 0}}}));
	//As mentioned above, all data members of the Styles object have default values.  The Subtitle constructor doesn't require you to provide styles at all, it'll just create a default Styles object.
	wm.AddWindow(Window("Testing default styles"));
	wm.AddWindow(Window({ "Testing custom fonts", {.position = {0, 0}, .fontPath = "C:\\Windows\\Fonts\\arial.ttf"} }));

	while (!WindowShouldClose()) {
		BeginDrawing();
		wm.DrawWindows();
		ClearBackground(BLANK);
		EndDrawing();
	}
	return 0;
}