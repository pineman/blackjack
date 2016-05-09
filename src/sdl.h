#ifndef SDL_H
#define SDL_H

#include "main.h"
#include "logic.h"
#include "list.h"

#define STRING_SIZE 100       // max size for some strings
#define CARD_WIDTH 67         // card width
#define CARD_HEIGHT 97        // card height
#define WINDOW_POSX 500       // initial position of the window: x
#define WINDOW_POSY 250       // initial position of the window: y
#define WIDTH_WINDOW 900      // window width
#define HEIGHT_WINDOW 525     // window height
#define EXTRASPACE 150
#define MARGIN 5

void InitEverything(int , int , SDL_Surface **, SDL_Window **, SDL_Renderer **);
void InitSDL();
void InitFont();

SDL_Window *CreateWindow(int , int);
SDL_Renderer *CreateRenderer(int, int, SDL_Window *);

int RenderText(int, int, const char *, TTF_Font *, SDL_Color *, SDL_Renderer *);
void RenderPlayerArea(List *players, SDL_Renderer* _renderer, TTF_Font *serif, int separatorPos);
int RenderLogo(int, int, SDL_Surface *, SDL_Renderer *);

void RenderTable(List *players, SDL_Surface **, SDL_Renderer *);

void RenderHouseCards(Player *house, SDL_Surface **_cards, SDL_Renderer* _renderer);
void RenderPlayerCards(List *players, SDL_Surface **_cards, SDL_Renderer* _renderer);
void RenderCard(int, int, int, SDL_Surface **, SDL_Renderer *);

void LoadCards(SDL_Surface **);
void UnLoadCards(SDL_Surface **);

void render_status(List *players, SDL_Renderer *renderer);
// definition of some strings: they cannot be changed when the program is executed !
extern const char myName[];
extern const char myNumber[];

#endif
