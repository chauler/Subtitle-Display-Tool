#include <msdf-atlas-gen/msdf-atlas-gen.h>
#include <raylib.h>

//class FontManager {
//public:
//    Font GetFont();
//    void SaveFont();
//private:
//    std::map<std::string, Font> fontStorage;
//};

msdf_atlas::BitmapAtlasStorage<msdf_atlas::byte, 3> image;

struct AtlasInfo {
    std::vector<msdf_atlas::GlyphGeometry> fontGeometry;
    msdf_atlas::BitmapAtlasStorage<msdf_atlas::byte, 3> image;
};

Font generateAtlas(const char* fontFilename) {
    bool success = false;
    std::vector<msdf_atlas::GlyphGeometry> glyphs;
    // Initialize instance of FreeType library
    if (msdfgen::FreetypeHandle* ft = msdfgen::initializeFreetype()) {
        // Load font file
        if (msdfgen::FontHandle* font = msdfgen::loadFont(ft, fontFilename)) {
            // Storage for glyph geometry and their coordinates in the atlas
            // FontGeometry is a helper class that loads a set of glyphs from a single font.
            // It can also be used to get additional font metrics, kerning information, etc.
            msdf_atlas::FontGeometry fontGeometry(&glyphs);
            // Load a set of character glyphs:
            // The second argument can be ignored unless you mix different font sizes in one atlas.
            // In the last argument, you can specify a charset other than ASCII.
            // To load specific glyph indices, use loadGlyphs instead.
            fontGeometry.loadCharset(font, 1.0, msdf_atlas::Charset::ASCII);
            // Apply MSDF edge coloring. See edge-coloring.h for other coloring strategies.
            const double maxCornerAngle = 3.0;
            for (msdf_atlas::GlyphGeometry& glyph : glyphs)
                glyph.edgeColoring(&msdfgen::edgeColoringInkTrap, maxCornerAngle, 0);
            // TightAtlasPacker class computes the layout of the atlas.
            msdf_atlas::TightAtlasPacker packer;
            // Set atlas parameters:
            // setDimensions or setDimensionsConstraint to find the best value
            packer.setDimensionsConstraint(msdf_atlas::DimensionsConstraint::SQUARE);
            // setScale for a fixed size or setMinimumScale to use the largest that fits
            packer.setScale(48.0);
            // setPixelRange or setUnitRange
            packer.setPixelRange(4.0);
            packer.setMiterLimit(1.0);
            // Compute atlas layout - pack glyphs
            packer.pack(glyphs.data(), glyphs.size());
            // Get final atlas dimensions
            int width = 0, height = 0;
            packer.getDimensions(width, height);
            // The ImmediateAtlasGenerator class facilitates the generation of the atlas bitmap.
            msdf_atlas::ImmediateAtlasGenerator<
                float, // pixel type of buffer for individual glyphs depends on generator function
                3, // number of atlas color channels
                msdf_atlas::msdfGenerator, // function to generate bitmaps for individual glyphs
                msdf_atlas::BitmapAtlasStorage<msdf_atlas::byte, 3> // class that stores the atlas bitmap
                // For example, a custom atlas storage class that stores it in VRAM can be used.
            > generator(width, height);
            // GeneratorAttributes can be modified to change the generator's default settings.
            //msdf_atlas::GeneratorAttributes attributes;
            //generator.setAttributes(attributes);
            //generator.setThreadCount(4);
            // Generate atlas bitmap
            generator.generate(glyphs.data(), glyphs.size());
            // The atlas bitmap can now be retrieved via atlasStorage as a BitmapConstRef.
            // The glyphs array (or fontGeometry) contains positioning data for typesetting text.
            image = generator.atlasStorage();
            // Cleanup
            msdfgen::destroyFont(font);
        }
        msdfgen::deinitializeFreetype(ft);
    }
    
    msdfgen::BitmapRef<msdf_atlas::byte, 3> bitmap = (msdfgen::BitmapRef<msdf_atlas::byte, 3>)image;
    
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
    UnloadImage(fontImage);

    return outputFont;
}