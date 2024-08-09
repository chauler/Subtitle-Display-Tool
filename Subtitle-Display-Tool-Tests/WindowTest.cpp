#include "gtest/gtest.h"
#include "Window.h"
#include "WindowManager.h"
#include "raylib.h"

class WindowTest : public testing::Test {
protected:
	WindowTest() {
		SetConfigFlags(FLAG_WINDOW_TRANSPARENT | FLAG_WINDOW_TOPMOST | FLAG_WINDOW_UNDECORATED | FLAG_WINDOW_UNFOCUSED | FLAG_WINDOW_MOUSE_PASSTHROUGH | FLAG_VSYNC_HINT);
		InitWindow(0, 0, "Subtitle Display Tool");
	}

	WindowManager wm{};
};

TEST_F(WindowTest, CheckExpired) {
	//Window should expire immediately
	Window testWindow{ {"", {.lifetime{0.0} } } };
	EXPECT_EQ(testWindow.IsVisible(), false);
}

TEST_F(WindowTest, CheckNotExpired) {
	//Window should expire immediately
	Window testWindow{ {"", {.lifetime{100.0} } } };
	EXPECT_EQ(testWindow.IsVisible(), true);
}

TEST_F(WindowTest, Draw) {
	Window testWindow{ {""} };
	EXPECT_NO_THROW(testWindow.Draw(););
}