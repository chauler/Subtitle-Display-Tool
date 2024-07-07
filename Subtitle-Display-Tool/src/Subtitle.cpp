#include "Subtitle.h"

Subtitle::Subtitle(std::string dialogue, Styles styles): 
	m_dialogue(dialogue),
	m_styles(styles),
	m_font(m_styles.fontPath.empty() ? GetFontDefault() : LoadFont(m_styles.fontPath.c_str()))
{
}