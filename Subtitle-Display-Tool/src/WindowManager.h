#pragma once
#include <vector>
#include "Window.h"

class WindowManager {
public:
	void AddWindow(const Window& window);
	void AddWindow(Window&& window);
	void DrawWindows();
private:
	std::vector<Window> m_windows;

#ifdef UNIT_TEST
	friend std::vector<Window>& GetWindows(WindowManager& wm);
#endif
};