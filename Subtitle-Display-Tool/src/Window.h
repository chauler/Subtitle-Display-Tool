#pragma once
#include "raylib.h"
#include "Subtitle.h"

class Window {
public:
	Window(std::string dialogue, double creationTime);
	Window(Subtitle subtitle);
	Window(const Window& other);
	Window(Window&& other) noexcept;
	Window& operator=(const Window& other);
	Window& operator=(Window&& other) noexcept;
	~Window();
	Vec2f GetWindowDimensions() const;
	void Draw() const;
	bool IsVisible() const;

private:
	Window();
	Subtitle m_subtitle;
	RenderTexture2D m_target;
	double m_creationTime;
};