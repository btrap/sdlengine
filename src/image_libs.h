#pragma once
#include <SDL_image.h>
struct Image
{
    SDL_Texture* texture;
    int width;
    int height;
};
Image image = { 0 };

struct Color
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
};

inline bool ImageInit()
{
    int imgFlags = IMG_INIT_PNG;
    int inittedFlags = IMG_Init(imgFlags);
    if((inittedFlags & imgFlags) != imgFlags)
    {
        // from document, not always that error string from IMG_GetError() will be set
        // so don't depend on it, just for pure information
        printf("SDL_Image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        return false;
    }

    return true;
}

void ImageRender(Image* image, int x, int y)
{
    if (image == NULL)
    {
        return;
    }

    // set rendering space and render to screen
    SDL_Rect renderQuad = { x, y, image->width, image->height };
    SDL_RenderCopy(sdl_State.renderer, image->texture, 0, &renderQuad);
}

void ImageFree(Image* image)
{
    // if exist then look inside their struct to deallocate, and reset stuff
    if (image != 0)
    {
        // destroy texture as attached to its texture
        if (image->texture != 0)
        {
            SDL_DestroyTexture(image->texture);
            image->texture = 0;
        }

        image->width = 0;
        image->height = 0;
    }
}

bool LoadImageFromFile(const char* path, Image* image)
{
    // free existing resource
    ImageFree(image);

    // load image at specified path
    SDL_Surface* loadedSurface = IMG_Load(path);
    if (loadedSurface == NULL)
    {
        printf("Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError());
        return false;
    }

    // color key image
    SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

    // create texture from surface
    SDL_Texture* newTexture = SDL_CreateTextureFromSurface(sdl_State.renderer, loadedSurface);
    if (newTexture == 0)
    {
        printf("Unable to create texture from %s! SDL Error: %s\n", path, SDL_GetError());
        // free surface
        SDL_FreeSurface(loadedSurface);
        return false;
    }

    // get image dimension
    image->width = loadedSurface->w;
    image->height = loadedSurface->h;
    // set texture to ltexture
    image->texture = newTexture;

    // free surface, we don't need it anymore
    SDL_FreeSurface(loadedSurface);

    return true;
}

void ShapeClearBackground(Color color);                                                  // Clear image background with given color
void ShapeDrawPoint(int posX, int posY, Color color);                                    // Draw pixel within an image
void ShapeDrawPoint(vec2 position, Color color);                                         // Draw pixel within an image (Vector version)
void ShapeDrawLine(int startPosX, int startPosY, int endPosX, int endPosY, Color color); // Draw line within an image
void ShapeDrawLine(vec2 start, vec2 end, Color color);                                   // Draw line within an image (Vector version)
void ShapeDrawCircle(int centerX, int centerY, int radius, Color color);                 // Draw circle within an image
void ShapeDrawCircle(vec2 center, int radius, Color color);                              // Draw circle within an image (Vector version)
void ShapeDrawRectangle(int posX, int posY, int width, int height, Color color);         // Draw rectangle within an image
void ShapeDrawRectangle(vec2 position, vec2 size, Color color);                          // Draw rectangle within an image (Vector version)

//void ShapeClearBackground(Color color = { 0x00, 0x00, 0x00, 0xFF })
//{
//}

void ShapeDrawPoint(int pos_X, int pos_Y, Color color)
{
    SDL_SetRenderDrawColor(sdl_State.renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawPoint(sdl_State.renderer, pos_X, pos_Y);
}

void ShapeDrawPoint(vec2 position, Color color)
{
    ShapeDrawPoint(position.x, position.y, color);
}

void ShapeDrawLine(int start_X, int start_Y, int end_X, int end_Y, Color color)
{
    SDL_SetRenderDrawColor(sdl_State.renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawLine(sdl_State.renderer, start_X, start_Y, end_X, end_Y);
}

void ShapeDrawLine(vec2 start, vec2 end, Color color)
{
    ShapeDrawLine(start.x, start.y, end.x, end.y, color);
}

void ShapeDrawCircle(int center_X, int center_Y, int radius, Color color)
{
    int offsetx = 0;
    int offsety = 0;
    int d = 0;
    int status = 0;

    offsety = radius;
    d = radius - 1;

    while (offsety >= offsetx)
    {
        ShapeDrawPoint(center_X + offsetx, center_Y + offsety, color);
        ShapeDrawPoint(center_X + offsety, center_Y + offsetx, color);
        ShapeDrawPoint(center_X - offsetx, center_Y + offsety, color);
        ShapeDrawPoint(center_X - offsety, center_Y + offsetx, color);
        ShapeDrawPoint(center_X + offsetx, center_Y - offsety, color);
        ShapeDrawPoint(center_X + offsety, center_Y - offsetx, color);
        ShapeDrawPoint(center_X - offsetx, center_Y - offsety, color);
        ShapeDrawPoint(center_X - offsety, center_Y - offsetx, color);

        if (d >= 2 * offsetx) {
            d -= 2 * offsetx + 1;
            offsetx += 1;
        }
        else if (d < 2 * (radius - offsety)) {
            d += 2 * offsety - 1;
            offsety -= 1;
        }
        else {
            d += 2 * (offsety - offsetx - 1);
            offsety -= 1;
            offsetx += 1;
        }
    }
}

void ShapeDrawCircle(vec2 center, int radius, Color color)
{
    ShapeDrawCircle(center.x, center.y, radius, color);
}

void ShapeDrawFillCircle(int center_X, int center_Y, int radius, Color color)
{
    int offsetx = 0;
    int offsety = 0;
    int d = 0;
    int status = 0;

    offsetx = 0;
    offsety = radius;
    d = radius - 1;

    while (offsety >= offsetx)
    {
        ShapeDrawLine(center_X - offsety, center_Y + offsetx, center_X + offsety, center_Y + offsetx, color);
        ShapeDrawLine(center_X - offsetx, center_Y + offsety, center_X + offsetx, center_Y + offsety, color);
        ShapeDrawLine(center_X - offsetx, center_Y - offsety, center_X + offsetx, center_Y - offsety, color);
        ShapeDrawLine(center_X - offsety, center_Y - offsetx, center_X + offsety, center_Y - offsetx, color);

        if (d >= 2 * offsetx) {
            d -= 2 * offsetx + 1;
            offsetx += 1;
        }
        else if (d < 2 * (radius - offsety)) {
            d += 2 * offsety - 1;
            offsety -= 1;
        }
        else {
            d += 2 * (offsety - offsetx - 1);
            offsety -= 1;
            offsetx += 1;
        }
    }
}

void ShapeDrawFillCircle(vec2 center, int radius, Color color)
{
    ShapeDrawCircle(center.x, center.y, radius, color);
}

//void ShapeDrawRectangle(int posX, int posY, int width, int height, Color color)
//{
//
//}
//
//void ShapeDrawRectangle(vec2 position, vec2 size, Color color)
//{
//}

//// Image loading functions
//// NOTE: This functions do not require GPU access
//Image LoadImage(const char* fileName);                                                             // Load image from file into CPU memory (RAM)
//Image LoadImageRaw(const char* fileName, int width, int height, int format, int headerSize);       // Load image from RAW file data
//Image LoadImageAnim(const char* fileName, int* frames);                                            // Load image sequence from file (frames appended to image.data)
//Image LoadImageFromMemory(const char* fileType, const unsigned char* fileData, int dataSize);      // Load image from memory buffer, fileType refers to extension: i.e. '.png'
//Image LoadImageFromTexture(Texture2D texture);                                                     // Load image from GPU texture data
//Image LoadImageFromScreen(void);                                                                   // Load image from screen buffer and (screenshot)
//void UnloadImage(Image image);                                                                     // Unload image from CPU memory (RAM)
//bool ExportImage(Image image, const char* fileName);                                               // Export image data to file, returns true on success
//bool ExportImageAsCode(Image image, const char* fileName);                                         // Export image as code file defining an array of bytes, returns true on success
//
//// Image generation functions
//Image GenImageColor(int width, int height, Color color);                                           // Generate image: plain color
//Image GenImageGradientV(int width, int height, Color top, Color bottom);                           // Generate image: vertical gradient
//Image GenImageGradientH(int width, int height, Color left, Color right);                           // Generate image: horizontal gradient
//Image GenImageGradientRadial(int width, int height, float density, Color inner, Color outer);      // Generate image: radial gradient
//Image GenImageChecked(int width, int height, int checksX, int checksY, Color col1, Color col2);    // Generate image: checked
//Image GenImageWhiteNoise(int width, int height, float factor);                                     // Generate image: white noise
//Image GenImageCellular(int width, int height, int tileSize);                                       // Generate image: cellular algorithm, bigger tileSize means bigger cells
//
//// Image manipulation functions
//Image ImageCopy(Image image);                                                                      // Create an image duplicate (useful for transformations)
//Image ImageFromImage(Image image, Rectangle rec);                                                  // Create an image from another image piece
//Image ImageText(const char* text, int fontSize, Color color);                                      // Create an image from text (default font)
//Image ImageTextEx(Font font, const char* text, float fontSize, float spacing, Color tint);         // Create an image from text (custom sprite font)
//void ImageFormat(Image* image, int newFormat);                                                     // Convert image data to desired format
//void ImageToPOT(Image* image, Color fill);                                                         // Convert image to POT (power-of-two)
//void ImageCrop(Image* image, Rectangle crop);                                                      // Crop an image to a defined rectangle
//void ImageAlphaCrop(Image* image, float threshold);                                                // Crop image depending on alpha value
//void ImageAlphaClear(Image* image, Color color, float threshold);                                  // Clear alpha channel to desired color
//void ImageAlphaMask(Image* image, Image alphaMask);                                                // Apply alpha mask to image
//void ImageAlphaPremultiply(Image* image);                                                          // Premultiply alpha channel
//void ImageResize(Image* image, int newWidth, int newHeight);                                       // Resize image (Bicubic scaling algorithm)
//void ImageResizeNN(Image* image, int newWidth, int newHeight);                                      // Resize image (Nearest-Neighbor scaling algorithm)
//void ImageResizeCanvas(Image* image, int newWidth, int newHeight, int offsetX, int offsetY, Color fill);  // Resize canvas and fill with color
//void ImageMipmaps(Image* image);                                                                   // Compute all mipmap levels for a provided image
//void ImageDither(Image* image, int rBpp, int gBpp, int bBpp, int aBpp);                            // Dither image data to 16bpp or lower (Floyd-Steinberg dithering)
//void ImageFlipVertical(Image* image);                                                              // Flip image vertically
//void ImageFlipHorizontal(Image* image);                                                            // Flip image horizontally
//void ImageRotateCW(Image* image);                                                                  // Rotate image clockwise 90deg
//void ImageRotateCCW(Image* image);                                                                 // Rotate image counter-clockwise 90deg
//void ImageColorTint(Image* image, Color color);                                                    // Modify image color: tint
//void ImageColorInvert(Image* image);                                                               // Modify image color: invert
//void ImageColorGrayscale(Image* image);                                                            // Modify image color: grayscale
//void ImageColorContrast(Image* image, float contrast);                                             // Modify image color: contrast (-100 to 100)
//void ImageColorBrightness(Image* image, int brightness);                                           // Modify image color: brightness (-255 to 255)
//void ImageColorReplace(Image* image, Color color, Color replace);                                  // Modify image color: replace color
//Color* LoadImageColors(Image image);                                                               // Load color data from image as a Color array (RGBA - 32bit)
//Color* LoadImagePalette(Image image, int maxPaletteSize, int* colorCount);                         // Load colors palette from image as a Color array (RGBA - 32bit)
//void UnloadImageColors(Color* colors);                                                             // Unload color data loaded with LoadImageColors()
//void UnloadImagePalette(Color* colors);                                                            // Unload colors palette loaded with LoadImagePalette()
//Rectangle GetImageAlphaBorder(Image image, float threshold);                                       // Get image alpha border rectangle
//Color GetImageColor(Image image, int x, int y);                                                    // Get image pixel color at (x, y) position
//
//// Image drawing functions
//// NOTE: Image software-rendering functions (CPU)
//void ImageClearBackground(Image* dst, Color color);                                                // Clear image background with given color
//void ImageDrawPixel(Image* dst, int posX, int posY, Color color);                                  // Draw pixel within an image
//void ImageDrawPixelV(Image* dst, Vector2 position, Color color);                                   // Draw pixel within an image (Vector version)
//void ImageDrawLine(Image* dst, int startPosX, int startPosY, int endPosX, int endPosY, Color color); // Draw line within an image
//void ImageDrawLineV(Image* dst, Vector2 start, Vector2 end, Color color);                          // Draw line within an image (Vector version)
//void ImageDrawCircle(Image* dst, int centerX, int centerY, int radius, Color color);               // Draw circle within an image
//void ImageDrawCircleV(Image* dst, Vector2 center, int radius, Color color);                        // Draw circle within an image (Vector version)
//void ImageDrawRectangle(Image* dst, int posX, int posY, int width, int height, Color color);       // Draw rectangle within an image
//void ImageDrawRectangleV(Image* dst, Vector2 position, Vector2 size, Color color);                 // Draw rectangle within an image (Vector version)
//void ImageDrawRectangleRec(Image* dst, Rectangle rec, Color color);                                // Draw rectangle within an image
//void ImageDrawRectangleLines(Image* dst, Rectangle rec, int thick, Color color);                   // Draw rectangle lines within an image
//void ImageDraw(Image* dst, Image src, Rectangle srcRec, Rectangle dstRec, Color tint);             // Draw a source image within a destination image (tint applied to source)
//void ImageDrawText(Image* dst, const char* text, int posX, int posY, int fontSize, Color color);   // Draw text (using default font) within an image (destination)
//void ImageDrawTextEx(Image* dst, Font font, const char* text, Vector2 position, float fontSize, float spacing, Color tint); // Draw text (custom sprite font) within an image (destination)
//
//// Texture loading functions
//// NOTE: These functions require GPU access
//Texture2D LoadTexture(const char* fileName);                                                       // Load texture from file into GPU memory (VRAM)
//Texture2D LoadTextureFromImage(Image image);                                                       // Load texture from image data
//TextureCubemap LoadTextureCubemap(Image image, int layout);                                        // Load cubemap from image, multiple image cubemap layouts supported
//RenderTexture2D LoadRenderTexture(int width, int height);                                          // Load texture for rendering (framebuffer)
//void UnloadTexture(Texture2D texture);                                                             // Unload texture from GPU memory (VRAM)
//void UnloadRenderTexture(RenderTexture2D target);                                                  // Unload render texture from GPU memory (VRAM)
//void UpdateTexture(Texture2D texture, const void* pixels);                                         // Update GPU texture with new data
//void UpdateTextureRec(Texture2D texture, Rectangle rec, const void* pixels);                       // Update GPU texture rectangle with new data
//
//// Texture configuration functions
//void GenTextureMipmaps(Texture2D* texture);                                                        // Generate GPU mipmaps for a texture
//void SetTextureFilter(Texture2D texture, int filter);                                              // Set texture scaling filter mode
//void SetTextureWrap(Texture2D texture, int wrap);                                                  // Set texture wrapping mode
//
//// Texture drawing functions
//void DrawTexture(Texture2D texture, int posX, int posY, Color tint);                               // Draw a Texture2D
//void DrawTextureV(Texture2D texture, Vector2 position, Color tint);                                // Draw a Texture2D with position defined as Vector2
//void DrawTextureEx(Texture2D texture, Vector2 position, float rotation, float scale, Color tint);  // Draw a Texture2D with extended parameters
//void DrawTextureRec(Texture2D texture, Rectangle source, Vector2 position, Color tint);            // Draw a part of a texture defined by a rectangle
//void DrawTextureQuad(Texture2D texture, Vector2 tiling, Vector2 offset, Rectangle quad, Color tint);  // Draw texture quad with tiling and offset parameters
//void DrawTextureTiled(Texture2D texture, Rectangle source, Rectangle dest, Vector2 origin, float rotation, float scale, Color tint); // Draw part of a texture (defined by a rectangle) with rotation and scale tiled into dest.
//void DrawTexturePro(Texture2D texture, Rectangle source, Rectangle dest, Vector2 origin, float rotation, Color tint);           // Draw a part of a texture defined by a rectangle with 'pro' parameters
//void DrawTextureNPatch(Texture2D texture, NPatchInfo nPatchInfo, Rectangle dest, Vector2 origin, float rotation, Color tint);   // Draws a texture (or part of it) that stretches or shrinks nicely
//void DrawTexturePoly(Texture2D texture, Vector2 center, Vector2* points, Vector2* texcoords, int pointCount, Color tint);       // Draw a textured polygon
//
//// Color/pixel related functions
//Color Fade(Color color, float alpha);                                 // Get color with alpha applied, alpha goes from 0.0f to 1.0f
//int ColorToInt(Color color);                                          // Get hexadecimal value for a Color
//Vector4 ColorNormalize(Color color);                                  // Get Color normalized as float [0..1]
//Color ColorFromNormalized(Vector4 normalized);                        // Get Color from normalized values [0..1]
//Vector3 ColorToHSV(Color color);                                      // Get HSV values for a Color, hue [0..360], saturation/value [0..1]
//Color ColorFromHSV(float hue, float saturation, float value);         // Get a Color from HSV values, hue [0..360], saturation/value [0..1]
//Color ColorAlpha(Color color, float alpha);                           // Get color with alpha applied, alpha goes from 0.0f to 1.0f
//Color ColorAlphaBlend(Color dst, Color src, Color tint);              // Get src alpha-blended into dst color with tint
//Color GetColor(unsigned int hexValue);                                // Get Color structure from hexadecimal value
//Color GetPixelColor(void* srcPtr, int format);                        // Get Color from a source pixel pointer of certain format
//void SetPixelColor(void* dstPtr, Color color, int format);            // Set color formatted into destination pixel pointer
//int GetPixelDataSize(int width, int height, int format);              // Get pixel data size in bytes for certain format
