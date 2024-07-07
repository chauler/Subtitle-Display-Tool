#include <iostream>
#include "raylib.h"
#include <math.h>
#include "Window.h"

Window::Window(std::string dialogue) : Window(Subtitle{dialogue}) {}

Window::Window(Subtitle subtitle) : m_subtitle(subtitle), m_target(LoadRenderTexture(GetWindowDimensions().x, GetWindowDimensions().y))
{
	BeginTextureMode(m_target);
	Color fontColor = { m_subtitle.GetColor().x, m_subtitle.GetColor().y, m_subtitle.GetColor().z, 255 };
	Color bgColor = { m_subtitle.GetBackgroundColor().x, m_subtitle.GetBackgroundColor().y, m_subtitle.GetBackgroundColor().z, m_subtitle.GetBackgroundColor().w };
	ClearBackground(bgColor);
	DrawText(m_subtitle.GetDialogue().c_str(), 0, 0, m_subtitle.GetFontSize(), fontColor);
	EndTextureMode();
}

//Textures need to be cleaned up from GPU memory, but texture id's currently are shared between copies when Window objects are passed around. Need to define copy/move constructors
//to prevent sharing of texture ids. Current solution: just don't clean up.
Window::~Window() {
	//UnloadTexture(m_target.texture);
}

Vec2f Window::GetWindowDimensions() const
{
	return { floorf(MeasureText(m_subtitle.GetDialogue().c_str(), m_subtitle.GetFontSize())), m_subtitle.GetFontSize() };
}

void Window::Draw() const
{
	DrawTextureRec(m_target.texture, { 0, 0, GetWindowDimensions().x, -GetWindowDimensions().y }, { (float)m_subtitle.GetPosition().x, (float)m_subtitle.GetPosition().y }, WHITE);
}
