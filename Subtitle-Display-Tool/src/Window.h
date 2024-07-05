#pragma once
#include "Subtitle.h"

class Window {
public:
	Window(std::string dialogue);
	Vec2f GetWindowDimensions();
	void Draw();
private:
	Subtitle m_subtitle;
};