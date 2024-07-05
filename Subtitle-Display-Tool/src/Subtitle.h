#pragma once
#include <string>

class Subtitle {
public:
	Subtitle(std::string dialogue);
	float GetFontSize() { return m_fontSize; }
	std::string GetDialogue() { return m_dialogue; }
private:
	std::string m_dialogue;
	float m_fontSize = 100;
};