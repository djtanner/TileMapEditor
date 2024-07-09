#pragma once

#include <SDL2/SDL.h>
#include "Mapinterface.h"

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 800;
// const int FPS = 500;
// const int MILLISECS_PER_FRAME = 1000 / FPS;

class Maptool
{
private:
    bool isRunning;
    int millisecsPreviousFrame = 0;
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *spritesheet;
    SDL_Surface *surface;
    bool clickOccurred = false;
    int clickX = 0;
    int clickY = 0;
    void fillRect(Mapinterface &mapinterface, int x, int y, int w, int h, SDL_Color color);

public:
    Maptool();
    ~Maptool();

    void Initialize(Mapinterface &mapinterface);
    void Run(Mapinterface &mapinterface);
    void ProcessInput(Mapinterface &mapinterface);
    void Update(Mapinterface &mapinterface);
    void Render(Mapinterface &mapinterface);
    void Destroy();
};
