#include <iostream>
#include "raylib.h"
#include <math.h>
#include "Window.h"

#define DEFAULT_SPACING 5
#define SDF_SHADER_PATH "../../Subtitle-Display-Tool/res/shaders/sdf.fs"
#define OUTLINE_SHADER_PATH "../../Subtitle-Display-Tool/res/shaders/outline.fs"
#define SHADOW_SHADER_PATH "../../Subtitle-Display-Tool/res/shaders/shadow.fs"

Window::Window(std::string dialogue, double creationTime) : Window(Subtitle{ dialogue, {}, creationTime }) {}

Window::Window(Subtitle subtitle) : 
	m_subtitle(subtitle), 
	m_target(LoadRenderTexture(GetWindowDimensions().x, GetWindowDimensions().y)), 
	m_creationTime(GetTime())
{
	Color fontColor = { m_subtitle.GetColor().x, m_subtitle.GetColor().y, m_subtitle.GetColor().z, m_subtitle.GetColor().w };
	Color bgColor = { m_subtitle.GetBackgroundColor().x, m_subtitle.GetBackgroundColor().y, m_subtitle.GetBackgroundColor().z, m_subtitle.GetBackgroundColor().w };
	int outlineSize = m_subtitle.GetStyles().outline.outlineSize;
	float* shadowColor = m_subtitle.GetStyles().shadow.color.values;
	int* shadowOffset = m_subtitle.GetStyles().shadow.offset.values;

	//Replace the macro once the layout of the distributed version is decided
	Shader SDFShader = LoadShader(0, SDF_SHADER_PATH);
	Shader outlineShader = LoadShader(0, OUTLINE_SHADER_PATH);
	Shader shadowShader = LoadShader(0, SHADOW_SHADER_PATH);

	//Update shader uniforms with values from subtitle styles
	int outlineSizeLoc = GetShaderLocation(outlineShader, "outlineSize");
	int outlineColorLoc = GetShaderLocation(outlineShader, "outlineColor");
	int textureSizeLoc = GetShaderLocation(outlineShader, "textureSize");
	float textureSize[2] = { GetWindowDimensions().x, GetWindowDimensions().y };
	SetShaderValue(outlineShader, outlineSizeLoc, &outlineSize, SHADER_UNIFORM_INT);
	SetShaderValue(outlineShader, outlineColorLoc, m_subtitle.GetStyles().outline.outlineColor.values, SHADER_UNIFORM_VEC4);
	SetShaderValue(outlineShader, textureSizeLoc, textureSize, SHADER_UNIFORM_VEC2);

	textureSizeLoc = GetShaderLocation(shadowShader, "samplerSize");
	SetShaderValue(shadowShader, textureSizeLoc, textureSize, SHADER_UNIFORM_VEC2);
	SetShaderValue(shadowShader, GetShaderLocation(shadowShader, "color"), shadowColor,SHADER_UNIFORM_VEC3);
	SetShaderValue(shadowShader, GetShaderLocation(shadowShader, "offset"), shadowOffset, SHADER_UNIFORM_IVEC2);

	RenderTexture2D secondaryTex = LoadRenderTexture(GetWindowDimensions().x, GetWindowDimensions().y);
	RenderTexture2D pingpongBuffers[2] = { LoadRenderTexture(GetWindowDimensions().x, GetWindowDimensions().y) , LoadRenderTexture(GetWindowDimensions().x, GetWindowDimensions().y) };
	//We are drawing to a texture, with a shader used to draw SDF fonts
	BeginTextureMode(secondaryTex);
	BeginShaderMode(SDFShader);
	//Texture is already sized to how it will be displayed, so just use the texture as the background and color it.
	ClearBackground(BLANK);
	rlDrawTextEx(m_subtitle.GetFont(), m_subtitle.GetDialogue().c_str(), { (float)m_subtitle.GetStyles().outline.outlineSize, (float)m_subtitle.GetStyles().outline.outlineSize }, m_subtitle.GetFontSize(), DEFAULT_SPACING, fontColor);
	EndShaderMode();
	EndTextureMode();
	
	//Second pass. Draw texture with outline shader enabled.
	BeginTextureMode(pingpongBuffers[0]);
	//We draw rectangle for the background so that non-opaque text correctly
	//inherits the background color, rather than the color of the screen behind the window
	BeginShaderMode(outlineShader);
	DrawTextureRec(secondaryTex.texture, {0, 0, GetWindowDimensions().x, -GetWindowDimensions().y}, {0, 0}, WHITE);
	EndShaderMode();
	EndTextureMode();

	BeginTextureMode(pingpongBuffers[1]);
	ClearBackground(BLANK);
	BeginShaderMode(shadowShader);
	DrawTextureRec(pingpongBuffers[0].texture, {0, 0, GetWindowDimensions().x, -GetWindowDimensions().y}, {0, 0}, WHITE);
	EndShaderMode();
	//DrawTextureRec(m_target.texture, { 0, 0, GetWindowDimensions().x, -GetWindowDimensions().y }, { 0, 0 }, WHITE);
	EndTextureMode();

	BeginTextureMode(m_target);
	ClearBackground(BLANK);
	DrawRectangle(0, 0, GetWindowDimensions().x, GetWindowDimensions().y, bgColor);
	DrawTextureRec(pingpongBuffers[1].texture, {0, 0, GetWindowDimensions().x, -GetWindowDimensions().y}, {0, 0}, WHITE);
	DrawTextureRec(pingpongBuffers[0].texture, {0, 0, GetWindowDimensions().x, -GetWindowDimensions().y}, {0, 0}, WHITE);
	EndTextureMode();
}

Window::Window(const Window& other): Window(other.m_subtitle)
{
}

//If we copy a temporary over, we don't want the texture to be unloaded when the temporary is destroyed.
// So, remove the temporary's render target.
Window::Window(Window&& other) noexcept : m_subtitle(other.m_subtitle), m_target(other.m_target), m_creationTime(other.m_creationTime)
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
	dims.x += 2*m_subtitle.GetStyles().outline.outlineSize;
	dims.y += 2*m_subtitle.GetStyles().outline.outlineSize;
	return { dims.x, dims.y };
}

void Window::Draw() const
{
	if (IsVisible()) {
		DrawTextureRec(m_target.texture, { 0, 0, GetWindowDimensions().x, -GetWindowDimensions().y }, { (float)m_subtitle.GetPosition().x, (float)m_subtitle.GetPosition().y }, WHITE);
	}
}

bool Window::IsVisible() const {
	double currentTime = GetTime();
	double subtitleStartTime = m_creationTime + m_subtitle.GetStartTime();
	return currentTime >= subtitleStartTime && currentTime < subtitleStartTime + m_subtitle.GetLifetime();
}