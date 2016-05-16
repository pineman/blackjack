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
#include "ea.h"
#include "error.h"

// TODO: COMENTAR CÓDIGO COMO DEVE DE SER!
// TODO: Adicionar "autores" e introdução e etc.!
int main(int argc, char *argv[])
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    TTF_Font *serif = NULL;
    SDL_Surface *cards[MAX_DECK_SIZE+1] = {NULL};
    SDL_Surface *imgs[2] = {NULL};
    SDL_Event event;
    int delay = 300;
	int ea_delay = 1500;
    bool quit = false;
    bool add_player_key = false;
    AddPlayerError add_player_error = OK;

	if (argc != 3) {
		puts("Erro: número inválido de argumentos.");
		puts("Utilização:");
		printf("%s <ficheiro de config. do jogo> <ficheiro de config. das EAs>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	// Ler ficheiro de configuração dos jogadores
	Config *config = read_config(argv[1]);

	// Ler ficheiro de estrategia das EAs
	Strategy *strategy = read_strategy(argv[2]);

	// Declarar a lista de jogadores
	List *players = (List *) ecalloc((size_t) 1, sizeof(List));
	// enchê-la com dados do ficheiro de configuração
	const int num_decks = init_game(config, players);

	// Declarar a lista de jogadores velhos
	List *old_players = (List *) ecalloc((size_t) 1, sizeof(List));

	// Inicializar o megabaralho
	// é uma struct que contém a lista das cartas em si
    int cards_left = 0;
	List *deck = (List *) ecalloc((size_t) 1, sizeof(List));
    Megadeck megadeck_real = {cards_left, num_decks, deck, 0};
    Megadeck *megadeck = &megadeck_real;
    megadeck->cards_left = create_megadeck(megadeck);

	// Inicializar a casa
	Player *house = (Player *) ecalloc((size_t) 1, sizeof(Player));
	// Inicializar um novo jogo
    srand(time(NULL));
	new_game(players, house, megadeck);

    // loads the cards images
    LoadCards(cards);

	// initialize graphics
	InitEverything(WIDTH_WINDOW, HEIGHT_WINDOW, &serif, imgs, &window, &renderer);

	List *aux = find_active_player(players);
	bool ea = false;
 	while (!quit) {
        // while there's events to handle
        while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
                // user killed the window
				quit = true;
			}
			else if (event.type == SDL_KEYDOWN) {
				switch (event.key.keysym.sym) {
					case SDLK_q:
						if (find_active_player(players) == NULL ||
							find_ingame_player(players) == NULL)
							quit = true;
						break;

					case SDLK_n:
						new_game(players, house, megadeck);
						break;

					case SDLK_r:
						if (find_active_human_player(players) != NULL)
							surrender(players, house, megadeck);
						break;

					case SDLK_d:
						if (find_active_human_player(players) != NULL)
							double_bet(players, house, megadeck);
						break;

					case SDLK_b:
						bet(players);
						break;

					case SDLK_a:
						if (find_active_player(players) == NULL)
							add_player_key = true;
						break;

					case SDLK_s:
						if (find_active_human_player(players) != NULL)
							stand(players, house, megadeck);
						break;

    				case SDLK_h:
						if (find_active_human_player(players) != NULL)
							player_hit(players, house, megadeck);
						break;

					case SDLK_UP:
						ea_delay+=100;
						break;

					case SDLK_DOWN:
						if (ea_delay > 100)
							ea_delay-=100;
						break;

					default:
						break;
				}
			}
		}

		// TODO: fazer função com isto tudo?
		if (add_player_key)
			show_add_player_message(window);
        // render game table
        RenderTable(players, serif, imgs, renderer);
        // render house cards
        RenderHouseCards(house, cards, serif, renderer);
        // render player cards
        RenderPlayerCards(players, cards, renderer);
        // render colorful status rects above player
    	render_status(players, serif, renderer);
        // render in the screen all changes above
        SDL_RenderPresent(renderer);
        // add a delay
		SDL_Delay(delay);

		aux = find_active_player(players);
		ea = false;
		if (aux != NULL)
			if (((Player * ) aux->payload)->type == EA)
				ea = true;

		if (ea) {
			ea_make_decision(players, house, megadeck, strategy);
			SDL_Delay(ea_delay);
		}

		else if (add_player_key) {
			add_player_error = add_player(players, old_players, window);
			if (add_player_error != OK)
				show_add_player_error_message(window, add_player_error);
			add_player_key = false;
		}
    }

	write_stats(players, house, old_players);

	// TODO: fazer função com isto tudo?
	destroy_players_list(players);
	destroy_players_list(old_players);
    destroy_stack(&house->cards);
	free(house);
	destroy_list(megadeck->deck);
	destroy_matrix(strategy->hard, HARD_LINES);
	destroy_matrix(strategy->soft, SOFT_LINES);
	free(strategy);

    UnLoadCards(cards);
    TTF_CloseFont(serif);
    SDL_FreeSurface(imgs[0]);
    SDL_FreeSurface(imgs[1]);
    SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return EXIT_SUCCESS;
}
