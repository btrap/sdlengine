#pragma once

void PlatformSetupInit(Platform_State* platform_State)
{
    sdl_State.platform_State = platform_State;
}

void PlatformSetupDimensions(Platform_State* platform_State, int width, int height)
{
    platform_State->window_Height = height;
    platform_State->window_Width = width;
}

//// 640 x 480 (min), 960 x 540, 1280 x 720, 1920 x 1080, 2048 x 1080, 4096 x 2160 (max)
//platform_State.window_Width = 640;
//platform_State.window_Height = 480;
//platform_State.window_Min_Width = 640;
//platform_State.window_Min_Height = 480;
//platform_State.window_Max_Width = 4096;
//platform_State.window_Max_Height = 2160;

int GetMonitorWidth(int monitor); // Get specified monitor width (max available by monitor)
int GetMonitorHeight(int monitor); // Get specified monitor height (max available by monitor)
int GetMonitorPhysicalWidth(int monitor); // Get specified monitor physical width in millimetres
int GetMonitorPhysicalHeight(int monitor); // Get specified monitor physical height in millimetres
int GetMonitorRefreshRate(int monitor); // Get specified monitor refresh rate
vec2 GetWindowPosition(); // Get window position XY on monitor
vec2 GetWindowScaleDPI(); // Get window scale DPI factor
bool InitWindow(int width, int height, const char* title); // Initialize window and OpenGL context
void UpdateWindow();
void CloseWindow();// Close window and unload OpenGL context
bool WindowShouldClose(); // Check if KEY_ESCAPE pressed or Close icon pressed
bool IsWindowReady(); // Check if window has been initialized successfully
bool IsWindowFullscreen(); // Check if window is currently fullscreen
bool IsWindowHidden(); // Check if window is currently hidden (only PLATFORM_DESKTOP)
bool IsWindowMinimized(); // Check if window is currently minimized (only PLATFORM_DESKTOP)
bool IsWindowMaximized(); // Check if window is currently maximized (only PLATFORM_DESKTOP)
bool IsWindowFocused(); // Check if window is currently focused (only PLATFORM_DESKTOP)
bool IsWindowResized(); // Check if window has been resized last frame
bool IsWindowState(unsigned int flag); // Check if one specific window flag is enabled
void SetWindowState(unsigned int flags); // Set window configuration state using flags
void ClearWindowState(unsigned int flags); // Clear window configuration state flags
void ToggleFullscreen(); // Toggle window state: fullscreen/windowed (only PLATFORM_DESKTOP)
void MaximizeWindow(); // Set window state: maximized, if resizable (only PLATFORM_DESKTOP)
void MinimizeWindow(); // Set window state: minimized, if resizable (only PLATFORM_DESKTOP)
void RestoreWindow(); // Set window state: not minimized/maximized (only PLATFORM_DESKTOP)
void SetWindowTitle(const char* title); // Set title for window (only PLATFORM_DESKTOP)
void SetWindowPosition(int x, int y); // Set window position on screen (only PLATFORM_DESKTOP)
void SetWindowMonitor(int monitor); // Set monitor for the current window (fullscreen mode)
void SetWindowMinSize(int width, int height); // Set window minimum dimensions (for FLAG_WINDOW_RESIZABLE)
void SetWindowSize(int width, int height); // Set window dimensions
int GetScreenWidth(); // Get current screen width
int GetScreenHeight(); // Get current screen height
int GetMonitorCount(); // Get number of connected monitors
int GetCurrentMonitor(); // Get current connected monitor
vec2 GetMonitorPosition(int monitor); // Get specified monitor position
const char* GetMonitorName(int monitor); // Get the human-readable, UTF-8 encoded name of the primary monitor
void SetClipboardText(const char* text); // Set clipboard text content
const char* GetClipboardText(); // Get clipboard text content
void* GetWindowHandle(); // Get native window handle
bool PollEvent();
void HandleEvent();
bool IsKeyPressed(int key); // Check if a key has been pressed once

// NOT DONE

bool IsKeyDown(int key); // Check if a key is being pressed
bool IsKeyReleased(int key); // Check if a key has been released once
bool IsKeyUp(int key); // Check if a key is NOT being pressed
void SetExitKey(int key); // Set a custom key to exit program (default is ESC)
int GetKeyPressed(void); // Get key pressed (keycode), call it multiple times for keys queued, returns 0 when the queue is empty
int GetCharPressed(void); // Get char pressed (unicode), call it multiple times for chars queued, returns 0 when the queue is empty

