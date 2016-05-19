#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "main.h"
#include "sdl.h"

const char myName1[] = "João Pinheiro 84086";
const char myName2[] = "João Freitas  84093";

/**
 * RenderTable: Draws the table where the game will be played, namely:
 * -  some texture for the background
 * -  the right part with the IST logo and the student name and number
 * -  squares to define the playing positions of each player
 * -  names and the available money for each player
 * \param _money amount of money of each player
 * \param _img surfaces where the table background and IST logo were loaded
 * \param _renderer renderer to handle all rendering in a window
 */
void RenderTable(List *players, TTF_Font *_font, SDL_Surface *_img[], SDL_Renderer *_renderer)
{
    SDL_Color black = {0, 0, 0, 255}; // black
    SDL_Texture *table_texture;
    SDL_Rect tableSrc, tableDest;
    int height;
    char money_str[STRING_SIZE];

    // set color of renderer to white
    SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);

    // clear the window
    SDL_RenderClear(_renderer);

    tableDest.x = tableSrc.x = 0;
    tableDest.y = tableSrc.y = 0;
    tableSrc.w = _img[0]->w;
    tableSrc.h = _img[0]->h;

    tableDest.w = SEP;
    tableDest.h = HEIGHT_WINDOW;

    table_texture = SDL_CreateTextureFromSurface(_renderer, _img[0]);
    SDL_RenderCopy(_renderer, table_texture, &tableSrc, &tableDest);

    // render the IST Logo
    height = RenderLogo(SEP, 0, _img[1], _renderer);

    // render the student name
    height += RenderText(SEP+3*MARGIN, height, myName1, _font, &black, _renderer);

    // this renders the student number
    height += RenderText(SEP+3*MARGIN, height, myName2, _font, &black, _renderer);

    // 2xnewline
    height += 2*RenderText(SEP+3*MARGIN, height, " ", _font, &black, _renderer);

	List *aux = players->next;
	Player *cur_player = NULL;
	while (aux) {
		cur_player = (Player *) aux->payload;
		if (cur_player->ingame) {
			sprintf(money_str, "%s (%s): %d euros",
					cur_player->name, cur_player->type == HU ? "HU" : "EA", cur_player->money);
			height += RenderText(SEP+3*MARGIN, height, money_str, _font, &black, _renderer);
		}
		aux = aux->next;
	}

	RenderPlayerArea(players, _renderer, _font);

    // destroy everything
    SDL_DestroyTexture(table_texture);
}


/* Desenhar a area do jogador
 * Nome, aposta, estado e pontos
 * Quadrado de cor diferente para o jogador que esta a jogar
 */
void RenderPlayerArea(List *players, SDL_Renderer* _renderer, TTF_Font *_font)
{
    SDL_Color white = {255, 255, 255, 255};
    SDL_Rect playerRect;
    char points_str[STRING_SIZE];
    char status_str[STRING_SIZE];
    List *aux = players->next;
    Player *cur_player = NULL;
    int num_player = 0;

	while (aux) {
		cur_player = (Player *) aux->payload;
		if (cur_player->ingame) {
			if (cur_player->playing)
				SDL_SetRenderDrawColor(_renderer, 255, 0, 0, 255);
			else
				SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);

			playerRect.x = num_player*PLAYER_RECT_X;
			playerRect.y = PLAYER_RECT_Y;
			playerRect.w = PLAYER_RECT_W;
			playerRect.h = PLAYER_RECT_H;

			if (cur_player->status == WW || cur_player->status == ST)
				sprintf(points_str, "%d", cur_player->points);
			else if (cur_player->status == BJ)
				sprintf(points_str, "BJ");
			else if (cur_player->status == BU)
				sprintf(points_str, "BU");
			else if (cur_player->status == SU)
				sprintf(points_str, "SU");

			sprintf(status_str, "%s -- bet: %d, points: %s",
					cur_player->name, cur_player->bet, points_str);
			RenderText(playerRect.x, playerRect.y-30, status_str, _font, &white, _renderer);
			SDL_RenderDrawRect(_renderer, &playerRect);
		}
		aux = aux->next;
		num_player++;
    }
}


void show_add_player_message(SDL_Window *window)
{
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,
							 "Adicionar Jogador",
							 "Clique num lugar vazio para inserir um novo jogador.",
							 window);
}

void show_add_player_error_message(SDL_Window *window, AddPlayerError error)
{
	char error_msg[MAX_STR_SIZE] = {0};

	switch(error) {
		case OUT:
			strcpy(error_msg, "Nao clicou dentro da area dos jogadores.\n"
				   "Tente novamente primindo a tecla <a>.");
			break;

		case NOTEMPTY:
			strcpy(error_msg, "Nao selecionou um lugar vazio.\n"
				   "Tente novamente primindo a tecla <a>.");
			break;

		default:
			break;
	}
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,
							 "Adicionar Jogador",
							 error_msg,
							 window);
}

