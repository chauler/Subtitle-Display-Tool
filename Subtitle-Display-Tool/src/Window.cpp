#include "raylib.h"
#include <math.h>
#include "Window.h"

Window::Window(std::string dialogue) : m_subtitle(dialogue) {}

Vec2f Window::GetWindowDimensions() const
{
	return { floorf(MeasureText(m_subtitle.GetDialogue().c_str(), m_subtitle.GetFontSize())), m_subtitle.GetFontSize() };
}

void Window::Draw() const
{
	Color fontColor = { m_subtitle.GetColor().x, m_subtitle.GetColor().y, m_subtitle.GetColor().z, 255 };
	Color bgColor = { m_subtitle.GetBackgroundColor().x, m_subtitle.GetBackgroundColor().y, m_subtitle.GetBackgroundColor().z, m_subtitle.GetBackgroundColor().w };
	DrawRectangle(m_subtitle.GetPosition().x, m_subtitle.GetPosition().y, GetWindowDimensions().x, GetWindowDimensions().y, bgColor);
	DrawText(m_subtitle.GetDialogue().c_str(), m_subtitle.GetPosition().x, m_subtitle.GetPosition().y, m_subtitle.GetFontSize(), fontColor);
}
