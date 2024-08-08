#pragma once
#include <vector>
#include "Window.h"

class WindowManager {
public:
	void AddWindow(const Window& window);
	void AddWindow(Window&& window);
	void DrawWindows();
	int hostX, hostY, hostWidth, hostHeight = 0;
private:
	std::vector<Window> m_windows;
};