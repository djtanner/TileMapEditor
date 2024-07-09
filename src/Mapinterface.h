#pragma once

#include <SDL2/SDL.h>
#include <iostream>
#include <unordered_map>
#include <fstream>

const int MAP_WIDTH = 1280;
const int MAP_HEIGHT = 512;
const int TILE_SIZE = 64;
const int rows = MAP_WIDTH / TILE_SIZE;
const int cols = MAP_HEIGHT / TILE_SIZE;
const int DEST_MAP_WIDTH = 64 + 640;
const int DEST_MAP_HEIGHT = 800 - 64;
const int SCALE = 2;

// this class will be used to display the map in the top half of the window

class Mapinterface
{
private:
    int **map;
    std::unordered_map<int, SDL_Rect> mapRects;
    int selectedTile = 1;

public:
    Mapinterface()
    {

        map = new int *[rows];
        for (int i = 0; i < rows; ++i)
        {
            for (int j = 0; j < cols; ++j)
            {
                map[i] = new int[cols];
                map[i][j] = 0;
            }
        }
    };
    ~Mapinterface()
    {

        for (int i = 0; i < rows; ++i)
        {
            delete[] map[i]; // Delete each sub-array
        }
        delete[] map;
    };

    void clearView(SDL_Renderer *renderer)
    {
        SDL_RenderClear(renderer);
    };

    void printMap()
    {

        for (int x = 0; x < cols; x++)
        {
            for (int y = 0; y < rows; y++)
            {
                std::cout << map[x][y] << " ";
            }
            std::cout << std::endl;
        }
    };

    void initalizeMapRects(SDL_Texture *spritesheet, int width, int height)
    {

        int tileWidth = width * SCALE / TILE_SIZE;
        int tileHeight = height * SCALE / TILE_SIZE;

        for (int i = 0; i < tileHeight; i++)
        {
            for (int j = 0; j < tileWidth; j++)
            {
                SDL_Rect rect = {j * TILE_SIZE / 2, i * TILE_SIZE / 2, TILE_SIZE / 2, TILE_SIZE / 2};
                mapRects[i * tileWidth + j + 1] = rect;
            }
        }
    };

    void updateMap(int x, int y)
    {
        int tileX = x / TILE_SIZE;
        int tileY = y / TILE_SIZE;

        if (tileX < rows && tileY < cols)
        {
            map[tileY][tileX] = selectedTile;
        }

        else if (tileY == cols + 1 && tileX >= 0 && tileX < rows)
        {
            selectedTile = tileX;
        }

        else if (tileY == cols + 2 && tileX >= 0 && tileX < rows)
        {
            selectedTile = tileX + tileY;
        }

        else if (tileY == cols + 3 && tileX >= 0 && tileX < rows)
        {

            selectedTile = tileX + 20;
        }
    };

    void deleteTile(int x, int y)
    {
        int tileX = x / TILE_SIZE;
        int tileY = y / TILE_SIZE;

        if (tileX < rows && tileY < cols)
        {
            map[tileY][tileX] = 0;
        }
    };

    void Update(SDL_Renderer *renderer, SDL_Texture *spritesheet)
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

        // draw outlines of rectangles in the destination map
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        for (int x = 0; x < MAP_WIDTH; x += TILE_SIZE)
        {
            for (int y = 0; y < MAP_HEIGHT; y += TILE_SIZE)
            {
                SDL_Rect rect = {x, y, TILE_SIZE, TILE_SIZE};
                SDL_RenderDrawRect(renderer, &rect);
            }
        }

        // draw outlines of rectangles in the source map and fill in the hashmap

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

        for (int x = TILE_SIZE; x < DEST_MAP_WIDTH; x += TILE_SIZE)
        {
            for (int y = 576; y < DEST_MAP_HEIGHT; y += TILE_SIZE)
            {
                SDL_Rect rect = {x, y, TILE_SIZE, TILE_SIZE};
                SDL_RenderDrawRect(renderer, &rect);
            }
        }

        // fill in rectangles from map
        for (int i = 0; i < cols; i++)
        {
            for (int j = 0; j < rows; j++)
            {
                if (map[i][j] != 0)
                {

                    SDL_Rect destRect = {j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE};
                    SDL_RenderCopy(renderer, spritesheet, &mapRects[map[i][j]], &destRect);
                }
            }
        }

        // add an overlay to the currently selected tile
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 75);
        SDL_Rect rect = mapRects[selectedTile];
        rect.x = rect.x * SCALE + TILE_SIZE;
        rect.y = rect.y * SCALE + 576; // horrible magic number
        rect.w = TILE_SIZE;
        rect.h = TILE_SIZE;
        SDL_RenderFillRect(renderer, &rect);
    }

    void saveMap()
    {
        std::ofstream file;
        file.open("map.txt");
        for (int x = 0; x < cols; x++)
        {
            for (int y = 0; y < rows; y++)
            {
                file << map[x][y] << " ";
            }
            file << std::endl;
        }
        file.close();
    }
};