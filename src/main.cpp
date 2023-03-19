//Using SDL and standard IO
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

struct vec2
{
    int x;
    int y;
};

#include <SDL.h>

#include <SDL_opengl.h>
//#include <gl/gl.h>
//#include "SDL_opengles.h"
// #include <SDL_test_common.h>

struct Platform_State
{
    int gl_Red_Size;
    int gl_Green_Size;
    int gl_Blue_Size;
    int gl_Depth_Size;

    int gl_Accelerated;
    int gl_Multisample_Buffers;
    int gl_Multisample_Samples;
    unsigned int render_Flags;

    int window_X;
    int window_Y;
    int window_Width;
    int window_Height;
    int window_Min_Width;
    int window_Min_Height;
    int window_Max_Width;
    int window_Max_Height;

    int mouse_X;
    int mouse_Y;
    int mouse_Pressed[3];
    int mouse_Released[3];

    unsigned int window_flags;
    bool quit;
};

struct SDL_State
{
    Platform_State* platform_State;
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Event event;
};
static SDL_State sdl_State = {};

#include "system_defines.h"
#include "memory.h"

#include "image_libs.h"
#include "text_libs.h"
#include "platform_libs.h"

inline void DrawPlayer()
{
    ShapeDrawFillCircle(100, 100, 20, { 128, 0, 0, 255 });
}

inline void DrawGrid()
{
    int rect_Size = 64;
    SDL_SetRenderDrawColor(sdl_State.renderer, 255, 255, 255, 255);
    SDL_Rect newRect = { 0, 0, 0, 0 };
    for( int i = 0; i < ( GetScreenWidth() / rect_Size ) + 1; ++i )
    {
        for( int j = 0; j < ( GetScreenHeight() / rect_Size ) + 1; ++j )
        {
            newRect = { i * rect_Size, j * rect_Size, ( i + 1 ) * rect_Size, ( j + 1 ) * rect_Size };
            SDL_RenderDrawRect(sdl_State.renderer, &newRect);
        }
    }
}

int main(int argc, char** argv)
{
    SetupMemoryPools();
    Platform_State *platform_State = 0;
    STORE_PERM_DATA(platform_State, 1);
    PlatformSetupInit(platform_State);

    bool success = InitWindow(1980 / 2, 1080 / 2, "Hello World");
    if(!success)
        return false;

    ImageInit();
    FontInit();

    while(!WindowShouldClose())
    {
        while(PollEvent())
        {
            HandleEvent(); // Should window close end it all?
        }

        ClearWindow();
        DrawGrid();
        DrawPlayer();
        UpdateWindow();
    }

    CloseWindow();

    return 0;
}