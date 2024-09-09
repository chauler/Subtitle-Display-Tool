#include <msdf-atlas-gen/msdf-atlas-gen.h>
#include <raylib.h>

class FontManager {
public:
    Font& GetFont(const std::string&);
    void SaveFont(const std::string&);
private:
    std::map<std::string, std::pair<Font, msdfgen::Bitmap<msdf_atlas::byte, 3>>> m_fontStorage;
};

std::pair<msdf_atlas::BitmapAtlasStorage<msdf_atlas::byte, 3>, std::vector<msdf_atlas::GlyphGeometry>> generateAtlas(const char*);
Font GenerateFontFromBitmap(msdfgen::BitmapRef<msdf_atlas::byte, 3>, std::vector<msdf_atlas::GlyphGeometry>&);
