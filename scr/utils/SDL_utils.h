#ifndef SDL_UTILS_H
#define SDL_UTILS_H

#include <SDL2/SDL.h>

void initializeSDL();
SDL_Window *createWindow(char *windowName, int screenHeight, int screenWidth);
SDL_Renderer *createRenderer(SDL_Window *window);
void closeSDL();
void drawFilledCircle(SDL_Renderer *renderer, int x, int y, int radius, SDL_Color color);

#endif