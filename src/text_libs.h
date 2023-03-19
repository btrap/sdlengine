#pragma once
#include <SDL_ttf.h>

struct Font
{
    TTF_Font* trueTypeFont;
};
Font defaultFont = {0};

bool ImageLoadFromRenderTexture(Font *font, const char* textureText, SDL_Color textColor, Uint32 wrapLength, Image* image)
{
    // free existing resource
    ImageFree(image);

    // render text surface
    SDL_Surface* textSurface = 0;

    // if wrapLength is 0, then render without auto-wrapping support
    if (wrapLength == 0)
    {
        textSurface = TTF_RenderText_Solid(font->trueTypeFont, textureText, textColor);
    }
    else
    {
        textSurface = TTF_RenderText_Blended_Wrapped(font->trueTypeFont, textureText, textColor, wrapLength);
    }

    if (textSurface == 0)
    {
        printf("Unable to render text surface! SDL_ttf error: %s\n", TTF_GetError());
        return false;
    }

    // create texture from surface pixels
    SDL_Texture* newTexture = SDL_CreateTextureFromSurface(sdl_State.renderer, textSurface);
    if (newTexture == 0)
    {
        printf("Unable to create texture from rendered text! SDL error: %s\n", SDL_GetError());
        SDL_FreeSurface(textSurface);
        return false;
    }

    // get image's dimensions
    image->width = textSurface->w;
    image->height = textSurface->h;
    image->texture = newTexture;

    // free surface, we don't need it anymore
    SDL_FreeSurface(textSurface);

    return true;
}

// Font loading/unloading functions
bool FontInit();
Font* GetFontDefault(void); // Get the default Font
Font* LoadFont(const char* fileName); // Load font from file into GPU memory (VRAM)

inline bool FontInit()
{
    // initialize SDL_ttf
    if(TTF_Init() == -1)
    {
        printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        return false;
    }

    GetFontDefault();

    return true;
}

void CloseFont();
inline void CloseFont()
{
    TTF_CloseFont(defaultFont.trueTypeFont);
    TTF_Quit();
}

Font* GetFontDefault(void)
{
    LoadFont("C:\\Users\\brand\\Software\\GitHub\\solitaireGame\\resources\\font\\temp.ttf");

    SDL_Color textColor = { 0, 255, 0, 255 };
    if (!ImageLoadFromRenderTexture(&defaultFont, "The quick brown fox jumps over the lazy dog", textColor, 200, &image))
    {
        printf("Failed to render text texture\n");
        return 0;
    }

    return &defaultFont;
}

Font* LoadFont(const char* fileName)
{
    defaultFont.trueTypeFont = TTF_OpenFont(fileName, 18);
    if (defaultFont.trueTypeFont == 0)
    {
        printf("Failed to load font! SDL_ttf error: %s\n", TTF_GetError());
        return 0;
    }

    return &defaultFont;
}

//Font LoadFontEx(const char* fileName, int fontSize, int* fontChars, int glyphCount);  // Load font from file with extended parameters, use NULL for fontChars and 0 for glyphCount to load the default character set
//Font LoadFontFromImage(Image image, Color key, int firstChar);                        // Load font from Image (XNA style)
//Font LoadFontFromMemory(const char* fileType, const unsigned char* fileData, int dataSize, int fontSize, int* fontChars, int glyphCount); // Load font from memory buffer, fileType refers to extension: i.e. '.ttf'
//GlyphInfo* LoadFontData(const unsigned char* fileData, int dataSize, int fontSize, int* fontChars, int glyphCount, int type); // Load font data for further use
//Image GenImageFontAtlas(const GlyphInfo* chars, Rectangle** recs, int glyphCount, int fontSize, int padding, int packMethod); // Generate image font atlas using chars info
//void UnloadFontData(GlyphInfo* chars, int glyphCount);                                // Unload font chars info data (RAM)
//void UnloadFont(Font font);                                                           // Unload font from GPU memory (VRAM)
//bool ExportFontAsCode(Font font, const char* fileName);                               // Export font as code file, returns true on success
//
//// Text drawing functions
//void DrawFPS(int posX, int posY);                                                     // Draw current FPS
//void DrawText(const char* text, int posX, int posY, int fontSize, Color color);       // Draw text (using default font)
//void DrawTextEx(Font font, const char* text, Vector2 position, float fontSize, float spacing, Color tint); // Draw text using font and additional parameters
//void DrawTextPro(Font font, const char* text, Vector2 position, Vector2 origin, float rotation, float fontSize, float spacing, Color tint); // Draw text using Font and pro parameters (rotation)
//void DrawTextCodepoint(Font font, int codepoint, Vector2 position, float fontSize, Color tint); // Draw one character (codepoint)
//void DrawTextCodepoints(Font font, const int* codepoints, int count, Vector2 position, float fontSize, float spacing, Color tint); // Draw multiple character (codepoint)

