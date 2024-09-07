#include "WindowManager.h"
#include <iostream>

#define SDF_SHADER_PATH "../../Subtitle-Display-Tool/res/shaders/sdf.fs"
#define MSDF_SHADER_PATH "../../Subtitle-Display-Tool/res/shaders/msdf.fs"
#define OUTLINE_SHADER_PATH "../../Subtitle-Display-Tool/res/shaders/outline.fs"
#define SHADOW_SHADER_PATH "../../Subtitle-Display-Tool/res/shaders/shadow.fs"

WindowManager::WindowManager(): 
	SDFShader(LoadShader(0, SDF_SHADER_PATH)),
	outlineShader(LoadShader(0, OUTLINE_SHADER_PATH)),
	shadowShader(LoadShader(0, SHADOW_SHADER_PATH)),
	MSDFShader(LoadShader(0, MSDF_SHADER_PATH)),
	hostX(0),
	hostY(0),
	hostWidth(0),
	hostHeight(0)
{

}

void WindowManager::AddWindow(const Window& window)
{
	m_windows.push_back(window);
}

void WindowManager::AddWindow(Window&& window)
{
	m_windows.push_back(std::move(window));
}

void WindowManager::DrawWindows()
{
	DrawConfig config{ .hostX{hostX},
		.hostY{hostY},
		.hostWidth{hostWidth},
		.hostHeight{hostHeight},
		.SDFShader{SDFShader},
		.outlineShader{outlineShader},
		.shadowShader{shadowShader},
		.MSDFShader{MSDFShader}
	};
	for (auto& window : m_windows) {
		window.Draw(config);
	}
}
