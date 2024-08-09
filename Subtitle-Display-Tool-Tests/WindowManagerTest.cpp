#include "gtest/gtest.h"
#include "WindowManager.h"

std::vector<Window>& GetWindows(WindowManager& wm) {
	return wm.m_windows;
}

class WindowManagerTest : public testing::Test {
protected:
	WindowManagerTest() {
		SetConfigFlags(FLAG_WINDOW_TRANSPARENT | FLAG_WINDOW_TOPMOST | FLAG_WINDOW_UNDECORATED | FLAG_WINDOW_UNFOCUSED | FLAG_WINDOW_MOUSE_PASSTHROUGH | FLAG_VSYNC_HINT);
		InitWindow(0, 0, "Subtitle Display Tool");
	}
};

TEST_F(WindowManagerTest, AddWindowFromStack) {
	WindowManager wm{};
	Window window{ {""} };
	wm.AddWindow(window);
	auto windows = GetWindows(wm);
	EXPECT_EQ(windows.size(), 1);
}

TEST_F(WindowManagerTest, AddWindowFromTemp) {
	WindowManager wm{};
	wm.AddWindow({ {""} });
	auto windows = GetWindows(wm);
	EXPECT_EQ(windows.size(), 1);
}