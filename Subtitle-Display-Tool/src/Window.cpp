#include <iostream>
#include "raylib.h"
#include <math.h>
#include "Window.h"

#define DEFAULT_SPACING 5

Window::Window(std::string dialogue) : Window(Subtitle{dialogue}) {}

Window::Window(Subtitle subtitle) : m_subtitle(subtitle), m_target(LoadRenderTexture(GetWindowDimensions().x, GetWindowDimensions().y)), m_startTime(GetTime())
{
	BeginTextureMode(m_target);
	Color fontColor = { m_subtitle.GetColor().x, m_subtitle.GetColor().y, m_subtitle.GetColor().z, m_subtitle.GetColor().w};
	Color bgColor = { m_subtitle.GetBackgroundColor().x, m_subtitle.GetBackgroundColor().y, m_subtitle.GetBackgroundColor().z, m_subtitle.GetBackgroundColor().w };
	ClearBackground(bgColor);
	DrawTextEx(m_subtitle.GetFont(), m_subtitle.GetDialogue().c_str(), {0, 0}, m_subtitle.GetFontSize(), DEFAULT_SPACING, fontColor);
	EndTextureMode();
}

//Textures need to be cleaned up from GPU memory, but texture id's currently are shared between copies when Window objects are passed around. Need to define copy/move constructors
//to prevent sharing of texture ids. Current solution: just don't clean up.
Window::~Window() {
	//UnloadTexture(m_target.texture);
}

Vec2f Window::GetWindowDimensions() const
{
	Vector2 dims = MeasureTextEx(m_subtitle.GetFont(), m_subtitle.GetDialogue().c_str(), m_subtitle.GetFontSize(), DEFAULT_SPACING);
	return { dims.x, dims.y };
}

void Window::Draw() const
{
	if (!IsExpired()) {
		DrawTextureRec(m_target.texture, { 0, 0, GetWindowDimensions().x, -GetWindowDimensions().y }, { (float)m_subtitle.GetPosition().x, (float)m_subtitle.GetPosition().y }, WHITE);
	}
}

bool Window::IsExpired() const
{
	//Not sure if this will be permanent - treat a lifetime of 0 as permanent
	if (m_subtitle.GetLifetime() < 0.01 && m_subtitle.GetLifetime() > -0.01) {
		return false;
	}
	return GetTime() - m_startTime > m_subtitle.GetLifetime();
}