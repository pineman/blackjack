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
    SDL_Surface *cards[MAX_DECK_SIZE+1], *imgs[2];
    SDL_Event event;
    int delay = 300;
    int quit = 0;
    int cards_left = 0;

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

	// Declarar o megabaralho (give_card enche-o automaticate)
	List *megadeck = (List *) calloc((size_t) 1, sizeof(List));

	// TODO: há mal a casa ser do tipo 'Player'?
	Player *house = (Player *) calloc((size_t) 1, sizeof(Player));

	new_game(players, house, megadeck, &cards_left, num_decks);

    //printf("%d", ((Player *) aux->prev->payload)->cards->card->id);

	// initialize graphics
	InitEverything(WIDTH_WINDOW, HEIGHT_WINDOW, imgs, &window, &renderer);
    // loads the cards images
    LoadCards(cards);

	// um bool diz se o jogador está currentemente a jogar!
	// cuidado para desativar um quando se ativa o outro!
	// portanto para passar a vez a outro jogador temos de verificar:
	/*
	// isto é basicamente o stand()
	while (players)
	{
		if (players->payload->playing) {
			aux = players;
			while (aux) {
				if (aux->payload->ingame)
					aux->payload->playing = true;
					players->payload->playing = false;
				aux = aux->next;
			{
		{
		players = players->next;
	}
	*/

	int i = 0;
 	while (quit == 0)
    {
		if (i == 10)
			((Player *) players->next->next->payload)->ingame = false;
		if (i++ == 20)
			((Player *) players->next->payload)->ingame = false;

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
						break;
    				case SDLK_h:
						// hit(players, house, megadeck); --> in logic.c
						break;
					case SDLK_n:
						new_game(players, house, megadeck, &cards_left, num_decks);
						break;
					case SDLK_a:
						// this is tricky.
						// do we keep old players (i.e. in the list players,
						// and know if theyre playing and their position
						// by special vars?
						// or do we remove them from the list and
						// copy their information to a static list
						// in write_stats?
						// add_player(players); // ler info do jogador de stdin --> in logic.c
						break;
					default:
						break;
				}
			}
        }
        // render game table
        RenderTable(players, imgs, renderer);
        // render house cards
        RenderHouseCards(house, cards, renderer);
        // render player cards
        RenderPlayerCards(players, cards, renderer);
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
	destroy_list(megadeck);

    UnLoadCards(cards);
    SDL_FreeSurface(imgs[0]);
    SDL_FreeSurface(imgs[1]);
    SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return EXIT_SUCCESS;
}
