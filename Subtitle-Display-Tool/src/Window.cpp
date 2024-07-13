#include <iostream>
#include "raylib.h"
#include <math.h>
#include "Window.h"

#define DEFAULT_SPACING 5

Window::Window(std::string dialogue) : Window(Subtitle{dialogue}) {}

Window::Window(Subtitle subtitle) : m_subtitle(subtitle), m_target(LoadRenderTexture(GetWindowDimensions().x, GetWindowDimensions().y))
{
	std::cout << "Constructor called" << std::endl;
	BeginTextureMode(m_target);
	Color fontColor = { m_subtitle.GetColor().x, m_subtitle.GetColor().y, m_subtitle.GetColor().z, 255 };
	Color bgColor = { m_subtitle.GetBackgroundColor().x, m_subtitle.GetBackgroundColor().y, m_subtitle.GetBackgroundColor().z, m_subtitle.GetBackgroundColor().w };
	ClearBackground(bgColor);
	DrawTextEx(m_subtitle.GetFont(), m_subtitle.GetDialogue().c_str(), {0, 0}, m_subtitle.GetFontSize(), DEFAULT_SPACING, WHITE);
	EndTextureMode();
}

Window::Window(const Window& other): Window(other.m_subtitle)
{
	std::cout << "Copy constructor called" << std::endl;
}

//If we copy a temporary over, we don't want the texture to be unloaded when the temporary is destroyed.
// So, remove the temporary's render target.
Window::Window(Window&& other) noexcept : m_subtitle(other.m_subtitle), m_target(other.m_target)
{
	std::cout << "Move Constructor Called" << std::endl;
	//BeginTextureMode(m_target);
	//Color fontColor = { m_subtitle.GetColor().x, m_subtitle.GetColor().y, m_subtitle.GetColor().z, 255 };
	//Color bgColor = { m_subtitle.GetBackgroundColor().x, m_subtitle.GetBackgroundColor().y, m_subtitle.GetBackgroundColor().z, m_subtitle.GetBackgroundColor().w };
	//ClearBackground(bgColor);
	//DrawTextEx(m_subtitle.GetFont(), m_subtitle.GetDialogue().c_str(), { 0, 0 }, m_subtitle.GetFontSize(), DEFAULT_SPACING, WHITE);
	//EndTextureMode();
	other.m_target = {};
}

Window& Window::operator=(const Window& other)
{
	std::cout << "Assignment operator - shouldn't be called" << std::endl;
	throw std::exception();
	// TODO: insert return statement here
	return *this = Window(other);
}

Window& Window::operator=(Window&& other) noexcept
{
	std::cout << "Move assignment operator - shouldn't be called" << std::endl;
	throw std::exception();
	// TODO: insert return statement here
	return *this;
}

//Textures need to be cleaned up from GPU memory, but texture id's currently are shared between copies when Window objects are passed around. Need to define copy/move constructors
//to prevent sharing of texture ids. Current solution: just don't clean up.
Window::~Window() {
	std::cout << "Destructor called" << std::endl;
	UnloadTexture(m_target.texture);
}

Vec2f Window::GetWindowDimensions() const
{
	Vector2 dims = MeasureTextEx(m_subtitle.GetFont(), m_subtitle.GetDialogue().c_str(), m_subtitle.GetFontSize(), DEFAULT_SPACING);
	return { dims.x, dims.y };
}

void Window::Draw() const
{
	DrawTextureRec(m_target.texture, { 0, 0, GetWindowDimensions().x, -GetWindowDimensions().y }, { (float)m_subtitle.GetPosition().x, (float)m_subtitle.GetPosition().y }, WHITE);
}
