#include "Subtitle.h"

Subtitle::Subtitle(std::string dialogue, float fontSize, Vec2i pos, Vec3i rgb): 
	m_dialogue(dialogue),
	m_fontSize(fontSize),
	m_position(pos),
	m_color(rgb)
{

}