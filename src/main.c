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

	House *house = (House *) calloc((size_t) 1, sizeof(House));

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
        RenderTable(players, imgs, renderer);
        // render house cards
        //RenderHouseCards(house, cards, renderer);
        // render player cards
        //RenderPlayerCards(players, cards, renderer);
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
