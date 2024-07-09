#include "Maptool.h"
#include <SDL2/SDL.h>
#include <iostream>
#include "Mapinterface.h"
#include <SDL2/SDL_image.h>
#include <string>
#include <SDL2/SDL_ttf.h>
#include <vector>

Maptool::Maptool()
{
    isRunning = false;
}

Maptool::~Maptool()
{
}

void Maptool::Initialize(Mapinterface &mapinterface)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        std::cerr << "Error initializing SDL." << std::endl;
        return;
    }
    TTF_Init();
    window = SDL_CreateWindow(
        NULL,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_RESIZABLE);

    if (!window)
    {
        std::cerr << "Error creating SDL window." << std::endl;
        return;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer)
    {
        std::cerr << "Error creating SDL renderer." << std::endl;
        return;
    }

    // add the texture of the source image to the bottom of the window
    std::string filePath = "./assets/desert.png";
    surface = IMG_Load(filePath.c_str());
    if (!surface)
    {
        std::cout << "Error loading surface: " << filePath << std::endl;
        return;
    }

    spritesheet = SDL_CreateTextureFromSurface(renderer, surface);
    if (!spritesheet)
    {
        std::cout << "Error creating texture: " << filePath << std::endl;
        return;
    }
    mapinterface.initalizeMapRects(spritesheet, surface->w, surface->h);

    isRunning = true;
}

void Maptool::ProcessInput(Mapinterface &mapinterface)
{
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type)
    {
    case SDL_QUIT:
        isRunning = false;
        break;
    case SDL_KEYDOWN:
        if (event.key.keysym.sym == SDLK_ESCAPE)
            isRunning = false;

        // save the file if ctrl + s is pressed

        else if (event.key.keysym.sym == SDLK_s && (SDL_GetModState() & KMOD_CTRL))
        {
            mapinterface.saveMap();
        }
        break;

    // handle mouse events
    case SDL_MOUSEBUTTONDOWN:
        if (event.button.button == SDL_BUTTON_LEFT)
        {
            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);
            clickOccurred = true;
            clickX = mouseX;
            clickY = mouseY;
            std::cout << "MouseX: " << mouseX << " MouseY: " << mouseY << std::endl;
        }

        if (event.button.button == SDL_BUTTON_RIGHT)
        {
            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);
            int x = mouseX % TILE_SIZE == 0 ? mouseX : mouseX - mouseX % TILE_SIZE;
            int y = mouseY % TILE_SIZE == 0 ? mouseY : mouseY - mouseY % TILE_SIZE;
            mapinterface.deleteTile(x, y);
        }
        break;
    }
}

void Maptool::Update(Mapinterface &mapinterface)
{
    /* int timeToWait = MILLISECS_PER_FRAME - (SDL_GetTicks() - millisecsPreviousFrame);
      if (timeToWait > 0 && timeToWait <= MILLISECS_PER_FRAME)
      {
          SDL_Delay(timeToWait);
      }

      //double deltaTime = (SDL_GetTicks() - millisecsPreviousFrame) / 1000.0;

      millisecsPreviousFrame = SDL_GetTicks();*/
}

void Maptool::Render(Mapinterface &mapinterface)
{
    SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
    SDL_RenderClear(renderer);

    if (clickOccurred)
    {
        // check if click is in the destination window
        int x = clickX % TILE_SIZE == 0 ? clickX : clickX - clickX % TILE_SIZE;
        int y = clickY % TILE_SIZE == 0 ? clickY : clickY - clickY % TILE_SIZE;

        mapinterface.updateMap(x, y);

        // mapinterface.printMap();

        clickOccurred = false;
    }

    // add the texture of the source image to the bottom of the window
    SDL_Rect destRect;
    destRect.x = 64;
    destRect.y = WINDOW_HEIGHT - surface->h - 128;
    destRect.w = (surface->w) * 2;
    destRect.h = (surface->h) * 2;

    SDL_RenderCopy(renderer, spritesheet, NULL, &destRect);

    mapinterface.Update(renderer, spritesheet);

    // add text to the bottom of the window
    const std::string filePath = "./assets/fonts/arial.ttf";
    TTF_Font *Sans = TTF_OpenFont(filePath.c_str(), 16);

    if (!Sans)
    {
        std::cerr << "Error loading font." << std::endl;
        return;
    }

    SDL_Color color = {255, 255, 255, 255};

       std::vector<std::string> lines = {
        "Select a tile from the bottom window to place it on the map.",
        "Right click on the map to erase.",
        "Ctrl + S to save"};

    int y_offset = 580; // Starting y coordinate
    for (const auto &line : lines)
    {
        SDL_Surface *surfaceMessage = TTF_RenderText_Solid(Sans, line.c_str(), color);
        SDL_Texture *Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);

        SDL_Rect Message_rect;
        Message_rect.x = 860;
        Message_rect.y = y_offset;
        // Adjust width and height based on the text size
        // For simplicity, we're using fixed values, but you should ideally use TTF_SizeText() to get the exact text size
        Message_rect.w = surfaceMessage->w; // Use the surface's width
        Message_rect.h = surfaceMessage->h; // Use the surface's height

        SDL_RenderCopy(renderer, Message, NULL, &Message_rect);
        SDL_FreeSurface(surfaceMessage);
        SDL_DestroyTexture(Message);

        y_offset += Message_rect.h + 5; // Move down for the next line, adjust spacing as needed
    }

    SDL_RenderPresent(renderer);
}

void Maptool::Run(Mapinterface &mapinterface)
{
    while (isRunning)
    {
        ProcessInput(mapinterface);
        Update(mapinterface);
        Render(mapinterface);
    }
}

void Maptool::Destroy()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    isRunning = false;
}