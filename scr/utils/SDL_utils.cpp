#ifndef SDL_UTILS_CPP
#define SDL_UTILS_CPP

#include <SDL2/SDL.h>
#include "SDL_utils.h"

void initializeSDL()
{
    // Initialize video
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Failed to initialize SDL");
        return;
    }
}

SDL_Window *createWindow(char windowName[], int screenHeight, int screenWidth)
{
    SDL_Window *window = SDL_CreateWindow(
        windowName, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenHeight, screenWidth, SDL_WINDOW_OPENGL);

    if (!window)
    {
        printf("Failed to create window");
        return nullptr;
    }

    printf("Windows created successfully\n");
    return window;
}

SDL_Renderer *createRenderer(SDL_Window *window)
{
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer)
    {
        printf("Failed to create renderer");
        return nullptr;
    }
    printf("Renderer created successfully\n");

    return renderer;
}
// void drawFilledCisrcle(SDL_Renderer *renderer, int x, int y, int radius, SDL_Color color)
// {
//     SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
//     for (int w = 0; w < radius * 2; w++)
//     {
//         for (int h = 0; h < radius * 2; h++)
//         {
//             int dx = radius - w; // horizontal offset
//             int dy = radius - h; // vertical offset
//             if ((dx * dx + dy * dy) <= (radius * radius))
//             {
//                 SDL_RenderDrawPoint(renderer, x + dx, y + dy);
//             }
//         }
//     }
// }

#endif