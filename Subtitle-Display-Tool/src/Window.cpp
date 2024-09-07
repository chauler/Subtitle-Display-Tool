#include <iostream>
#include "raylib.h"
#include <math.h>
#include "Window.h"
#include <vector>
#include <sstream>
#include "util\util.h"

#define DEFAULT_SPACING 5

Window::Window(std::string dialogue, double creationTime) : Window(Subtitle{ dialogue, {}, creationTime }) {}

Window::Window(Subtitle subtitle) :
	m_subtitle(subtitle),
	m_target(),
	m_creationTime(GetTime()),
	m_textureRendered(false),
	renderedText()
{

}

Window::Window(const Window& other): Window(other.m_subtitle)
{
}

//If we copy a temporary over, we don't want the texture to be unloaded when the temporary is destroyed.
// So, remove the temporary's render target.
Window::Window(Window&& other) noexcept : m_subtitle(other.m_subtitle), m_target(other.m_target), m_creationTime(other.m_creationTime), m_textureRendered(false)
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

Vec2f Window::GetWindowDimensions(const std::string& text) const
{
	//Calculate x first, as it can be done in one pass.
	Vector2 dims = { MeasureTextEx(m_subtitle.GetFont(), text.c_str(), m_subtitle.GetFontSize(), DEFAULT_SPACING).x, 0 };
	dims.x += 2 * m_subtitle.GetStyles().outline.outlineSize;

	//Calculate y by adding height for each line.
	std::istringstream textStream{ text };
	std::string line;
	while (getline(textStream, line)) {
		dims.y += GetLineHeight(line);
	}
	return { dims.x, dims.y };
}

float Window::GetLineHeight(const std::string& text) const {
	//Window width has to be calculated with all lines considered, so we skip it here.
	float height = MeasureTextEx(m_subtitle.GetFont(), text.c_str(), m_subtitle.GetFontSize(), DEFAULT_SPACING).y;

	//Add extra pixels for anything added to the text - outlines, shadows, underlines, etc.
	height += 2 * m_subtitle.GetStyles().outline.outlineSize;

	//Still need to decide a good way to calculate the space to add for shadows.
	return height;
}

void Window::Draw(const DrawConfig& config)
{
	if (IsVisible()) {
		float finalXPosition = config.hostX + m_subtitle.GetPosition().x;
		float finalYPosition = config.hostY + m_subtitle.GetPosition().y;
		std::string wrappedText = WrapText(m_subtitle.GetDialogue().c_str(), config.hostWidth, config.hostHeight);
		if (m_textureRendered == false || wrappedText != renderedText) {
			std::cout << wrappedText << std::endl;
			if (m_textureRendered) {
				UnloadRenderTexture(m_target);
			}
			RenderTexture(wrappedText, config.MSDFShader, config.outlineShader, config.shadowShader);
			renderedText = wrappedText;
			m_textureRendered = true;
		}
		DrawTextureRec(m_target.texture,
			{ 0, 0, GetWindowDimensions(wrappedText).x, -GetWindowDimensions(wrappedText).y },
			{ finalXPosition, finalYPosition },
			rlWHITE);
	}
	if (!IsVisible() && m_textureRendered) {
		UnloadRenderTexture(m_target);
		m_textureRendered = false;
	}
}

bool Window::IsVisible() const {
	double currentTime = GetTime();
	double subtitleStartTime = m_creationTime + m_subtitle.GetStartTime();
	return currentTime >= subtitleStartTime && currentTime < subtitleStartTime + m_subtitle.GetLifetime();
}

