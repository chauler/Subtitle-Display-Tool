#include "raylib.h"
#include <math.h>
#include "Window.h"

Window::Window(std::string dialogue) : m_subtitle(dialogue) {}

WindowDims Window::GetWindowDimensions()
{
	return { floorf(MeasureText(m_subtitle.GetDialogue().c_str(), m_subtitle.GetFontSize())), m_subtitle.GetFontSize() };
}

void Window::Draw()
{
	DrawText(m_subtitle.GetDialogue().c_str(), 0, 0, m_subtitle.GetFontSize(), LIGHTGRAY);
}