void show_add_player_input_message(SDL_Window *window)
{
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,
							 "Adicionar Jogador",
							 "Insira os dados do jogador no terminal.",
							 window);
}


//Obter posição para inserir o novo jogador
int get_clicked_player()
{
	SDL_Event event;
	int i = 0;

	while (1) {
		SDL_PollEvent(&event);
		if (event.type == SDL_MOUSEBUTTONDOWN)
			break;
	}

	int mouse_x = event.button.x;
	int mouse_y = event.button.y;

	if (mouse_y >= PLAYER_RECT_Y && mouse_y <= PLAYER_RECT_Y + PLAYER_RECT_H)
		while (mouse_x >= 0*PLAYER_RECT_X) {
			mouse_x -= PLAYER_RECT_W;
			i++;
		}
	else
		i = 0;

	return i;
}

/**
 * RenderHouseCards: Renders cards of the house
 * \param _house vector with the house cards
 * \param _pos_house_hand position of the vector _house with valid card IDs
 * \param _cards vector with all loaded card images
 * \param _renderer renderer to handle all rendering in a window
 */
void RenderHouseCards(Player *house, SDL_Surface **_cards, TTF_Font *_font, SDL_Renderer* _renderer)
{
    int x = 0, y = 0;
    int div = WIDTH_WINDOW/CARD_WIDTH;
	Card *cur_card = NULL;
	int card_id = 0;
	int num_cards = 0;
    SDL_Color white = { 255, 255, 255, 255};
	char status_str[STRING_SIZE] = {0};
	char points_str[STRING_SIZE] = {0};

	if (house->status == WW || house->status == ST)
		sprintf(points_str, "%d", house->points);
	else if (house->status == BJ)
		sprintf(points_str, "BJ");
	else if (house->status == BU)
		sprintf(points_str, "BU");

	sprintf(status_str, "dealer: %s points", points_str);
	RenderText(20, 130, status_str, _font, &white, _renderer);

    Stack *aux = house->cards;
    Stack *tmp = NULL;
    // drawing all house cards
	while (tmp != house->cards) {
		aux = house->cards;
        while (aux->next != tmp)
            aux = aux->next;

		cur_card = aux->card;
		card_id = cur_card->id + cur_card->suit * SUIT_SIZE;

        // calculate its position
        x = (div/2 - house->num_cards/2 + num_cards)*CARD_WIDTH + 15;
        y = (int) (0.26f*HEIGHT_WINDOW);
        RenderCard(x, y, card_id, _cards, _renderer);

		num_cards++;
        tmp = aux;
    }

    // If the dealer has only 2 cards and no blackjack, draw the second card face down
    if (house->num_cards == 1 && house->status != BJ) {
        x = (div/2-house->num_cards/2+1)*CARD_WIDTH + 15;
        y = (int) (0.26f*HEIGHT_WINDOW);
        RenderCard(x, y, MAX_DECK_SIZE, _cards, _renderer);
    }
}

/**
 * RenderPlayerCards: Renders the hand, i.e. the cards, for each player
 * \param _player_cards 2D array with the player cards, 1st dimension is the player ID
 * \param _pos_player_hand array with the positions of the valid card IDs for each player
 * \param _cards vector with all loaded card images
 * \param _renderer renderer to handle all rendering in a window
 */
void RenderPlayerCards(List *players, SDL_Surface **_cards, SDL_Renderer* _renderer)
{
    int pos = 0, x = 0, y = 0;
    int num_player = 0;
    int num_cards = 0;
    int card_id = 0;

    List *aux = players->next; // dummy head
    Player *cur_player = NULL;
    Card *cur_card = 0;
	Stack *aux_cards = NULL;
    // Iterate over all players
    while (aux) {
        cur_player = (Player *) aux->payload;
		if (cur_player->ingame) {
			// Iterate over the stack backwards
			aux_cards = cur_player->cards;
			if (aux_cards)
				while (aux_cards->next)
					aux_cards = aux_cards->next;

			// agora aux_cards aponta para o último elemento da stack
			while (aux_cards) {
				// get the card
				cur_card = aux_cards->card;
				card_id = cur_card->id + cur_card->suit * SUIT_SIZE;

				// draw the card
				pos = num_cards % 4;
				x = (int) num_player * (SEP/4-5)+(num_cards/4)*12+15;
				y = (int) PLAYER_RECT_Y+10;
				if ( pos == 1 || pos == 3) x += CARD_WIDTH + 30;
				if ( pos == 2 || pos == 3) y += CARD_HEIGHT+ 10;
				RenderCard(x, y, card_id, _cards, _renderer);

				num_cards++;
				aux_cards = aux_cards->prev;
			}
			num_cards = 0;
		}
		aux = aux->next;
		num_player++;
    }
}

