#include <iostream>
#include <algorithm>
#include <SDL2/SDL.h>
#include <vector>
#include <math.h>
#include "utils/SDL_utils.h"

SDL_Color backgroundColor{255, 255, 255, 255};
SDL_Color liveCellColor{0, 0, 0, 0};
SDL_Color gridlineColor{0, 0, 0, 0};
SDL_Color hoverColor{191, 191, 191, 1};

unsigned SCREEN_HEIGHT = 1000;
unsigned SCREEN_WIDTH = 1000;
const int GRID_SIZE_X = 100;
const int GRID_SIZE_Y = 100;
int UNIT_SIZE_X = SCREEN_WIDTH / GRID_SIZE_X;
int UNIT_SIZE_Y = SCREEN_HEIGHT / GRID_SIZE_Y;

struct Pair
{
    int x, y;
};

bool operator==(const Pair &lhs, const Pair &rhs)
{
    return (lhs.x == rhs.x) && (lhs.y == rhs.y);
}

int getNeighbours(Pair element, std::vector<Pair> *vec)
{
    int nNeighbours = 0;
    std::vector<Pair>::iterator it;
    for (it = vec->begin(); it != vec->end(); it++)
    {
        // Logic for finding neighbours
        for (int i = -1; i < 2; i++)
            if (it->x + i == element.x)
            {
                if ((it->y - 1 == element.y) || it->y == element.y || it->y + 1 == element.y)
                {
                    if (!(i == 0 && it->y == element.y))
                    {
                        nNeighbours++;
                    }
                }
            }
    }
    return nNeighbours;
}

void drawGridLines(SDL_Renderer *renderer, SDL_Color gridlineColor)
{

    SDL_SetRenderDrawColor(renderer, gridlineColor.r,
                           gridlineColor.g, gridlineColor.b,
                           gridlineColor.a);

    for (int i = UNIT_SIZE_X; i < SCREEN_WIDTH; i += UNIT_SIZE_X)
    {
        SDL_RenderDrawLine(renderer, i, 0, i, SCREEN_HEIGHT);
    }
    for (int i = UNIT_SIZE_Y; i < SCREEN_HEIGHT; i += UNIT_SIZE_Y)
    {
        SDL_RenderDrawLine(renderer, 0, i, SCREEN_WIDTH, i);
    }
}

void drawHoverRect(SDL_Renderer *renderer, int x, int y, SDL_Color hoverColor)
{
    SDL_SetRenderDrawColor(renderer, hoverColor.r,
                           hoverColor.b, hoverColor.g,
                           hoverColor.a);
    SDL_Rect rect;
    rect.x = x * UNIT_SIZE_X + 1;
    rect.y = y * UNIT_SIZE_Y + 1;
    rect.w = UNIT_SIZE_X - 1;
    rect.h = UNIT_SIZE_Y - 1;

    SDL_RenderFillRect(renderer, &rect);
}

void drawBackground(SDL_Renderer *renderer, SDL_Color backgroundColor)
{
    SDL_SetRenderDrawColor(renderer, backgroundColor.r,
                           backgroundColor.g, backgroundColor.b,
                           backgroundColor.a);

    SDL_RenderClear(renderer);
}

void updateGrid(std::vector<Pair> *vec)
{
    std::vector<Pair> new_state;
    for (int i = 0; i < GRID_SIZE_X; i++)
    {
        for (int j = 0; j < GRID_SIZE_X; j++)
        {
            Pair p{.x = i, .y = j};
            int nNeighbours = getNeighbours(p, vec);
            if (std::find(vec->begin(), vec->end(), p) != vec->end())
            {
                if (nNeighbours == 2 || nNeighbours == 3)
                {
                    new_state.push_back(p);
                }
            }
            else
            {
                if (nNeighbours == 3)
                {
                    new_state.push_back(p);
                }
            }
        }
    }

    *vec = new_state;
}

void drawLiveCells(SDL_Renderer *renderer, std::vector<Pair> vec, SDL_Color fillColor)
{
    SDL_SetRenderDrawColor(renderer, fillColor.r,
                           fillColor.b, fillColor.g,
                           fillColor.a);

    //  Update the world and fill all the living cells
    for (auto i = vec.begin(); i != vec.end(); i++)
    {
        SDL_Rect rect;
        rect.x = i->x * UNIT_SIZE_X;
        rect.y = i->y * UNIT_SIZE_Y;
        rect.w = UNIT_SIZE_X;
        rect.h = UNIT_SIZE_Y;
        SDL_RenderFillRect(renderer, &rect);
    };
}

void handleMouseButtonDownEvent(SDL_Event event, bool *draw)
{
    switch (event.button.button)
    {
    case SDL_BUTTON_LEFT:
        *draw = true;
        break;
    default:
        break;
    }
}
void handleMouseButtonUpEvent(SDL_Event event, bool *draw)
{
    switch (event.button.button)
    {
    case SDL_BUTTON_LEFT:
        *draw = false;
        break;
    default:
        break;
    }
}

void handleKeyDownEvent(SDL_Event event, bool *running_simulation, bool *quit, bool *clear)
{
    switch (event.key.keysym.sym)
    {
    case SDLK_r:
        printf("start automata! ");
        *running_simulation = true;
        break;

    case SDLK_s:
        printf("Pause automata! ");
        *running_simulation = false;
        break;

    case SDLK_c:
        printf("Clear world");
        *clear = true;
        break;

    case SDLK_q:
        *quit = true;
        break;
    default:
        break;
    }
}

int main()
{
    char windowName[] = "Game of life";

    initializeSDL();
    SDL_Window *window = createWindow(windowName, SCREEN_HEIGHT, SCREEN_WIDTH);
    SDL_Renderer *renderer = createRenderer(window);

    SDL_Event event;
    int mouse_x, mouse_y;
    bool quit = false;
    bool draw = false;
    bool simulation_active = false;
    bool clear = false;

    std::vector<Pair>
        vec;

    while (!quit)
    {
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_MOUSEMOTION:
                mouse_x = floor(event.motion.x / UNIT_SIZE_X);
                mouse_y = floor(event.motion.y / UNIT_SIZE_Y);
                break;

            case SDL_MOUSEBUTTONDOWN:
                handleMouseButtonDownEvent(event, &draw);
                break;

            case SDL_MOUSEBUTTONUP:
                handleMouseButtonUpEvent(event, &draw);
                break;

            case SDL_KEYDOWN:
                handleKeyDownEvent(event, &simulation_active, &quit, &clear);
                break;

            case SDL_QUIT:
                quit = true;
                break;
            }
        }

        // Draw background
        drawBackground(renderer, backgroundColor);
        drawGridLines(renderer, gridlineColor);

        if (draw)
        {
            Pair pair_to_add;
            pair_to_add.x = mouse_x;
            pair_to_add.y = mouse_y;

            if (std::find(vec.begin(), vec.end(), pair_to_add) == vec.end())
            {
                vec.push_back(pair_to_add);
            }
        }

        if (clear)
        {
            vec.clear();
            clear = false;
        }

        if (simulation_active)
        {
            updateGrid(&vec);
        }
        // Fill the cells alive
        drawLiveCells(renderer, vec, liveCellColor);

        // Show hover
        SDL_SetRenderDrawColor(renderer, 150, 53, 200, 13);
        drawHoverRect(renderer, mouse_x, mouse_y, hoverColor);

        SDL_RenderPresent(renderer);
    }

    SDL_Quit();
    return EXIT_SUCCESS;
}