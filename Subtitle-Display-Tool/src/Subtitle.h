#pragma once
#include <string>
#include "raylib.h"

struct Vec2f {
	float x;
	float y;
};

struct Vec2i {
	int x;
	int y;
};

struct Vec4i {
	int x;
	int y;
	int z;
	int w;
};

struct Styles {
	float fontSize = 50;
	Vec2i position = {100, 100};
	Vec4i fontColor = {255, 255, 255, 255};
	Vec4i bgColor = {0, 0, 0, 255};
	std::string fontPath = "";
	double lifetime = 5.0;
};

class Subtitle {
public:
	Subtitle(std::string dialogue, Styles styles = {});
	float GetFontSize() const { return m_styles.fontSize; }
	std::string GetDialogue() const { return m_dialogue; }
	Vec2i GetPosition() const { return m_styles.position; }
	Vec4i GetColor() const { return m_styles.fontColor; }
	Vec4i GetBackgroundColor() const { return m_styles.bgColor; }
	Font GetFont() const { return m_font; }
	double GetLifetime() const { return m_styles.lifetime; }
private:
	std::string m_dialogue;
	Styles m_styles;
	Font m_font;
};