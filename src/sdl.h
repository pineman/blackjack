#ifndef SDL_H
#define SDL_H

#include "main.h"
#include "logic.h"
#include "list.h"

#define STRING_SIZE 100       // max size for some strings
#define CARD_WIDTH 67         // card width
#define CARD_HEIGHT 97        // card height
#define WINDOW_POSX 100       // initial position of the window: x
#define WINDOW_POSY 100       // initial position of the window: y
#define WIDTH_WINDOW 900      // window width
#define HEIGHT_WINDOW 525     // window height
#define EXTRASPACE 150
#define MARGIN 5

void InitEverything(int width, int height, TTF_Font **_font, SDL_Surface *_img[], SDL_Window** _window, SDL_Renderer** _renderer);
void InitSDL();
void InitFont();

SDL_Window* CreateWindow(int width, int height);
SDL_Renderer* CreateRenderer(int width, int height, SDL_Window *_window);

int RenderText(int x, int y, const char *text, TTF_Font *_font, SDL_Color *_color, SDL_Renderer* _renderer);
void RenderPlayerArea(List *players, SDL_Renderer* _renderer, TTF_Font *_font, int separatorPos);
int RenderLogo(int, int, SDL_Surface *, SDL_Renderer *);

void RenderTable(List *players, TTF_Font *_font, SDL_Surface *_img[], SDL_Renderer *_renderer);

void RenderHouseCards(Player *house, SDL_Surface **_cards, SDL_Renderer* _renderer);
void RenderPlayerCards(List *players, SDL_Surface **_cards, SDL_Renderer* _renderer);
void RenderCard(int _x, int _y, int _num_card, SDL_Surface **_cards, SDL_Renderer* _renderer);

void LoadCards(SDL_Surface **_cards);
void UnLoadCards(SDL_Surface **_array_of_cards);

void render_status(List *players, TTF_Font *_font, SDL_Renderer *renderer);

// definition of some strings: they cannot be changed when the program is executed !
extern const char myName[];
extern const char myNumber[];

#endif
