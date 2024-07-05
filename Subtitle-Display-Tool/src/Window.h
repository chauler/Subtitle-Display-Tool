#pragma once
#include "Subtitle.h"

struct WindowDims {
	float w;
	float h;
};

class Window {
public:
	Window(std::string dialogue);
	WindowDims GetWindowDimensions();
	void Draw();
private:
	Subtitle m_subtitle;
};