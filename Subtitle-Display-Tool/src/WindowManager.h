#pragma once
#include <vector>
#include "Window.h"
#include "Timeline.h"
#include "Timer.h"

class WindowManager {
public:
	void AddWindow(const Window& window);
	void AddWindow(Window&& window);
	void DrawWindows();
private:
	std::vector<Window> m_windows;
	Timeline masterTimeline;
	Timer timer;
};