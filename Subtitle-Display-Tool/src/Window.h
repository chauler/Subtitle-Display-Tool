#pragma once
#include "raylib.h"
#include "Subtitle.h"

class Window {
public:
	Window(std::string dialogue);
	Window(Subtitle subtitle);
	~Window();
	Vec2f GetWindowDimensions() const;
	void Draw() const;
private:
	Window();
	Subtitle m_subtitle;
	RenderTexture2D m_target;
};