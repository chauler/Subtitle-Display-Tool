#include "Subtitle.h"

Subtitle::Subtitle(std::string dialogue, Styles styles): 
	m_dialogue(dialogue),
	m_styles(styles),
	m_font(m_styles.fontPath.empty() ? GetFontDefault() : ConvertFontToSDF(m_styles.fontPath.c_str()))
{
}

Font Subtitle::ConvertFontToSDF(std::string fontPath)
{
	//Load file data to work with when generating SDF font
	int fileSize = 0;
	unsigned char* fileData = LoadFileData(fontPath.c_str(), &fileSize);

	Font baseFont = LoadFont(fontPath.c_str());
	Font sdfFont{.baseSize = baseFont.baseSize, .glyphCount = baseFont.glyphCount};
	sdfFont.glyphs = LoadFontData(fileData, fileSize, baseFont.baseSize, 0, baseFont.glyphCount, FONT_SDF);
	Image atlas = GenImageFontAtlas(sdfFont.glyphs, &sdfFont.recs, baseFont.glyphCount, baseFont.baseSize, 0, 1);
	sdfFont.texture = LoadTextureFromImage(atlas);
	SetTextureFilter(sdfFont.texture, TEXTURE_FILTER_BILINEAR);
	UnloadImage(atlas);
	UnloadFileData(fileData);

	return sdfFont;
}
