#pragma once
#include "raylib.h"
#include "Subtitle.h"

/**
* Container for a subtitle and the data and methods needed to draw that subtitle to the screen.
*/
class Window {
public:
	Window(std::string dialogue, double creationTime);
	Window(Subtitle subtitle);
	Window(const Window& other);
	Window(Window&& other) noexcept;
	Window& operator=(const Window& other);
	Window& operator=(Window&& other) noexcept;
	~Window();
	/**
	* Returns the width and height of the window, calculated to the exact size of the text + any graphical effects.
	*/
	Vec2f GetWindowDimensions() const;
	/**
	* Draws the Window's associated texture to whatever texture is active at the time of call (in general, the main framebuffer).
	*/
	void Draw() const;
	/**
	* Returns a bool describing whether the window should be drawn at the current time, depending on time of creation and lifetime.
	*/
	bool IsVisible() const;

private:
	Window();
	Subtitle m_subtitle;
	RenderTexture2D m_target;
	double m_creationTime;
};