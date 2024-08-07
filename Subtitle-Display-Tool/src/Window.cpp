#include <iostream>
#include "raylib.h"
#include <math.h>
#include "Window.h"

#define DEFAULT_SPACING 5
#define TEMP_SHADER_PATH "../../Subtitle-Display-Tool/res/shaders/sdf.fs"

Window::Window(std::string dialogue) : Window(Subtitle{dialogue}) {}

Window::Window(Subtitle subtitle) : m_subtitle(subtitle), m_target(LoadRenderTexture(GetWindowDimensions().x, GetWindowDimensions().y)), m_startTime(GetTime())
{
	Color fontColor = { m_subtitle.GetColor().x, m_subtitle.GetColor().y, m_subtitle.GetColor().z, m_subtitle.GetColor().w };
	Color bgColor = { m_subtitle.GetBackgroundColor().x, m_subtitle.GetBackgroundColor().y, m_subtitle.GetBackgroundColor().z, m_subtitle.GetBackgroundColor().w };

	//Replace the macro once the layout of the distributed version is decided
	Shader shader = LoadShader(0, TEMP_SHADER_PATH);

	//We are drawing to a texture, with a shader used to draw SDF fonts
	BeginTextureMode(m_target);
	BeginShaderMode(shader);
	//Texture is already sized to how it will be displayed, so just use the texture as the background and color it.
	ClearBackground(bgColor);
	rlDrawTextEx(m_subtitle.GetFont(), m_subtitle.GetDialogue().c_str(), {0, 0}, m_subtitle.GetFontSize(), DEFAULT_SPACING, fontColor);
	EndShaderMode();
	EndTextureMode();
}

Window::Window(const Window& other): Window(other.m_subtitle)
{
}

//If we copy a temporary over, we don't want the texture to be unloaded when the temporary is destroyed.
// So, remove the temporary's render target.
Window::Window(Window&& other) noexcept : m_subtitle(other.m_subtitle), m_target(other.m_target), m_startTime(other.m_startTime)
{
	other.m_target = {};
}

Window& Window::operator=(const Window& other)
{
	std::cout << "Assignment operator - shouldn't be called" << std::endl;
	throw std::exception();
	return *this = Window(other);
}

Window& Window::operator=(Window&& other) noexcept
{
	std::cout << "Move assignment operator - shouldn't be called" << std::endl;
	throw std::exception();
	return *this;
}

//Textures need to be cleaned up from GPU memory, but texture id's currently are shared between copies when Window objects are passed around. Need to define copy/move constructors
//to prevent sharing of texture ids. Current solution: just don't clean up.
Window::~Window() {
	UnloadTexture(m_target.texture);
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
