#include "WindowManager.h"
#include <iostream>

WindowManager::WindowManager(): hostX(0), hostY(0), hostWidth(GetScreenWidth()), hostHeight(GetScreenHeight())
{
}

void WindowManager::AddWindow(const Window& window)
{
	m_windows.push_back(window);
}

void WindowManager::AddWindow(Window&& window)
{
	m_windows.push_back(std::move(window));
}

void WindowManager::DrawWindows()
{
	for (auto& window : m_windows) {
		window.Draw({ .hostX{hostX}, .hostY{hostY}, .hostWidth{hostWidth}, .hostHeight{hostHeight} });
	}
}