// Input-related functions: mouse
bool IsMouseButtonPressed(int button);                      // Check if a mouse button has been pressed once
bool IsMouseButtonDown(int button);                         // Check if a mouse button is being pressed
bool IsMouseButtonReleased(int button);                     // Check if a mouse button has been released once
bool IsMouseButtonUp(int button);                           // Check if a mouse button is NOT being pressed
int GetMouseX(void); // Get mouse position X
int GetMouseY(void); // Get mouse position Y
vec2 GetMousePosition(void); // Get mouse position XY
vec2 GetMouseDelta(void);                                   // Get mouse delta between frames
void SetMousePosition(int x, int y);                        // Set mouse position XY
void SetMouseOffset(int offsetX, int offsetY);              // Set mouse offset
void SetMouseScale(float scaleX, float scaleY);             // Set mouse scaling
float GetMouseWheelMove(void);                              // Get mouse wheel movement for X or Y, whichever is larger
vec2 GetMouseWheelMoveV(void);                              // Get mouse wheel movement for both X and Y
void SetMouseCursor(int cursor);                            // Set mouse cursor

//int GetMonitorWidth(int monitor);
//int GetMonitorHeight(int monitor);
//int GetMonitorPhysicalWidth(int monitor);
//int GetMonitorPhysicalHeight(int monitor);
//int GetMonitorRefreshRate(int monitor);
//vec2 GetWindowPosition();
//vec2 GetWindowScaleDPI();

bool InitWindow(int width, int height, const char* title)
{
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        // Message
        return false;
    }

    SetWindowSize(width, height);

    sdl_State.window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
    if(!sdl_State.window)
    {
        return false;
    }

    sdl_State.renderer = SDL_CreateRenderer(sdl_State.window, -1, SDL_RENDERER_ACCELERATED);

    return true;
}

void ClearWindow(Color color = { 0x00, 0x00, 0x00, 0xFF })
{
    SDL_SetRenderDrawColor(sdl_State.renderer, color.r, color.g, color.b, color.a);
    SDL_RenderClear(sdl_State.renderer);
}

char key_Message[256];
Image key_Image = { 0 };
char mouse_Message[256];
Image mouse_Image = { 0 };

void UpdateWindow()
{
    // RED RECT
    //SDL_Rect fillRect = { 640 / 4, 480 / 4, 640 / 2, 480 / 2 };
    //SDL_SetRenderDrawColor(sdl_State.renderer, 0xFF, 0x00, 0x00, 0xFF);
    //SDL_RenderFillRect(sdl_State.renderer, &fillRect);

    //ImageRender(&image, 0, 0);

    ImageRender(&key_Image, 0, 0);
    ImageRender(&mouse_Image, 0, 50);

    int width = 0, height = 0;
    SDL_QueryTexture(image.texture, 0, 0, &width, &height);
    SDL_SetRenderDrawColor(sdl_State.renderer, 255, 255, 255, 255);

    //char test[] = "a";
    //for (int i = 0; i < ('z'-'a'); ++i)
    //{
    //    TTF_SizeText(defaultFont.trueTypeFont, test, &width, &height);
    //    printf("%s = w-%d, h-%d\n", test, width, height);
    //    (*test)++;
    //}

    SDL_Rect newRect = { 0, 0, width, height };
    //TTF_SizeText(defaultFont.trueTypeFont, "The quick brown fox jumps over the lazy dog", &width, &height);
    //SDL_RenderDrawRect(sdl_State.renderer, &newRect);

    //newRect = { 0, 0, width, height };
    SDL_SetRenderDrawColor(sdl_State.renderer, 255, 255, 255, 255);
    //SDL_RenderDrawRect(sdl_State.renderer, &newRect);

    vec2 mousePos = GetMousePosition();
    newRect = { 0, 0, mousePos.x, mousePos.y };
    //SDL_RenderDrawRect(sdl_State.renderer, &newRect);
    ShapeDrawCircle(mousePos.x, mousePos.y, 10, { 255, 0, 255, 255 });

    SDL_RenderPresent(sdl_State.renderer);
}

void CloseWindow()
{
    ImageFree(&image);
    IMG_Quit();

    SDL_DestroyRenderer(sdl_State.renderer);
    SDL_DestroyWindow(sdl_State.window);
    SDL_Quit();
}

bool WindowShouldClose()
{
    return sdl_State.platform_State->quit;
}

//bool IsWindowReady(void);
//bool IsWindowFullscreen(void);
//bool IsWindowHidden(void);
//bool IsWindowMinimized(void);
//bool IsWindowMaximized(void);
//bool IsWindowFocused(void);
//bool IsWindowResized(void);
//bool IsWindowState(unsigned int flag);
//void SetWindowState(unsigned int flags);
//void ClearWindowState(unsigned int flags);
//void ToggleFullscreen(void);

void MaximizeWindow()
{
    SDL_MaximizeWindow(sdl_State.window);
}

void MinimizeWindow()
{
    SDL_MinimizeWindow(sdl_State.window);
}

void RestoreWindow()
{
    SDL_RestoreWindow(sdl_State.window);
}

//void SetWindowIcon(Image image);

void SetWindowTitle(const char* title)
{
    SDL_SetWindowTitle(sdl_State.window, title);
}

void SetWindowPosition(int x, int y)
{
    SDL_SetWindowPosition(sdl_State.window, x, y);
}

