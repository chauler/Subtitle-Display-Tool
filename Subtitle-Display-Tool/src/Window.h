#pragma once
#include "raylib.h"
#include "Subtitle.h"

struct DrawConfig {
	int hostX;
	int hostY;
	int hostWidth;
	int hostHeight;
	Shader SDFShader;
	Shader outlineShader;
	Shader shadowShader;
};

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
	Vec2f GetWindowDimensions(const std::string&) const;
	/**
	* Draws the Window's associated texture to whatever texture is active at the time of call (in general, the main framebuffer).
	*/
	void Draw(const DrawConfig&);
	/**
	* Returns a bool describing whether the window should be drawn at the current time, depending on time of creation and lifetime.
	*/
	bool IsVisible() const;

private:
	Window();
	void RenderTexture(const std::string& text, const Shader& SDFShader, const Shader& outlineShader, const Shader& shadowShader);
	std::string WrapText(const std::string& text, const int& w, const int& h);
	Subtitle m_subtitle;
	RenderTexture2D m_target;
	double m_creationTime;
	bool m_textureRendered;
	std::string renderedText;
};