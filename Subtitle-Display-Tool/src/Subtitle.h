#pragma once
#include <string>
#include "raylib.h"

struct Vec2f {
	float x;
	float y;
};

struct Vec4f {
	float x;
	float y;
	float z;
	float w;
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

//Modify individual channels using outlineColor.r, .g, .b, and .a
//Read all 4 floats using outlineColor.values
struct OutlineSettings {
	union {
		struct {
			float r;
			float g;
			float b;
			float a;
		};
		float values[4] = {1.0, 0.0, 0.0, 1.0};
	} outlineColor;
	int outlineSize = 2;
};

struct DropShadowSettings {
	union {
		struct {
			float r;
			float g;
			float b;
		};
		float values[3] = { 0.0, 0.0, 0.0 };
	} color;
	union {
		struct {
			int x;
			int y;
		};
		int values[2] = { 1, 1 };
	} offset;
	int blurStrength = 1;
};

struct Styles {
	float fontSize = 50;
	Vec2i position = {100, 100};
	Vec4i fontColor = {255, 255, 255, 255};
	Vec4i bgColor = {0, 0, 0, 255};
	std::string fontPath = "";
	double lifetime = 5.0;
	OutlineSettings outline = {};
	DropShadowSettings shadow = {};
};

class Subtitle {
public:
	Subtitle(std::string dialogue, Styles styles = {}, double startTime = 0.0);
	float GetFontSize() const { return m_styles.fontSize; }
	std::string GetDialogue() const { return m_dialogue; }
	Vec2i GetPosition() const { return m_styles.position; }
	Vec4i GetColor() const { return m_styles.fontColor; }
	Vec4i GetBackgroundColor() const { return m_styles.bgColor; }
	Font GetFont() const { return m_font; }
	double GetLifetime() const { return m_styles.lifetime; }
	double GetStartTime() const { return m_startTime; }
	Styles GetStyles() const { return m_styles; }
private:
	Font ConvertFontToSDF(std::string fontPath);
	std::string m_dialogue;
	Styles m_styles;
	Font m_font;
	double m_startTime;
};