void Window::RenderTexture(const std::string& text, const Shader& SDFShader, const Shader& outlineShader, const Shader& shadowShader) {
	Vec2f dims = GetWindowDimensions(text);
	m_target = LoadRenderTexture(dims.x, dims.y);
	Color fontColor = { m_subtitle.GetColor().x, m_subtitle.GetColor().y, m_subtitle.GetColor().z, m_subtitle.GetColor().w };
	Color bgColor = { m_subtitle.GetBackgroundColor().x, m_subtitle.GetBackgroundColor().y, m_subtitle.GetBackgroundColor().z, m_subtitle.GetBackgroundColor().w };
	int outlineSize = m_subtitle.GetStyles().outline.outlineSize;
	float* shadowColor = m_subtitle.GetStyles().shadow.color.values;
	int* shadowOffset = m_subtitle.GetStyles().shadow.offset.values;
	float fontSize = m_subtitle.GetFontSize();
	SetShaderValue(SDFShader, GetShaderLocation(SDFShader, "fontSize"), &fontSize, SHADER_UNIFORM_FLOAT);
	//Update shader uniforms with values from subtitle styles
	int outlineSizeLoc = GetShaderLocation(outlineShader, "outlineSize");
	int outlineColorLoc = GetShaderLocation(outlineShader, "outlineColor");
	int textureSizeLoc = GetShaderLocation(outlineShader, "textureSize");
	float textureSize[2] = { dims.x, dims.y };
	SetShaderValue(outlineShader, outlineSizeLoc, &outlineSize, SHADER_UNIFORM_INT);
	SetShaderValue(outlineShader, outlineColorLoc, m_subtitle.GetStyles().outline.outlineColor.values, SHADER_UNIFORM_VEC4);
	SetShaderValue(outlineShader, textureSizeLoc, textureSize, SHADER_UNIFORM_VEC2);

	textureSizeLoc = GetShaderLocation(shadowShader, "samplerSize");
	SetShaderValue(shadowShader, textureSizeLoc, textureSize, SHADER_UNIFORM_VEC2);
	SetShaderValue(shadowShader, GetShaderLocation(shadowShader, "color"), shadowColor, SHADER_UNIFORM_VEC3);
	SetShaderValue(shadowShader, GetShaderLocation(shadowShader, "offset"), shadowOffset, SHADER_UNIFORM_IVEC2);

	RenderTexture2D secondaryTex = LoadRenderTexture(dims.x, dims.y);
	RenderTexture2D pingpongBuffers[2] = { LoadRenderTexture(dims.x, dims.y) , LoadRenderTexture(dims.x, dims.y) };
	//We are drawing to a texture, with a shader used to draw SDF fonts
	BeginTextureMode(m_target);
	BeginShaderMode(SDFShader);
	//Texture is already sized to how it will be displayed, so just use the texture as the background and color it.
	ClearBackground(rlBLANK);
	std::istringstream textStream{ text };
	std::string line;
	std::string prevLine;
	int cumulativeHeight = 0;
	for (int i = 0; getline(textStream, line); i++, prevLine=line) {
		cumulativeHeight += prevLine.empty() ? 0 : MeasureTextEx(
			m_subtitle.GetFont(),
			prevLine.c_str(),
			m_subtitle.GetFontSize(),
			DEFAULT_SPACING).y;

		rlDrawTextEx(m_subtitle.GetFont(),
			line.c_str(),
			{ 
				(float)m_subtitle.GetStyles().outline.outlineSize,
				i * (float)m_subtitle.GetStyles().outline.outlineSize + cumulativeHeight
			},
			m_subtitle.GetFontSize(),
			DEFAULT_SPACING,
			fontColor);
	}
	EndShaderMode();
	EndTextureMode();

	//Second pass. Draw texture with outline shader enabled.
	//BeginTextureMode(pingpongBuffers[0]);
	////We draw rectangle for the background so that non-opaque text correctly
	////inherits the background color, rather than the color of the screen behind the window
	//BeginShaderMode(outlineShader);
	//DrawTextureRec(secondaryTex.texture, { 0, 0, dims.x, -dims.y }, { 0, 0 }, rlWHITE);
	//EndShaderMode();
	//EndTextureMode();

	//BeginTextureMode(pingpongBuffers[1]);
	//ClearBackground(rlBLANK);
	//BeginShaderMode(shadowShader);
	//DrawTextureRec(pingpongBuffers[0].texture, { 0, 0, dims.x, -dims.y }, { 0, 0 }, rlWHITE);
	//EndShaderMode();
	////DrawTextureRec(m_target.texture, { 0, 0, GetWindowDimensions().x, -GetWindowDimensions().y }, { 0, 0 }, WHITE);
	//EndTextureMode();

	//BeginTextureMode(m_target);
	//ClearBackground(rlBLANK);
	//DrawRectangle(0, 0, dims.x, dims.y, bgColor);
	//DrawTextureRec(pingpongBuffers[1].texture, { 0, 0, dims.x, -dims.y }, { 0, 0 }, rlWHITE);
	//DrawTextureRec(pingpongBuffers[0].texture, { 0, 0, dims.x, -dims.y }, { 0, 0 }, rlWHITE);
	//EndTextureMode();

	UnloadRenderTexture(secondaryTex);
	UnloadRenderTexture(pingpongBuffers[0]);
	UnloadRenderTexture(pingpongBuffers[1]);
}

std::string Window::WrapText(const std::string& text, const int& w, const int& h)
{
	int usableW = w - m_subtitle.GetPosition().x;
	int usableH = h - m_subtitle.GetPosition().y;
	std::istringstream textStream{text};
	std::ostringstream output{};

	std::string line;
	//Split based on user-provided newlines
	while (getline(textStream, line)) {
		//For each line, add words until line becomes longer than host width. If first word is longer than host width, let it overflow for now.
		std::istringstream lineStream{ line };
		std::string subLine;
		std::string word;
		while (getline(lineStream, word, ' ')) {
			if (MeasureTextEx(m_subtitle.GetFont(), (subLine + ' ' + word).c_str(), m_subtitle.GetFontSize(), DEFAULT_SPACING).x > usableW) {
				output << subLine << '\n';
				subLine.assign(word);
			}
			else {
				subLine.append(' ' + word);
			}
		}
		output << subLine << '\n';
	}
	return trim_copy(output.str());
}
