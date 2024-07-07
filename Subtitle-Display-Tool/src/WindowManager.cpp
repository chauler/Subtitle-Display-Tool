#include "WindowManager.h"

void WindowManager::AddWindow(Window window)
{
	m_windows.push_back(window);
}

void WindowManager::DrawWindows()
{
	for (const auto& window : m_windows) {
		window.Draw();
	}
}
