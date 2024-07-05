#pragma once
#include <string>

struct Vec2f {
	float x;
	float y;
};

struct Vec2i {
	int x;
	int y;
};

struct Vec3i {
	int x;
	int y;
	int z;
};

class Subtitle {
public:
	Subtitle(std::string dialogue, float fontSize=50, Vec2i pos={100, 100}, Vec3i rgb={255,255,255});
	float GetFontSize() const { return m_fontSize; }
	std::string GetDialogue() const { return m_dialogue; }
	Vec2i GetPosition() const { return m_position; }
	Vec3i GetColor() const { return m_color; }
private:
	std::string m_dialogue;
	//The following properties should eventually be placed into a Styles object.
	float m_fontSize = 50;
	Vec2i m_position{100, 100};
	Vec3i m_color{255, 255, 255};
};