/**
 * RenderCard: Draws one card at a certain position of the window, based on the card code
 * \param _x X coordinate of the card position in the window
 * \param _y Y coordinate of the card position in the window
 * \param _num_card card code that identifies each card
 * \param _cards vector with all loaded card images
 * \param _renderer renderer to handle all rendering in a window
 */
void RenderCard(int _x, int _y, int _num_card, SDL_Surface **_cards, SDL_Renderer* _renderer)
{
    SDL_Texture *card_text;
    SDL_Rect boardPos;

    // area that will be occupied by each card
    boardPos.x = _x;
    boardPos.y = _y;
    boardPos.w = CARD_WIDTH;
    boardPos.h = CARD_HEIGHT;

    // render it !
    card_text = SDL_CreateTextureFromSurface(_renderer, _cards[_num_card]);
    SDL_RenderCopy(_renderer, card_text, NULL, &boardPos);

	// destroy everything
    SDL_DestroyTexture(card_text);
}

/**
 * LoadCards: Loads all images of the cards
 * \param _cards vector with all loaded card images
 */
void LoadCards(SDL_Surface **_cards)
{
	int i = 0;
    char filename[STRING_SIZE] = {0};

     // loads all cards to an array
    for (i = 0; i < MAX_DECK_SIZE; i++) {
        // create the filename !
        sprintf(filename, ".//assets//cartas//carta_%02d.png", i+1);
        // loads the image !
        _cards[i] = IMG_Load(filename);
        // check for errors: deleted files ?
        if (_cards[i] == NULL) {
            fprintf(stderr, "Unable to load image: %s\n", SDL_GetError());
            exit(EXIT_FAILURE);
        }
    }
    // loads the card back
    _cards[i] = IMG_Load(".//assets//cartas//carta_back.jpg");
    if (_cards[i] == NULL) {
        fprintf(stderr, "Unable to load image: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
}


/**
 * UnLoadCards: unloads all card images of the memory
 * \param _cards vector with all loaded card images
 */
void UnLoadCards(SDL_Surface **_array_of_cards)
{
    // unload all cards of the memory: +1 for the card back
    for (int i = 0 ; i < MAX_DECK_SIZE + 1; i++ )
    {
        SDL_FreeSurface(_array_of_cards[i]);
    }
}
// Desenhar o estado do jogador
// Blackjack, Bust e Surrender
void render_status(List *players, TTF_Font *_font, SDL_Renderer *renderer)
{
    SDL_Rect rect;

    char bust[] = "BUST";
    char blackjack[] = "BLACKJACK";
	char surrender[] = "SURRENDER";

    List *aux = players->next;
    Player *cur_player = NULL;
    SDL_Color white = { 255, 255, 255, 255};
    for (int i=0; aux; i++) {
        cur_player = (Player *) aux->payload;
        rect.y = 380;
        rect.h = 30;
        if (cur_player->ingame) {
			if (cur_player->status == BJ) {
				rect.x = 55 + 208*i;
				rect.w = 115;
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255 );
				SDL_RenderFillRect(renderer, &rect);
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255 );
				SDL_RenderDrawRect(renderer, &rect);
				RenderText(64+208*i, 382, blackjack, _font, &white, renderer);
			}
			else if (cur_player->status == BU) {
				rect.x = 80 + 208*i;
				rect.w = 70;
				SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255 );
				SDL_RenderFillRect(renderer, &rect);
				SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255 );
				SDL_RenderDrawRect(renderer, &rect);
				RenderText(94+(208*i), 382, bust, _font, &white, renderer);
			}
			else if (cur_player->status == SU) {
				rect.x = 55 + 208*i;
				rect.w = 115;
				SDL_SetRenderDrawColor(renderer, 255, 200, 0, 255 );
				SDL_RenderFillRect(renderer, &rect);
				SDL_SetRenderDrawColor(renderer, 255, 200, 0, 255 );
				SDL_RenderDrawRect(renderer, &rect);
				RenderText(64+208*i, 382, surrender, _font, &white, renderer);
			}

		}
        aux = aux->next;
    }
}

/**
 * RenderLogo function: Renders the IST Logo on the window screen
 * \param x X coordinate of the Logo
 * \param y Y coordinate of the Logo
 * \param _logoIST surface with the IST logo image to render
 * \param _renderer renderer to handle all rendering in a window
 */
