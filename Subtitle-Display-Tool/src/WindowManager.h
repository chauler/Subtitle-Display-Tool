#pragma once
#include <vector>
#include "Window.h"
#include "FontManager.h"

/**
* High-level management of Window objects. Stores them, controls their lifetime, and displays them.
*/
class WindowManager {
public:
	WindowManager();
	/**
	* Pushes window into the WindowManager's internal list of windows.
	* @param window The window to be pushed - is copied before being pushed.
	*/
	void AddWindow(const Window& window);
	/**
	* Takes a temporary window and moves it into the WindowManager's internal list of windows.
	* @param window The window to be pushed - the object provided will be invalid after calling this function.
	*/
	void AddWindow(Window&& window);
	/**
	* Iterates through the list of Windows and calls the Draw() method of each visible one.
	*/
	void DrawWindows();
	int hostX, hostY, hostWidth, hostHeight;
private:
	std::vector<Window> m_windows;
	FontManager m_fontManager;
	Shader SDFShader;
	Shader outlineShader;
	Shader shadowShader;
	Shader MSDFShader;

#ifdef UNIT_TEST
	friend std::vector<Window>& GetWindows(WindowManager& wm);
#endif
};