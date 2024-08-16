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

/**
* Struct containing data for the configuration of the outline in the Styles object.
*/
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
	OutlineSettings() {}
	OutlineSettings(float* color, int size): outlineSize(size) {
		for (int i = 0; i < 4; i++) {
			if (color[i] < 0) {
				outlineColor.values[i] = 0;
			}
			else if (color[i] > 1) {
				outlineColor.values[i] = 1;
			}
			else {
				outlineColor.values[i] = color[i];
			}
		}
	}
};

/**
* Struct containing data for the configuration of the drop shadow in the Styles object.
*/
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

/**
* Used to hold all data determining how a given piece of dialogue is displayed.
*/
struct Styles {
	float fontSize = 50;
	Vec2i position = {100, 100};
	Vec4i fontColor = {255, 255, 255, 255};
	Vec4i bgColor = {0, 0, 0, 0};
	std::string fontPath = "";
	double lifetime = 5.0;
	OutlineSettings outline = {};
	DropShadowSettings shadow = {};
};

/**
* Data class used to store dialogue and all the styling information for that piece of dialogue.
*/
class Subtitle {
public:
	Subtitle(std::string dialogue, Styles styles = {}, double startTime = 0.0);
	/**
	* Returns the subtitle's font size.
	*/
	float GetFontSize() const { return m_styles.fontSize; }
	/**
	* Returns the subtitle's dialogue.
	*/
	std::string GetDialogue() const { return m_dialogue; }
	/**
	* Returns the subtitle's position.
	*/
	Vec2i GetPosition() const { return m_styles.position; }
	/**
	* Returns the subtitle's font color.
	*/
	Vec4i GetColor() const { return m_styles.fontColor; }
	/**
	* Returns the subtitle's background color.
	*/
	Vec4i GetBackgroundColor() const { return m_styles.bgColor; }
	/**
	* Returns the subtitle's font face.
	*/
	Font GetFont() const { return m_font; }
	/**
	* Returns the subtitle's lifetime.
	*/
	double GetLifetime() const { return m_styles.lifetime; }
	/**
	* Returns the subtitle's start time.
	*/
	double GetStartTime() const { return m_startTime; }
	/**
	* Returns the subtitle's Styles object.
	*/
	Styles GetStyles() const { return m_styles; }
private:
	Font ConvertFontToSDF(std::string fontPath);
	std::string m_dialogue;
	Styles m_styles;
	Font m_font;
	double m_startTime;
};