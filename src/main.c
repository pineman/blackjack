#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <time.h>
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
    TTF_Font *serif = NULL;
    SDL_Surface *cards[MAX_DECK_SIZE+1], *imgs[2];
    SDL_Event event;
    int delay = 300;
    bool quit = false;

	if (argc != 2)
		// TODO: error msg
		exit(EXIT_FAILURE);

    srand(time(NULL));

	char *filename = argv[1];

	// Ler configuração (TODO: PODE FALHAR EARLY)
	Config *config = read_config(filename);

	// Declarar a lista de jogadores e enchê-la
	List *players = (List *) calloc((size_t) 1, sizeof(List));
	const int num_decks = init_game(config, players);
	free(config);

	// Declarar o megabaralho (give_card enche-o automaticate)
    int cards_left = 0;
	List *deck = (List *) calloc((size_t) 1, sizeof(List));
    Megadeck megadeck_real = {cards_left, num_decks, deck};
    Megadeck *megadeck = &megadeck_real;

	// TODO: há mal a casa ser do tipo 'Player'?
	Player *house = (Player *) calloc((size_t) 1, sizeof(Player));

	// Inicializar um novo jogo
	new_game(players, house, megadeck);

    // loads the cards images
    LoadCards(cards);

	// initialize graphics
	InitEverything(WIDTH_WINDOW, HEIGHT_WINDOW, &serif, imgs, &window, &renderer);

	// temos de ver se o rato é clicado dentro destas áreas
	// (em que num_player varia de 0 a 3):
    //int separatorPos = (int)(0.95f*WIDTH_WINDOW); // seperates the left from the right part of the window
	//playerRect.x = num_player * (separatorPos/4-5)+10;
	//playerRect.y = (int) (0.55f*HEIGHT_WINDOW);
	//playerRect.w = separatorPos/4-5;
	//playerRect.h = (int) (0.42f*HEIGHT_WINDOW);

 	while (!quit)
    {
        // while there's events to handle
        while (SDL_PollEvent(&event))
        {
			if (event.type == SDL_QUIT) {
                // quit the program
                // don't write stats?
                // quit regardless of context
				quit = true;
			}
			else if (event.type == SDL_KEYDOWN) {
				switch (event.key.keysym.sym)
				{
					case SDLK_q:
                        // only quit when all plays have been made
						test_quit(players, &quit);
                        if (quit)
                            write_stats(players, house);
						break;

					case SDLK_s:
						// stand
                        stand(players, house, megadeck);
						break;

    				case SDLK_h:
						// hit
						player_hit(players, house, megadeck);
						break;

					case SDLK_n:
						// new_game
						new_game(players, house, megadeck);
						break;

					case SDLK_a:
						// add_player
						// this is tricky.
						// do we keep old players (i.e. in the list players,
						// and know if theyre playing and their position
						// by special vars?
						// or do we remove them from the list and
						// copy their information to a static list
						// in write_stats?
						// add_player(players); // ler info do jogador de stdin --> in logic.c
						break;

					case SDLK_r:
						// surrender
						surrender(players, house, megadeck);
						break;

					case SDLK_d:
						// double
                        double_bet(players, house, megadeck);
						break;

					case SDLK_b:
						// bet
						bet(players);
						break;

					default:
						break;
				}
			}
        }
        // render game table
        RenderTable(players, serif, imgs, renderer);
        // render house cards
        RenderHouseCards(house, cards, renderer);
        // render player cards
        RenderPlayerCards(players, cards, renderer);
        // render colorful status rects above player
    	render_status(players, serif, renderer);
        // render in the screen all changes above
        SDL_RenderPresent(renderer);
        // add a delay
		SDL_Delay(delay);
    }

	// TODO: remember to free the list of old players too!
    List *aux = players->next;
    Player *cur_player = NULL;
    while (aux) {
		cur_player = (Player *) aux->payload;
		destroy_stack(&cur_player->cards);
		aux = aux->next;
    }
    destroy_stack(&house->cards);
	destroy_list(players);
	free(house);
	destroy_list(megadeck->deck);

    UnLoadCards(cards);
    TTF_CloseFont(serif);
    SDL_FreeSurface(imgs[0]);
    SDL_FreeSurface(imgs[1]);
    SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return EXIT_SUCCESS;
}
