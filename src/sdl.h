#ifndef SDL_H
#define SDL_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
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
#define SEP ((int) (0.95f*WIDTH_WINDOW))
#define PLAYER_RECT_X (SEP/4 - 5) + 10
#define PLAYER_RECT_Y ((int) (0.55f*HEIGHT_WINDOW))
#define PLAYER_RECT_W SEP/4 - 5
#define PLAYER_RECT_H ((int) (0.42f*HEIGHT_WINDOW))

void InitEverything(int width, int height, TTF_Font **_font, SDL_Surface *_img[], SDL_Window** _window, SDL_Renderer** _renderer);
void InitSDL();
void InitFont();

SDL_Window* CreateWindow(int width, int height);
SDL_Renderer* CreateRenderer(int width, int height, SDL_Window *_window);

int RenderText(int x, int y, const char *text, TTF_Font *_font, SDL_Color *_color, SDL_Renderer* _renderer);
void RenderPlayerArea(List *players, SDL_Renderer* _renderer, TTF_Font *_font);

void show_add_player_message(SDL_Window *window);
void show_add_player_error_message(SDL_Window *window, AddPlayerError error);
void show_add_player_input_message(SDL_Window *window);

int get_clicked_player();

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
