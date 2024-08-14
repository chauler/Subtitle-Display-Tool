#pragma once
#include "raylib.h"
#include "Subtitle.h"
#include "Timeline.h"

struct DrawConfig {
	int hostX;
	int hostY;
	int hostWidth;
	int hostHeight;
};

/**
* Container for a subtitle and the data and methods needed to draw that subtitle to the screen.
*/
class Window : public Timeline {
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
	void Draw(DrawConfig) const;
	/**
	* Returns a bool describing whether the window should be drawn at the current time, depending on time of creation and lifetime.
	*/
	bool IsVisible() const;
	void Play(double timestamp) override;

private:
	Window();
	Subtitle m_subtitle;
	RenderTexture2D m_target;
	double m_creationTime;
};