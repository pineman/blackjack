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

// TODO: ERROR CHECKING CALLOC()

int main(int argc, char *argv[])
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    TTF_Font *serif = NULL;
    SDL_Surface *cards[MAX_DECK_SIZE+1] = {NULL};
    SDL_Surface *imgs[2] = {NULL};
    SDL_Event event;
    int delay = 300;
    bool quit = false;
    bool add_player_key = false;
    char error_msg[MAX_STR_SIZE] = {0};

	if (argc != 2) {
		puts("Erro: número inválido de argumentos.");
		puts("É esperado um argumento: ficheiro de configuração.");
		exit(EXIT_FAILURE);
	}

    srand(time(NULL));

	char *filename = argv[1];

	// Ler configuração
	Config *config = read_config(filename);

	// Declarar a lista de jogadores
	List *players = (List *) calloc((size_t) 1, sizeof(List));
	// enchê-la com dados do ficheiro de configuração
	const int num_decks = init_game(config, players);

	// Declarar a lista de jogadores velhos
	List *old_players = (List *) calloc((size_t) 1, sizeof(List));

	// Declarar o megabaralho (give_card() enche-o quando precisar)
    int cards_left = 0;
	List *deck = (List *) calloc((size_t) 1, sizeof(List));
    Megadeck megadeck_real = {cards_left, num_decks, deck};
    Megadeck *megadeck = &megadeck_real;

	// Inicializar a casa
	Player *house = (Player *) calloc((size_t) 1, sizeof(Player));

	// Inicializar um novo jogo
	new_game(players, house, megadeck);

    // loads the cards images
    LoadCards(cards);

	// initialize graphics
	InitEverything(WIDTH_WINDOW, HEIGHT_WINDOW, &serif, imgs, &window, &renderer);

 	while (!quit) {
        // while there's events to handle
        while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
                // user killed the window
                // don't write stats, quit regardless of context
				quit = true;
			}
			else if (event.type == SDL_KEYDOWN) {
				switch (event.key.keysym.sym) {
					case SDLK_q:
						quit_game(players, &quit);
						break;

					case SDLK_n:
						new_game(players, house, megadeck);
						break;

					case SDLK_r:
						surrender(players, house, megadeck);
						break;

					case SDLK_d:
                        double_bet(players, house, megadeck);
						break;

					case SDLK_b:
						bet(players);
						break;

					case SDLK_a:
						if (!find_active_player(players))
							add_player_key = true;
						break;

					case SDLK_s:
                        stand(players, house, megadeck);
						break;

    				case SDLK_h:
						player_hit(players, house, megadeck);
						break;

					default:
						break;
				}
			}
		}
		if (add_player_key)
			show_add_player_message(window);
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

		if (add_player_key) {
			strcpy(error_msg, add_player(players, old_players, window));
			printf("%s\n", error_msg);
			if (error_msg[0] != '\0')
				show_add_player_error_message(window, error_msg);
			add_player_key = false;
		}
    }

	write_stats(players, house, old_players);

	destroy_players_list(players);
	destroy_players_list(old_players);
    destroy_stack(&house->cards);
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
