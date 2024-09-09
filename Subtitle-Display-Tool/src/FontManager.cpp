#include "FontManager.h"

Font& FontManager::GetFont(const std::string& fontPath)
{
    SaveFont(fontPath);
    auto it = m_fontStorage.find(fontPath);
    return it->second.first;
}

void FontManager::SaveFont(const std::string& fontPath)
{
    //Exit early if key already exists
    if (m_fontStorage.find(fontPath) != m_fontStorage.end()) {
        return;
    }

    std::pair<msdf_atlas::BitmapAtlasStorage<msdf_atlas::byte, 3>, std::vector<msdf_atlas::GlyphGeometry>> atlasFontPair = generateAtlas(fontPath.c_str());
    msdfgen::Bitmap<msdf_atlas::byte, 3> bitmap = (msdfgen::Bitmap<msdf_atlas::byte, 3>&&) atlasFontPair.first;
    Font font = GenerateFontFromBitmap((msdfgen::BitmapRef<msdf_atlas::byte, 3>)bitmap, atlasFontPair.second);
    std::pair<Font, msdfgen::Bitmap<msdf_atlas::byte, 3>> valuePair{font, std::move(bitmap)};
    std::pair<std::string, std::pair<Font, msdfgen::Bitmap<msdf_atlas::byte, 3>>> kvPair{ fontPath, std::move(valuePair) };
    m_fontStorage.emplace(std::move(kvPair));
}

std::pair<msdf_atlas::BitmapAtlasStorage<msdf_atlas::byte, 3>, std::vector<msdf_atlas::GlyphGeometry>> generateAtlas(const char* fontFilename) {
    bool success = false;
    std::vector<msdf_atlas::GlyphGeometry> glyphs;
    if (msdfgen::FreetypeHandle* ft = msdfgen::initializeFreetype()) {
        if (msdfgen::FontHandle* font = msdfgen::loadFont(ft, fontFilename)) {
            msdf_atlas::FontGeometry fontGeometry(&glyphs);
            fontGeometry.loadCharset(font, 1.0, msdf_atlas::Charset::ASCII);
            const double maxCornerAngle = 3.0;
            for (msdf_atlas::GlyphGeometry& glyph : glyphs)
                glyph.edgeColoring(&msdfgen::edgeColoringInkTrap, maxCornerAngle, 0);
            msdf_atlas::TightAtlasPacker packer;
            packer.setDimensionsConstraint(msdf_atlas::DimensionsConstraint::SQUARE);
            packer.setScale(48.0);
            packer.setPixelRange(4.0);
            packer.setMiterLimit(1.0);
            packer.pack(glyphs.data(), glyphs.size());
            int width = 0, height = 0;
            packer.getDimensions(width, height);
            msdf_atlas::ImmediateAtlasGenerator<
                float,
                3,
                msdf_atlas::msdfGenerator,
                msdf_atlas::BitmapAtlasStorage<msdf_atlas::byte, 3>
            > generator(width, height);
            //msdf_atlas::GeneratorAttributes attributes;
            //generator.setAttributes(attributes);
            generator.setThreadCount(4);
            generator.generate(glyphs.data(), glyphs.size());
            msdf_atlas::BitmapAtlasStorage<msdf_atlas::byte, 3> image{ generator.atlasStorage() };
            msdfgen::destroyFont(font);
            msdfgen::deinitializeFreetype(ft);
            return { image, glyphs };
        }
        msdfgen::deinitializeFreetype(ft);
    }
}

Font GenerateFontFromBitmap(msdfgen::BitmapRef<msdf_atlas::byte, 3> bitmap, std::vector<msdf_atlas::GlyphGeometry>& glyphs) {
    Image fontImage = { .data{bitmap.pixels},
    .width{bitmap.width},
    .height{bitmap.height},
    .mipmaps{1},
    .format{PIXELFORMAT_UNCOMPRESSED_R8G8B8} };
    ImageFlipVertical(&fontImage);
    Texture2D tex = LoadTextureFromImage(fontImage);

    size_t numGlyphs = glyphs.size();
    Font outputFont = { 0 };
    outputFont.baseSize = 48;
    outputFont.glyphCount = numGlyphs;
    outputFont.glyphPadding = 0;
    outputFont.texture = tex;
    outputFont.recs = new rlRectangle[numGlyphs];
    outputFont.glyphs = new GlyphInfo[numGlyphs];

    SetTextureFilter(outputFont.texture, TEXTURE_FILTER_BILINEAR);

    for (unsigned int i = 0; i < numGlyphs; i++) {
        const msdf_atlas::GlyphGeometry& glyph = glyphs[i];
        outputFont.glyphs[i].value = glyph.getCodepoint();
        outputFont.recs[i].x = (float)glyph.getBoxRect().x;
        outputFont.recs[i].y = bitmap.height - (float)glyph.getBoxRect().y - (float)glyph.getBoxRect().h;
        outputFont.recs[i].width = (float)glyph.getBoxRect().w;
        outputFont.recs[i].height = (float)glyph.getBoxRect().h;
        outputFont.glyphs[i].offsetX = 0;
        outputFont.glyphs[i].offsetY = 0;
        outputFont.glyphs[i].advanceX = 0;
        outputFont.glyphs[i].image = ImageFromImage(fontImage, outputFont.recs[i]);
    }
    return outputFont;
}