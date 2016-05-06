#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "main.h"
#include "logic.h"
#include "file.h"
#include "sdl.h"

/**
 * main function: entry point of the program
 * only to invoke other functions !
 */
int main(int argc, char *argv[])
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Surface *cards[MAX_DECK_SIZE+1], *imgs[2];
    SDL_Event event;
    int delay = 300;
    int quit = 0;
    int money[MAX_PLAYERS] = {110, 110, 110, 110};
	int player_cards[MAX_PLAYERS][MAX_CARD_HAND] = {{0}};
    int house_cards[MAX_CARD_HAND] = {0};
    int pos_house_hand = 0;
    int pos_player_hand[MAX_PLAYERS] = {0};

	if (argc != 2)
		// TODO: error msg
		exit(EXIT_FAILURE);

	char *filename = argv[1];

	// initialize graphics
	InitEverything(WIDTH_WINDOW, HEIGHT_WINDOW, imgs, &window, &renderer);
    // loads the cards images
    LoadCards(cards);

	Config *config = read_config(filename);

	List *players = (List *) calloc((size_t) 1, sizeof(List));
	// this shall update players list and return num_decks
	const int num_decks = init_game(config, players, filename);

	// o give_card faz init do megadeck automagicamente
	// List *megadeck = (List *) calloc((size_t) 1, sizeof(List));

	// House *house = (House *) calloc((size_t) 1, sizeof(House));

    // put down some cards just for testing purposes: for you to remove !
    player_cards[0][0] = 0;
    player_cards[0][1] = 15;
    player_cards[1][0] = 10;
    player_cards[1][1] = 34;
    player_cards[1][2] = 0;
    player_cards[1][3] = 15;
    player_cards[1][4] = 10;
    player_cards[2][0] = 34;
    player_cards[2][1] = 0;
    player_cards[3][0] = 15;
    player_cards[3][1] = 10;
    pos_player_hand[0] = 2;
    pos_player_hand[1] = 5;
    pos_player_hand[2] = 2;
    pos_player_hand[3] = 2;

    house_cards[0] = 0;
    house_cards[1] = 12;
    pos_house_hand = 1;

 	while (quit == 0)
    {
        // while there's events to handle
        while (SDL_PollEvent(&event))
        {
			if (event.type == SDL_QUIT) {
                // quit the program
				quit = 1;
			}
			else if (event.type == SDL_KEYDOWN) {
				switch (event.key.keysym.sym)
				{
					case SDLK_q:
						// write_stats(players, house); --> in file.c
						quit = 1;
						break;
					case SDLK_s:
                        // stand(players, house, megadeck); --> in logic.c
					case SDLK_h:
						// hit(players, house, megadeck); --> in logic.c
					case SDLK_n:
						// new(players, house, megadeck); --> in logic.c
					case SDLK_a:
						// this is tricky.
						// do we keep old players (i.e. in the list players,
						// and know if theyre playing and their position
						// by special vars?
						// or do we remove them from the list and
						// copy their information to a static list
						// in write_stats?
						// add_player(players); // ler info do jogador de stdin --> in logic.c
					default:
						break;
				}
			}
        }
        // render game table
        RenderTable(money, imgs, renderer);
        // render house cards
        RenderHouseCards(house_cards, pos_house_hand, cards, renderer);
        // render player cards
        RenderPlayerCards(player_cards, pos_player_hand, cards, renderer);
        // render in the screen all changes above
        SDL_RenderPresent(renderer);
    	// add a delay
		SDL_Delay(delay);
    }

    // free memory allocated for images and textures and close everything
    UnLoadCards(cards);
    SDL_FreeSurface(imgs[0]);
    SDL_FreeSurface(imgs[1]);
    SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return EXIT_SUCCESS;
}
