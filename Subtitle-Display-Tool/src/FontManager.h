#include <msdf-atlas-gen/msdf-atlas-gen.h>
#include <raylib.h>

class FontManager {
public:
    Font GetFont();
    void SaveFont();
private:
    std::map<std::string, Font> fontStorage;
};

Font generateAtlas(const char* fontFilename);