#pragma once
#include <vector>
#include "Window.h"

class WindowManager {
public:
	void AddWindow(const Window& window);
	void DrawWindows();
private:
	std::vector<Window> m_windows;
};