int RenderLogo(int x, int y, SDL_Surface *_logoIST, SDL_Renderer* _renderer)
{
	SDL_Texture *text_IST;
	SDL_Rect boardPos;

    // space occupied by the logo
	boardPos.x = x;
	boardPos.y = y;
	boardPos.w = _logoIST->w;
	boardPos.h = _logoIST->h;

    // render it
	text_IST = SDL_CreateTextureFromSurface(_renderer, _logoIST);
	SDL_RenderCopy(_renderer, text_IST, NULL, &boardPos);

    // destroy associated texture !
	SDL_DestroyTexture(text_IST);
	return _logoIST->h;
}

/**
 * RenderText function: Renders the IST Logo on the window screen
 * \param x X coordinate of the text
 * \param y Y coordinate of the text
 * \param text string where the text is written
 * \param font TTF font used to render the text
 * \param _renderer renderer to handle all rendering in a window
 */
int RenderText(int x, int y, const char *text, TTF_Font *_font, SDL_Color *_color, SDL_Renderer* _renderer)
{
	SDL_Surface *text_surface;
	SDL_Texture *text_texture;
	SDL_Rect solidRect;

	solidRect.x = x;
	solidRect.y = y;
    // create a surface from the string text with a predefined font
	text_surface = TTF_RenderUTF8_Blended(_font,text,*_color);
	if (text_surface == NULL)
	{
	    fprintf(stderr, "TTF_RenderText_Blended: %s\n", TTF_GetError());
	    exit(EXIT_FAILURE);
	}
    // create texture
	text_texture = SDL_CreateTextureFromSurface(_renderer, text_surface);
    // obtain size
	SDL_QueryTexture( text_texture, NULL, NULL, &solidRect.w, &solidRect.h );
    // render it !
	SDL_RenderCopy(_renderer, text_texture, NULL, &solidRect);

	SDL_DestroyTexture(text_texture);
	SDL_FreeSurface(text_surface);
	return solidRect.h;
}

/**
 * InitEverything: Initializes the SDL2 library and all graphical components: font, window, renderer
 * \param width width in px of the window
 * \param height height in px of the window
 * \param _img surface to be created with the table background and IST logo
 * \param _window represents the window of the application
 * \param _renderer renderer to handle all rendering in a window
 */
void InitEverything(int width, int height, TTF_Font **_font, SDL_Surface *_img[], SDL_Window** _window, SDL_Renderer** _renderer)
{
    InitSDL();
    InitFont();
    *_window = CreateWindow(width, height);
    *_renderer = CreateRenderer(width, height, *_window);

    // load the table texture
    _img[0] = IMG_Load("assets//table_texture.png");
    if (_img[0] == NULL) {
        fprintf(stderr, "Unable to load image: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // load IST logo
    _img[1] = SDL_LoadBMP("assets//ist_logo.bmp");
    if (_img[1] == NULL) {
        fprintf(stderr, "Unable to load bitmap: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // this opens (loads) a font file and sets a size
    *_font = TTF_OpenFont("assets//FreeSerif.ttf", 16);
    if(*_font == NULL) {
        fprintf(stderr, "TTF_OpenFont: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }
}

/**
 * InitSDL: Initializes the SDL2 graphic library
 */
void InitSDL()
{
    // init SDL library
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		fprintf(stderr, "Failed to initialize SDL: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
	}
}

/**
 * InitFont: Initializes the SDL2_ttf font library
 */
void InitFont()
{
	// Init font library
	if (TTF_Init() == -1) {
	    fprintf(stderr, "TTF_Init: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
	}
}

/**
 * CreateWindow: Creates a window for the application
 * \param width width in px of the window
 * \param height height in px of the window
 * \return pointer to the window created
 */
SDL_Window *CreateWindow(int width, int height)
{
    SDL_Window *window;
    // init window
	window = SDL_CreateWindow("Blackjack", WINDOW_POSX, WINDOW_POSY, width+EXTRASPACE, height, 0);
    // check for error !
	if (window == NULL) {
		fprintf(stderr, "Failed to create window : %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	return window;
}

/**
 * CreateRenderer: Creates a renderer for the application
 * \param width width in px of the window
 * \param height height in px of the window
 * \param _window represents the window for which the renderer is associated
 * \return pointer to the renderer created
 */
SDL_Renderer *CreateRenderer(int width, int height, SDL_Window *_window)
{
    SDL_Renderer *renderer;
    // init renderer
	renderer = SDL_CreateRenderer(_window, -1, 0);

	if (renderer == NULL) {
		fprintf(stderr, "Failed to create renderer : %s", SDL_GetError());
        exit(EXIT_FAILURE);
	}

	// set size of renderer to the same as window
	SDL_RenderSetLogicalSize(renderer, width+EXTRASPACE, height);

	return renderer;
}
