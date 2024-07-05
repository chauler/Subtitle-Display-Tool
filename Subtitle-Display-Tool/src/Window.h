#pragma once
#include "Subtitle.h"

class Window {
public:
	Window(std::string dialogue);
	Window(Subtitle subtitle):m_subtitle(subtitle) {}
	Vec2f GetWindowDimensions() const;
	void Draw() const;
private:
	Subtitle m_subtitle;
};