//void SetWindowMonitor(int monitor);

void SetWindowMinSize(int width, int height)
{
    sdl_State.platform_State->window_Min_Height = height;
    sdl_State.platform_State->window_Min_Width = width;
}

void SetWindowSize(int width, int height)
{
    sdl_State.platform_State->window_Height = height;
    sdl_State.platform_State->window_Width = width;
}

int GetScreenWidth()
{
    return sdl_State.platform_State->window_Width;
}

int GetScreenHeight()
{
    return sdl_State.platform_State->window_Height;
}

int GetMonitorCount()
{
    return SDL_GetNumVideoDisplays();
}

int GetCurrentMonitor()
{
    SDL_Window *current_Window = SDL_GetGrabbedWindow();
    if (!current_Window)
        return -1;

    return SDL_GetWindowID(current_Window);
}

//vec2 GetMonitorPosition(int monitor);
//int GetMonitorWidth(int monitor);
//int GetMonitorHeight(int monitor);
//int GetMonitorPhysicalWidth(int monitor);
//int GetMonitorPhysicalHeight(int monitor);
//int GetMonitorRefreshRate(int monitor);
//vec2 GetWindowPosition();
//vec2 GetWindowScaleDPI();

const char* GetMonitorName(int monitor)
{
    return SDL_GetWindowTitle(sdl_State.window);
}

//void SetClipboardText(const char* text);
//const char* GetClipboardText();
//void* GetWindowHandle();

bool PollEvent()
{
    return SDL_PollEvent(&sdl_State.event) != 0;
}

void HandleEvent()
{
    if(sdl_State.event.type == SDL_QUIT)
    {
        sdl_State.platform_State->quit = true;
    }
    else if(sdl_State.event.type == SDL_KEYDOWN)
    {
        int key_Sym = sdl_State.event.key.keysym.sym;
        sprintf(key_Message, "Key Sym (%d - \"%c\")\n", key_Sym, key_Sym);
        ImageLoadFromRenderTexture(&defaultFont, key_Message, SDL_Color{ 0, 255, 0, 255 }, 400, &key_Image);
        switch(key_Sym)
        {
            case SDLK_w:
            {
                sdl_State.platform_State->quit = true;
            }break;
            case SDLK_a:
            {
                sdl_State.platform_State->quit = true;
            }break;
            case SDLK_ESCAPE:
            {
                sdl_State.platform_State->quit = true;
            }break;
        }
    }
    else if(sdl_State.event.type == SDL_MOUSEBUTTONDOWN)
    {
        sprintf(mouse_Message, "Mouse Press %d\n", sdl_State.event.button.button);
        ImageLoadFromRenderTexture(&defaultFont, mouse_Message, SDL_Color{ 0, 255, 0, 255 }, 400, &mouse_Image);
        switch(sdl_State.event.button.button)
        {
            case SDL_BUTTON_LEFT:
            {
                sdl_State.platform_State->mouse_Pressed[0] = (sdl_State.event.button.state == SDL_PRESSED);
                sdl_State.platform_State->mouse_Released[0] = (sdl_State.event.button.state == SDL_RELEASED);
            }break;
            case SDL_BUTTON_MIDDLE:
            {
                sdl_State.platform_State->mouse_Pressed[1] = ( sdl_State.event.button.state == SDL_PRESSED );
                sdl_State.platform_State->mouse_Released[1] = ( sdl_State.event.button.state == SDL_RELEASED );
            }break;
            case SDL_BUTTON_RIGHT:
            {
                sdl_State.platform_State->mouse_Pressed[2] = ( sdl_State.event.button.state == SDL_PRESSED );
                sdl_State.platform_State->mouse_Released[2] = ( sdl_State.event.button.state == SDL_RELEASED );
            }break;
        }
    }
    vec2 mouse_Pos = GetMousePosition();
    //PRINT_CONSOLE(" Mouse Pos (%d - %d)\n", mouse_Pos.x, mouse_Pos.y);
}

bool IsKeyPressed(int key)
{
    return false;
}

bool IsMouseButtonPressed(int button)
{
    return 0;
}

bool IsMouseButtonDown(int button)
{
    return 0;
}

bool IsMouseButtonReleased(int button)
{
    return 0;
}

bool IsMouseButtonUp(int button)
{
    return 0;
}

int GetMouseX(void)
{
    int x = 0;
    int buttonMask = SDL_GetMouseState(&x, 0);
    return x;
}

int GetMouseY(void)
{
    int y=0;
    int buttonMask = SDL_GetMouseState(0, &y);
    return y;
}

vec2 GetMousePosition(void)
{
    int buttonMask = SDL_GetMouseState(&sdl_State.platform_State->mouse_X, &sdl_State.platform_State->mouse_Y);
    vec2 position = { sdl_State.platform_State->mouse_X, sdl_State.platform_State->mouse_Y };
    return position;
}
