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

class Subtitle {
public:
	Subtitle(std::string dialogue);
	float GetFontSize() { return m_fontSize; }
	std::string GetDialogue() { return m_dialogue; }
	Vec2i GetPosition() { return m_position; }
private:
	std::string m_dialogue;
	//The following properties should eventually be placed into a Styles object.
	float m_fontSize = 50;
	Vec2i m_position{100, 100};
};