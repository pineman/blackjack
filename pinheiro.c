/*
 * Jogo Blackjack
 * Projeto Intermédio
 * Unidade Curricular de Programação, 2º Semestre 2015/2016
 * Data de Entrega: 03/04/2016
 * MEEC - Mestrado em Engenharia Eletrotécnica e de Computadores
 * Instituto Superior Técnico
 *
 * Autor: João Francisco de Castro Martins Calvário Pinheiro
 * Número: 84086
 * istID: ist425386
 * Email: <joao.castro.pinheiro@tecnico.ulisboa.pt>
 */

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>
#include <limits.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>

#define STRING_SIZE 100			// max size for some strings
#define WIDTH_WINDOW 900		// window width
#define HEIGHT_WINDOW 525		// window height
#define CARD_WIDTH 67			// card width
#define CARD_HEIGHT 97			// card height
#define WINDOW_POSX 0			// initial position of the window: x
#define WINDOW_POSY 0			// initial position of the window: y
#define EXTRASPACE 150
#define MARGIN 5
#define MAX_PLAYERS 4			// number of maximum players
#define MAX_DECK_SIZE 52		// number of max cards in the deck
#define MAX_CARD_HAND 11		// 11 cards max. that each player can hold
#define MAX_BET_PERCENT 0.20f	// maximum bet, 20% of initial money
#define MAX_INITIAL 999999		// limitar o número de digitos a mostrar
#define MAX_DECKS 6 			// maximum number of decks
#define MAX_CARDS MAX_DECKS * MAX_DECK_SIZE
#define HOUSE_HIT_POINTS 17
#define MAX_NAME_SIZE 10
// fracão da aposta retornada ou retirada ao jogador quando este tem blackjack
// ou foi busted, respetivamente
#define BET_FRACTION 2L

/*
 * Ninguém deveria alguma vez de ser obrigado a escrever estas definições
 * de funções.
 * Logo, eu não as escrevi.
 * Gerado automaticamente pelo programa 'makeheaders'
 * http://www.hwaci.com/sw/mkhdr/
 * ... Graças a deus.
 *
 * Regras deste projeto:
 * Não há:
 * 	- Structs (tudo se resolvia com três destas)
 * 	- Globais
 * 	- Objetos (porque estamos em C)
 * 	- goto
 * Há:
 * 	- Limite no número de linhas de uma função.
 * 	- Despenalização por ter poucas funções, mesmo que estas não
 * 	  existam por razões práticas como evitar repetição de código,
 * 	  servindo só para encurtar cada função.
 * 	- Montes de variáveis que deveriam ser globais, e na prática são porque
 * 	  qualquer função pode modificar variáveis da função invocadora
 * 	  utilizando ponteiros, exceto que se tem de passar à função
 * 	  o ponteiro explicitamente como argumento; enquanto que se a
 * 	  variável fosse global não era necessário passá-la como argumento,
 * 	  e a funcionalidade era a mesma, sem qualquer perigo.
 * 	  _MAS_ segundo http://c2.com/cgi/wiki?GlobalVariablesAreBad:
 * 	   - Really Bad Reasons to Use Global Variables:
 * 	     - "I'm a slow typist. Globals save me keystrokes."
 * 	     - "I don't want to pass it around all the time."
 * 	  Por isso eu declaro o meu programa "entregável".
 *  - Estupidez do autor do programa, que achou por bem ter três funções
 *    enormes que correm quando as teclas correspondente são primidas
 *    _E_ que se podem chamar umas às outras. Que boa jogada!
 */

// 45j ou CTRL-f ou Ctrl-d Ctrl-d ou até Page Down
SDL_Renderer *CreateRenderer(int width,int height,SDL_Window *_window);
SDL_Window *CreateWindow(int width,int height);
void InitFont();
void InitSDL();
void RenderCard(int _x,int _y,int _num_card,SDL_Surface **_cards,SDL_Renderer *_renderer);
int RenderText(int x,int y,const char *text,TTF_Font *_font,SDL_Color *_color,SDL_Renderer *_renderer);
void render_player_area(TTF_Font *_font,SDL_Renderer *_renderer,int separatorPos,int current_player,long money[MAX_PLAYERS],const long initial_money,int player_points[MAX_PLAYERS],int has_money[MAX_PLAYERS],int is_bust[MAX_PLAYERS+1],int has_blackjack[MAX_PLAYERS+1],int wins[MAX_PLAYERS],int losses[MAX_PLAYERS],int draws[MAX_PLAYERS]);
void render_student_area(TTF_Font *_font,SDL_Renderer *_renderer,int separatorPos,int height);
int RenderLogo(int x,int y,SDL_Surface *_logoIST,SDL_Renderer *_renderer);
int count_blackjack(int player_points[MAX_PLAYERS],int has_blackjack[MAX_PLAYERS]);
void end_calc(int player_points[MAX_PLAYERS],int *house_points,long money[MAX_PLAYERS],const long initial_money,const long max_bet,int is_bust[MAX_PLAYERS+1],int has_blackjack[MAX_PLAYERS+1],int has_money[MAX_PLAYERS],int wins[MAX_PLAYERS],int losses[MAX_PLAYERS],int draws[MAX_PLAYERS]);
void hit_house(int deck[MAX_CARDS],const int num_decks,const int max_cards,int *cards_left,int house_cards[MAX_CARD_HAND],int *pos_house_hand);
int initial_calc(int *pos_house_hand,int *current_player,int house_cards[MAX_CARD_HAND],int player_points[MAX_PLAYERS],int *house_points,long money[MAX_PLAYERS],const long initial_money,const long max_bet,int has_blackjack[MAX_PLAYERS],int has_money[MAX_PLAYERS],int wins[MAX_PLAYERS],int losses[MAX_PLAYERS],int draws[MAX_PLAYERS]);
int count_points(int hand[MAX_CARD_HAND],int num_cards,int *points);
void clear_hand(int hand[MAX_CARD_HAND]);
void clear(int *current_player,int house_cards[MAX_CARD_HAND],int player_cards[MAX_PLAYERS][MAX_CARD_HAND],int pos_player_hand[MAX_PLAYERS],int is_bust[MAX_PLAYERS+1],int has_blackjack[MAX_PLAYERS+1],int has_money[MAX_PLAYERS],int wins[MAX_PLAYERS],int losses[MAX_PLAYERS],int draws[MAX_PLAYERS]);
bool valid_initial_cards(int deck[MAX_CARDS],const int num_decks,const int max_cards,int *cards_left,int player_cards[][MAX_CARD_HAND],int house_cards[MAX_CARD_HAND],int pos_player_hand[MAX_PLAYERS],long money[MAX_PLAYERS],const long max_bet,int has_money[MAX_PLAYERS]);
void give_card(int deck[MAX_CARDS],const int num_decks,const int max_cards,int *cards_left,int hand[MAX_CARD_HAND],int pos);
void swap(int *a,int *b);
void insert_and_shuffle_cards(int deck[MAX_CARDS],const int num_decks,const int max_cards,int *cards_left);
void UnLoadCards(SDL_Surface **_array_of_cards);
void record_or_write_stats(long money[MAX_PLAYERS],const long initial_money,bool record,char type,int pos);
void RenderPlayerCards(int _player_cards[][MAX_CARD_HAND],int _pos_player_hand[],SDL_Surface **_cards,SDL_Renderer *_renderer);
void RenderHouseCards(int _house[],int _pos_house_hand,SDL_Surface **_cards,SDL_Renderer *_renderer);
void RenderTable(long money[MAX_PLAYERS],TTF_Font *_font,SDL_Surface *_img[],SDL_Renderer *_renderer,int current_player,int player_points[MAX_PLAYERS],int house_points,const long initial_money,int has_money[MAX_PLAYERS],int is_bust[MAX_PLAYERS+1],int has_blackjack[MAX_PLAYERS+1],int wins[MAX_PLAYERS],int losses[MAX_PLAYERS],int draws[MAX_PLAYERS]);
void stand(int deck[MAX_CARDS],const int num_decks,const int max_cards,int *cards_left,int house_cards[MAX_CARD_HAND],int *pos_house_hand,int *current_player,int player_points[MAX_PLAYERS],int *house_points,long money[MAX_PLAYERS],const long initial_money,const long max_bet,int is_bust[MAX_PLAYERS+1],int has_blackjack[MAX_PLAYERS+1],int has_money[MAX_PLAYERS],int wins[MAX_PLAYERS],int losses[MAX_PLAYERS],int draws[MAX_PLAYERS]);
void hit_player(int deck[MAX_CARDS],const int num_decks,const int max_cards,int *cards_left,int player_cards[][MAX_CARD_HAND],int house_cards[MAX_CARD_HAND],int pos_player_hand[MAX_PLAYERS],int *pos_house_hand,int *current_player,int player_points[MAX_PLAYERS],int *house_points,long money[MAX_PLAYERS],const long initial_money,const long max_bet,int is_bust[MAX_PLAYERS+1],int has_blackjack[MAX_PLAYERS+1],int has_money[MAX_PLAYERS],int wins[MAX_PLAYERS],int losses[MAX_PLAYERS],int draws[MAX_PLAYERS]);
void LoadCards(SDL_Surface **_cards);
void InitEverything(int width,int height,TTF_Font **_font,SDL_Surface *_img[],SDL_Window **_window,SDL_Renderer **_renderer);
bool new(int deck[MAX_CARDS],const int num_decks,const int max_cards,int *cards_left,int player_cards[][MAX_CARD_HAND],int house_cards[MAX_CARD_HAND],int pos_player_hand[MAX_PLAYERS],int *pos_house_hand,int *current_player,int player_points[MAX_PLAYERS],int *house_points,long money[MAX_PLAYERS],const long initial_money,const long max_bet,int is_bust[MAX_PLAYERS+1],int has_blackjack[MAX_PLAYERS+1],int has_money[MAX_PLAYERS],int wins[MAX_PLAYERS],int losses[MAX_PLAYERS],int draws[MAX_PLAYERS]);
int get_args_decks();
long get_args_money(long money[MAX_PLAYERS]);

// definition of some strings: they cannot be changed when the program is executed !
const char myName_bold[][STRING_SIZE] = {"Nome: "};
const char myName[][STRING_SIZE] =		{"João Francisco de", "Castro Martins Calvário", "Pinheiro"};

const char myNumber_bold[][STRING_SIZE] = {"Número: ", "istID: "};
const char myNumber[][STRING_SIZE] =	  {"84086", "ist425386"};

const char instructions_bold[][STRING_SIZE] = {"Instruções: ", "<h>: ", "<s>: ", "<n>:", "<q>: "};
const char instructions[][STRING_SIZE] = 	  {" ", "Hit", "Stand", "Novo Jogo", "Sair"};

char playerNames[MAX_PLAYERS][MAX_NAME_SIZE+1] = {"Player 1", "Player 2", "Player 3", "Player 4"};


/**
 * main function: entry point of the program
 * only to invoke other functions !
 */
int main()
{
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    TTF_Font *serif = NULL;
    SDL_Surface *cards[MAX_DECK_SIZE+1], *imgs[2];
    SDL_Event event;
    int delay = 300;
    int quit = 0;

	// Converter para minúsculas os nomes dos jogadores exceto a primeira
	// letra
	for (int i = 0; i < MAX_PLAYERS; i++)
		for (int j = 0; j < MAX_NAME_SIZE; j++)
			if (!j == 0)
				playerNames[i][j] = tolower(playerNames[i][j]);

	/* <19variáveis> */
	// Array com o dinheiro de cada jogador
	long money[MAX_PLAYERS] = {0};
	// Dinheiro máximo de aposta
	const long max_bet = get_args_money(money);
	// Dinheiro inicial
	const long initial_money = money[0]; // nesta altura a array está populada
	// Número de baralhos a utilizar
    const int num_decks = get_args_decks();

	// Representação interna do baralho de cartas disponível ao jogo
	// (composto entre 1 a 6 baralhos de 52 cartas)
	int deck[MAX_CARDS] = {0};
	// O número máximo de cartas que podemos alguma vez distribuir
	const int max_cards = num_decks * MAX_DECK_SIZE; // de 0 a 312
 	// O número currente de cartas que temos para distribuir
	int cards_left = 0; // de 0 a 312

	// Arrays que representam as cartas currentes de cada jogador e da casa
	int player_cards[MAX_PLAYERS][MAX_CARD_HAND] = {{0}};
    int house_cards[MAX_CARD_HAND] = {0};
	// esencialmente o número de cartas de cada jogador
    int pos_player_hand[MAX_PLAYERS] = {0};
    int pos_house_hand = 0; // = 1 para não mostrar a segunda carta ao início (até todos fazerem stand)
    // Arrays que contêm a pontuação de cada jogador e da casa
    int player_points[MAX_PLAYERS] = {0};
	int house_points = 0;
	// de 0 a MAX_PLAYERS, indica que jogador está a jogar currentemente,
	int current_player = 0;

	// Arrays que dizem quais jogadores ou a casa estão currentemente busted,
	// têm blackjack e quais jogadores têm dinheiro para jogar
	int is_bust[MAX_PLAYERS+1] = {0};
	int has_blackjack[MAX_PLAYERS+1] = {0};
	int has_money[MAX_PLAYERS] = {0};
	// Arrays que indicam quais jogadores ganharam, perderam ou empataram
	// no contento de um jogo (new() põe-nas a zero)
	int wins[MAX_PLAYERS] = {0};
	int losses[MAX_PLAYERS] = {0};
	int draws[MAX_PLAYERS] = {0};
	/* </19variáveis> */

	// Inicializar a função rand() com a semente 456 (exigida)
	//srand(time(NULL));
	srand(456);

	// distribuir as cartas (iniciais)
	new(deck, num_decks, max_cards, &cards_left,
		player_cards, house_cards, pos_player_hand,
		&pos_house_hand, &current_player,
		player_points, &house_points,
		money, initial_money, max_bet,
		is_bust, has_blackjack, has_money, wins, losses, draws);

	// initialize graphics
	InitEverything(WIDTH_WINDOW, HEIGHT_WINDOW, &serif, imgs, &window, &renderer);
    // loads the cards images
    LoadCards(cards);

	while (!quit) {
        // while there's events to handle
        while ( SDL_PollEvent(&event) ) {
			if (event.type == SDL_QUIT) {
                quit = true;
            }
			else if (event.type == SDL_KEYDOWN) {
				switch (event.key.keysym.sym) {
				// demasiada identação
				case SDLK_h: // hit
					// só fazer hit pelo teclado se for a vez dum jogador
					if (current_player < MAX_PLAYERS) {
						hit_player(deck, num_decks, max_cards, &cards_left,
									player_cards, house_cards, pos_player_hand,
									&pos_house_hand, &current_player,
									player_points, &house_points,
									money, initial_money,max_bet,
									is_bust, has_blackjack, has_money,
									wins, losses, draws);
					}
					break;
				case SDLK_s: // stand
					// só fazer stand pelo teclado se for a vez dum jogador
					if (current_player < MAX_PLAYERS) {
						stand(deck, num_decks, max_cards, &cards_left,
								house_cards, &pos_house_hand, &current_player,
								player_points, &house_points,
								money, initial_money, max_bet,
								is_bust, has_blackjack, has_money,
								wins, losses, draws);
					}
					break;
				case SDLK_n: // new round
					// só fazer new game se já todos os jogadores jogaram
					if (current_player == MAX_PLAYERS) {
						quit = new(deck, num_decks, max_cards, &cards_left,
									player_cards, house_cards, pos_player_hand,
									&pos_house_hand, &current_player,
									player_points, &house_points,
									money, initial_money, max_bet,
									is_bust, has_blackjack, has_money,
									wins, losses, draws);
					}
					break;
				case SDLK_q: // quit
					quit = true;
					break;
				default:
					break;
				}
			}
        }
        // render game table
        RenderTable(money, serif, imgs, renderer,
        			current_player, player_points, house_points, initial_money,
        			has_money, is_bust, has_blackjack, wins, losses, draws);
        // render house cards
        RenderHouseCards(house_cards, pos_house_hand, cards, renderer);
        // render player cards
        RenderPlayerCards(player_cards, pos_player_hand, cards, renderer);
        // render in the screen all changes above
        SDL_RenderPresent(renderer);
    	// add a delay
		SDL_Delay(delay);
    }

	// O jogo vai fechar, escrever stats para ficheiro
	record_or_write_stats(money, initial_money, false, 0, 0);
    // free memory allocated for images and textures and close everything
    UnLoadCards(cards);
    TTF_CloseFont(serif);
    SDL_FreeSurface(imgs[0]);
    SDL_FreeSurface(imgs[1]);
    SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return EXIT_SUCCESS;
}

/*
 * ---------------------------------------------------------------------------
 *  							PARÂMETROS
 * ---------------------------------------------------------------------------
 */

/*
 * As seguintes duas funções perguntam ao utilizador os argumentos e retornam
 * ou atualizam as variáveis necessárias, utilizando passagem por referência.
 * Utiliza-se fgets() e strtoul() para evitar overflow vindo do utilizador
 * e unsigned longs para estender o intervalo de números possíveis de serem
 * representados, pois o dinheiro inicial poderia ser arbritariamente grande.
 *
 * No entanto, obtou-se por majorá-lo pelo #define MAX_INITIAL, por causa do
 * aspeto gráfico do jogo: temos de garantir que temos espaço para mostrar
 * gráficamente um certo número de dígitos, que está naturalmente limitado.
 *
 * Mais tarde, na escrita do ficheiro de estatísticas, percebe-se que
 * o dinheiro dos jogadores terá de ser limitado no máximo
 * por LONG_MAX / MAX_PLAYERS, pois é necessário escrever o dinheiro final da casa.
 * Assim, utilizar unsigned longs para o dinheiro dos jogadores é inútil em
 * termos do tamanho que estas variáveis vão na realidade utilizar.
 */
long get_args_money(
	long money[MAX_PLAYERS]
)
{
	long money_tmp = 0, max_bet_tmp = 0; // Temporárias para testar overflow
	char buffer[100] = {0}; // Arbritário

	///* Como vamos escrever o dinheiro final da casa, que é signed,
	// * o máximo de dinheiro que cada jogador poderá ter será o máximo
	// * que signed long pode representar a dividir pelo número de jogadores
	// * (menos 1 por causa de precisão perdida nas divisões de longs) */
	//long max = (LONG_MAX / MAX_PLAYERS) - 1;
	//printf("Introduza o dinheiro inicial dos jogadores [10 - %ld]:\n", max);

	printf("Introduza o dinheiro inicial dos jogadores [10 - %d]:\n", MAX_INITIAL);
	fgets(buffer, sizeof(buffer), stdin);
	money_tmp = strtol(buffer, NULL, 10);

	//errno = 0;
	//if (money_tmp < 10 || errno == ERANGE) { // Prevenir overflow
	if (money_tmp < 10 || money_tmp > MAX_INITIAL) { // limitar digitos a mostrar
		puts("Dinheiro inicial inválido.");
		exit(EXIT_FAILURE);
	}
	for (int i = 0; i < MAX_PLAYERS; i++) // Encher a array do dinheiro dos jogadores
		money[i] = money_tmp;

	double percent = floor(MAX_BET_PERCENT * (double) money_tmp);
	printf("Introduza o valor máximo da aposta [2 - %.0f]:\n", percent);
	fgets(buffer, sizeof(buffer), stdin),
	max_bet_tmp = strtol(buffer, NULL, 10);

	if (max_bet_tmp < 2 || max_bet_tmp > percent ) {
		puts("Aposta máxima inválida.");
		exit(EXIT_FAILURE);
	}
	return max_bet_tmp;
}
int get_args_decks()
{
	int num_decks_tmp = 0; // Temporária para testar overflow
	char buffer[100] = {0}; // Arbritário

	printf("Introduza o número de baralhos a utilizar [1 - 6]:\n");
	fgets(buffer, sizeof(buffer), stdin);
	num_decks_tmp = strtoul(buffer, NULL, 10);

	if (num_decks_tmp < 1 || num_decks_tmp > MAX_DECKS) {
		puts("Número de baralhos inválido.");
		exit(EXIT_FAILURE);
	}
	return num_decks_tmp;
}


/*
 * ---------------------------------------------------------------------------
 * 								LÓGICA DO JOGO
 * ---------------------------------------------------------------------------
 */

/*
 * Função que insere cartas consoante o número de baralhos pedido na
 * array de cartas deck e depois as baralha utilizando o método
 * sugerido nas aulas teóricas.
 */
void insert_and_shuffle_cards(
	int deck[MAX_CARDS],
	const int num_decks,
	const int max_cards,
	int *cards_left
)
{
	// Inserir 0-51 inclusivé (MAX_DECK_SIZE) em cada bloco de 52 (num_decks)
	for (int i = 0; i < num_decks; i++)
		for (int j = 0; j < MAX_DECK_SIZE; j++)
			deck[(i*MAX_DECK_SIZE)+j] = j;

	/* Fisher-Yates(-Durstenfeld-Knuth) */
	int range = max_cards - 1;
	int times = 2;
	int random = 0;
	int max_before_skew = 0;

	for (int i = 0; i < times; i++) {
		for (; range > 0; range--) {
			//random = rand() % range;
			/* Averiguar o maior múltiplo de range tal que range * múltiplo é menor
			* ou igual a RAND_MAX. Isto porque se RAND_MAX não for divisível por range,
			* fazer simplesmente random = range % rand() irá ter um erro devido aos
			* números vindos de rand() que sejam maiores que range * múltiplo.
			* Explicações melhores e fontes:
			* http://www.azillionmonkeys.com/qed/random.html
			* https://en.wikipedia.org/wiki/Fisher%E2%80%93Yates_shuffle#Modulo_bias */
			max_before_skew = ((int) (RAND_MAX / range)); // Maior múltiplo
			do {
				random = rand();
				/* Pedir um número novo se o número for daqueles 'perigosos'
				 * que se falou anteriormente. */
			} while (random >= max_before_skew * range);
			// agora sim fazer o módulo do número aleatório
			// (range + 1) para o número aleatório puder chegar a ser range.
			random %= (range + 1);
			swap(&deck[range], &deck[random]);
		}
	}
	// Agora temos todas as cartas disponíveis outra vez
	*cards_left = max_cards;
}

/*
 * Função para dar uma carta do deck a uma mão (array) na posição passada.
 * Atualiza também o número de cartas disponíveis e, caso este chegar
 * a zero, insere e baralha novas cartas.
 */
void give_card(
	int deck[MAX_CARDS],
	const int num_decks,
	const int max_cards,
	int *cards_left,
	int hand[MAX_CARD_HAND],
	int pos
)
{
	// Baralhar outra vez se não tivermos mais cartas para dar
	if (*cards_left == 0)
		insert_and_shuffle_cards(deck, num_decks, max_cards, cards_left);
	// Dar a carta do topo (do fim para o início do deck, falando em índices)
	// à array passada na posição passada.
	/* Array tem 0:(MAX_CARDS-1) elementos, cards_left começa em MAX_CARDS,
	 * por isso é necessário subtrair 1 para o utilizar como índice */
	hand[pos] = deck[*cards_left-1];
	// Decrementar o número de cartas disponíveis
	(*cards_left)--;
}

/*
 * Função que distribui as cartas iniciais realisticamente usando give_card()
 * e que verifica quais jogadores podem jogar. Se nenhum puder, retorna
 * false para sinalizar que é game_over.
 */
bool valid_initial_cards(
	int deck[MAX_CARDS],
	const int num_decks,
	const int max_cards,
	int *cards_left,
	int player_cards[][MAX_CARD_HAND],
	int house_cards[MAX_CARD_HAND],
	int pos_player_hand[MAX_PLAYERS],
	long money[MAX_PLAYERS],
	const long max_bet,
	int has_money[MAX_PLAYERS]
)
{
	// indica se existe pelo menos um jogador para jogar
	bool valid_game = false;

	/* Dar uma carta a cada jogador e depois à casa e repetir */
	// Correr duas vezes...
	for (int i = 0; i < 2; i++) {
		// ...para cada jogador...
		for (int j = 0; j < MAX_PLAYERS; j++) {
			// ...dar uma carta _SE_ o jogador tiver dinheiro suficiente para
			// cobrir uma aposta.
			if (money[j] >= max_bet) {
				valid_game = true;
				has_money[j] = 1;
				give_card(deck, num_decks, max_cards, cards_left, player_cards[j], i);
				pos_player_hand[j]++;
			}
		}
		// Se nenhum jogador estiver a jogar, é game over!
		if (!valid_game)
			return false;

		// dar uma carta à casa a seguir às primeiras dos jogadores
		give_card(deck, num_decks, max_cards, cards_left, house_cards, i);
	}
	return true;
}

/*
 * Função que limpa certas variáveis antes de cada ronda
 */
void clear(
	int *current_player,
	int house_cards[MAX_CARD_HAND],
	int player_cards[MAX_PLAYERS][MAX_CARD_HAND],
	int pos_player_hand[MAX_PLAYERS],
	int is_bust[MAX_PLAYERS+1],
	int has_blackjack[MAX_PLAYERS+1],
	int has_money[MAX_PLAYERS],
	int wins[MAX_PLAYERS],
	int losses[MAX_PLAYERS],
	int draws[MAX_PLAYERS]
)
{
	// começamos no primeiro jogador sempre
	*current_player = 0;
	// limpar as cartas antigas da casa.
	clear_hand(house_cards);
	// no início ninguém está busted, tem blackjack, tem cartas, ganhou, perdeu
	// ou empatou. Como é óbvio.
	for (int i = 0; i < MAX_PLAYERS; i++) {
		clear_hand(player_cards[i]);
		pos_player_hand[i] = 0;
		is_bust[i] = 0;
		has_blackjack[i] = 0;
		has_money[i] = 0;
		wins[i] = 0;
		losses[i] = 0;
		draws[i] = 0;
	}
	// nem a casa
	is_bust[MAX_PLAYERS+1] = 0;
	has_blackjack[MAX_PLAYERS+1] = 0;
}

/*
 * Função que trata de inicializar um novo jogo.
 * Dá as cartas iniciais, identifica quem tem blackjack e assim decide
 * quem joga primeiro.
 * Se não existirem jogadores com dinheiro, retorna true, para sinalizar
 * que se deve sair do jogo.
 * Caso contrário, retorna false.
 * No caso execional de todos os jogadores terem blackjack,
 * passa logo a jogada à casa.
 */
bool new(
	int deck[MAX_CARDS],
	const int num_decks,
	const int max_cards,
	int *cards_left,
	int player_cards[][MAX_CARD_HAND],
	int house_cards[MAX_CARD_HAND],
	int pos_player_hand[MAX_PLAYERS],
	int *pos_house_hand,
	int *current_player,
	int player_points[MAX_PLAYERS],
	int *house_points,
	long money[MAX_PLAYERS],
	const long initial_money,
	const long max_bet,
	int is_bust[MAX_PLAYERS+1],
	int has_blackjack[MAX_PLAYERS+1],
	int has_money[MAX_PLAYERS],
	int wins[MAX_PLAYERS],
	int losses[MAX_PLAYERS],
	int draws[MAX_PLAYERS]
)
{
	/* Limpar variáveis de rondas passadas */
	clear(current_player, house_cards, player_cards, pos_player_hand,
		  is_bust, has_blackjack, has_money, wins, losses, draws);

	/* Distribuir as cartas realisticamente */
	bool valid_game = false;
	valid_game = valid_initial_cards(deck, num_decks, max_cards, cards_left,
									 player_cards, house_cards, pos_player_hand,
									 money, max_bet, has_money);
	if (!valid_game) {
		// draw_gameover()
		SDL_Delay(2000);
		return true;
	}

	// Retirar o dinheiro da aposta apenas se o jogador tiver dinheiro, e
	// contar os pontos do jogador.
	for (int i = 0; i < MAX_PLAYERS; i++) {
		if (has_money[i]) {
			money[i] -= max_bet;
			count_points(player_cards[i], pos_player_hand[i], &player_points[i]);
		}
	}

	// Identificar jogadores com blackjack, contar quantos têm e distribuir
	// já o dinheiro que for necessário
	int all_blackjack = 0;
	all_blackjack = initial_calc(pos_house_hand, current_player, house_cards,
								 player_points, house_points, money, initial_money,
								 max_bet, has_blackjack, has_money, wins, losses, draws);

	// Se por acaso todos os jogadores tiverem blackjack, passar à vez da
	// casa através da função stand() (*current_player é igual a MAX_PLAYERS
	// e, portanto, ela faz isso!
	if (all_blackjack) {
		stand(deck, num_decks, max_cards, cards_left,
			  house_cards, pos_house_hand, current_player,
			  player_points, house_points,
			  money, initial_money, max_bet,
			  is_bust, has_blackjack, has_money, wins, losses, draws);
	}

	return false;
}

/*
 * Função que faz stand: passa à vez do próximo jogador válido e sem blackjack
 * e trata também do "stand" da casa (faz hit enquanto os pontos são menores
 * que HOUSE_HIT_POINTS) e calcula os pontos no final da ronda.
 */
void stand(
	int deck[MAX_CARDS],
	const int num_decks,
	const int max_cards,
	int *cards_left,
	int house_cards[MAX_CARD_HAND],
	int *pos_house_hand,
	int *current_player,
	int player_points[MAX_PLAYERS],
	int *house_points,
	long money[MAX_PLAYERS],
	const long initial_money,
	const long max_bet,
	int is_bust[MAX_PLAYERS+1],
	int has_blackjack[MAX_PLAYERS+1],
	int has_money[MAX_PLAYERS],
	int wins[MAX_PLAYERS],
	int losses[MAX_PLAYERS],
	int draws[MAX_PLAYERS]
)
{
	if (*current_player < MAX_PLAYERS) {
		/* Passar à frente jogadores imediatamente a seguir que tenham blackjack
		 * ou que não possam jogar, testando-os. */
		int i = 0;
		for (i = *current_player + 1; i < MAX_PLAYERS; i++) {
			if (!(player_points[i] == 21 || !has_money[i])) {
				// encontrámos o primeiro jogador válido.
				break;
			}
		}

		/* O ciclo para quando se encontra o primeiro jogador válido, e i fica
		 * com o número desse jogador. Por isso, atualizar *current_player  */
		*current_player = i;
	}
	if (*current_player == MAX_PLAYERS) {
		/* Já todos os jogadores fizeram stand, passar à jogada da casa
		 * ("jogador" representado por (*current_player = MAX_PLAYERS) ) */
		*pos_house_hand = 2; // revelar carta

		// Atualizar pontos da casa e contar os seus ases
		int num_ace = 0;
		num_ace = count_points(house_cards, *pos_house_hand, house_points);

		/* Fazer hit da casa enquanto os pontos desta são menores que HOUSE_HIT_POINTS
		* _OU_ a casa tem um ás a 11 pontos e um conjunto qualquer de cartas
		* que totalize 6 pontos */
		while (*house_points < HOUSE_HIT_POINTS || (num_ace = 1 && *house_points == HOUSE_HIT_POINTS) ) {
			hit_house(deck, num_decks, max_cards, cards_left, house_cards, pos_house_hand);
			num_ace = count_points(house_cards, *pos_house_hand, house_points);
		}

		if (*house_points > 21) {
			// A casa foi busted
			is_bust[MAX_PLAYERS+1] = 1;
		}

		/* Agora que todas as cartas estão distribuídas, vamos ver que jogadores
		* que não tenham bust ou blackjack ganharam/empataram/perderam */
		end_calc(player_points, house_points, money, initial_money, max_bet,
				 is_bust, has_blackjack, has_money, wins, losses, draws);
	}
}

/*
 * Função que trata dos hits dos jogadores.
 * Verifica se o jogador pode fazer mais hits (senão, chama stand() nele),
 * faz hit, atualiza os seus pontos e identifica bust.
 *
 * Esta função recebe estes argumentos todos porque necessita de chamar
 * a função stand(), que precisa deles.
 * Isto é assim devido à arquitetura desta solução que refletem as decisões
 * e maneira de pensar do autor.
 * Poderão existir soluções mais repartidas, com mais funções, com mais
 * if's na função main(). Esta não é uma delas.
 * Pronto, não tem mal uma função levar 19 argumentos, certo?...
 * É melhor do que usar gotos ou globais! Certo? Não há structs...
 * Graças a deus existem programas que escrevem as declarações de funções
 * a partir das duas definições!...
 */
void hit_player(
	int deck[MAX_CARDS],
	const int num_decks,
	const int max_cards,
	int *cards_left,
	int player_cards[][MAX_CARD_HAND],
	int house_cards[MAX_CARD_HAND],
	int pos_player_hand[MAX_PLAYERS],
	int *pos_house_hand,
	int *current_player,
	int player_points[MAX_PLAYERS],
	int *house_points,
	long money[MAX_PLAYERS],
	const long initial_money,
	const long max_bet,
	int is_bust[MAX_PLAYERS+1],
	int has_blackjack[MAX_PLAYERS+1],
	int has_money[MAX_PLAYERS],
	int wins[MAX_PLAYERS],
	int losses[MAX_PLAYERS],
	int draws[MAX_PLAYERS]
)
{
	// hit do jogador *current_player
	if (pos_player_hand[*current_player] < MAX_CARD_HAND) {
		// Dentro do limite do número de cartas na mão, prosseguir
		give_card(deck, num_decks, max_cards, cards_left,
				  player_cards[*current_player], pos_player_hand[*current_player]);
		pos_player_hand[*current_player]++;
		// Atualizar os pontos do jogador
		count_points(player_cards[*current_player],
					 pos_player_hand[*current_player],
					 &player_points[*current_player]);

		// Identificar bust ou 21 pontos e fazer stand do jogador
		// Retirar a aposta se for bust
		if (player_points[*current_player] > 21) {
			// Como o dinheiro da aposta já foi retirado no início da ronda,
			// não é necessário atualizar o dinheiro do jogador.
			is_bust[*current_player] = 1;
			losses[*current_player] = 1;
			record_or_write_stats(money, initial_money, true, 'l', *current_player);
			// draw_player_bust();
		}
		if (player_points[*current_player] >= 21) {
			// 5yy
			stand(deck, num_decks, max_cards, cards_left,
				  house_cards, pos_house_hand, current_player,
				  player_points, house_points,
				  money, initial_money, max_bet,
				  is_bust, has_blackjack, has_money, wins, losses, draws);
		}
	}
	else {
		/* O jogador atingiu o número máximo de cartas na mão,
		 * por isso fazer stand do jogador. */
		// 5pp
		stand(deck, num_decks, max_cards, cards_left,
			  house_cards, pos_house_hand, current_player,
			  player_points, house_points,
			  money, initial_money, max_bet,
			  is_bust, has_blackjack, has_money, wins, losses, draws);
	}
}
/*
 * Função que trata dos hits da casa. Simples
 */
void hit_house(
	int deck[MAX_CARDS],
	const int num_decks,
	const int max_cards,
	int *cards_left,
	int house_cards[MAX_CARD_HAND],
	int *pos_house_hand
)
{
	// hit da casa
	if (*pos_house_hand < MAX_CARD_HAND) {
		give_card(deck, num_decks, max_cards, cards_left, house_cards, *pos_house_hand);
		(*pos_house_hand)++;
	}
	else {
		// A casa atingiu o limite de cartas na mão.
		/* Não acontece nada, a casa fica com a pontuação que tem e
		 * espera-se um novo jogo. */
	}
}

/*----------------------------------------------------------------------------
 * 							PONTUAÇÃO E APOSTAS
 *----------------------------------------------------------------------------
 */

/*
 * Esta função conta os pontos da mão passada, colocando a pontuação num
 * inteiro passado por referência à função. Esta função retorna ainda o número
 * de ases que a mão tem, que é usado para o caso especial de hit da casa
 * caso esta tenha um ás e um conjunto de cartas que totalize 6 pontos.
 */
int count_points(int hand[MAX_CARD_HAND], int num_cards, int *points)
{
	// 0 - 7 : números 2 - 9 : 2 - 9 pontos
	// 8 - 11 : dez valete dama rei : 10 pontos
	// 12 : ás : 11 pontos
	int type = 0;
	int num_ace = 0;
	*points = 0;
	for (int i = 0; i < num_cards; i++) {
		/* Isto funciona porque os valores 0-12 representam todas as pontuações
		 * associadas a cartas (sem contar com os naipes),
		 * ou seja, o período da função de contar pontos é 13 (12 por causa
		 * dos índices começarem em 0)... se quisermos ser matemáticos. */
		type = hand[i] % 13;
		if (type <= 7) { // números
			*points += type + 2;
		}
		else if (type <= 11) { // 10 ou figuras
			*points += 10;
		}
		else { // ás
			*points += 11;
			num_ace++;
		}
	}

	/* "Despromover" um ás para 1 ponto se o jogador tiver mais de 21 pontos,
	 * o que é equivalente a retirar 10 pontos à pontuação do jogador
	 * por cada às "despromovido"! */
	while (*points > 21 && num_ace) {
		// Enquanto o jogador tiver mais que 21 pontos e pelo menos um ás
		*points -= 10;
		num_ace--;
	}
	// retornar o número de ases da mão, que é opcionalmente usado pela
	// função invocadora.
	return num_ace;
}

/*
 * Função que conta e calcula blackjacks
 */
int count_blackjack(int player_points[MAX_PLAYERS], int has_blackjack[MAX_PLAYERS])
{
	int num_blackjacks = 0;
	for (int i = 0; i < MAX_PLAYERS; i++) {
		if (player_points[i] == 21) {
			// O jogador tem blackjack
			// Alertar o jogador que este tem blackjack
			// draw_player_blackjack();
			has_blackjack[i] = 1;
			num_blackjacks++;
		}
	}
	return num_blackjacks;
}

/*
 * Função para calcular a pontuação, o primeiro jogador a jogar e o dinheiro
 * caso haja blackjacks (isto logo no início da ronda)
 */
int initial_calc(
	int *pos_house_hand,
	int *current_player,
	int house_cards[MAX_CARD_HAND],
	int player_points[MAX_PLAYERS],
	int *house_points,
	long money[MAX_PLAYERS],
	const long initial_money,
	const long max_bet,
	int has_blackjack[MAX_PLAYERS],
	int has_money[MAX_PLAYERS],
	int wins[MAX_PLAYERS],
	int losses[MAX_PLAYERS],
	int draws[MAX_PLAYERS]
)
{
	// Calcular pontos da casa
	count_points(house_cards, 2, house_points);
	// Calcular blackjacks
	int num_blackjacks = count_blackjack(player_points, has_blackjack);

	int all_blackjack = 0;
	if (*house_points == 21) {
		// A casa tem blackjack
		*pos_house_hand = 2; // Revelar logo as cartas
		*current_player = MAX_PLAYERS; // Passar à vez da casa
		has_blackjack[MAX_PLAYERS+1]= 1;
		for (int i = 0; i < MAX_PLAYERS; i++) {
			if (has_blackjack[i]) {
				// O jogador tem blackjack, é empate
				/* Nesta altura, jogadores sem dinheiro suficiente para jogar
				 * não têm sequer cartas, por isso, não têm blackjack e logo
				 * não nos preocupamos com eles. */
				money[i] += max_bet;
				draws[i] = 1;
				record_or_write_stats(money, initial_money, true, 'd', i);
			}
			else {
				// O jogador não tem blackjack: perdeu a aposta.
				losses[i] = 1;
				record_or_write_stats(money, initial_money, true, 'l', i);
				// não é necessário atualizar o seu dinheiro.
			}
		}
	}
	else {
		// A casa não tem blackjack
		// colocar pos_house_hand a 1 para esconder a segunda carta da casa
		*pos_house_hand = 1;
		// Calcular pontos outra vez, mas apenas de uma carta
		count_points(house_cards, 1, house_points);
		/* Dar a vez ao primeiro jogador sem blackjack e que esteja a jogar. */
		int found = 0; // temporária, diz quando encontrámos o primeiro jogador válido
		for (int i = 0; i < MAX_PLAYERS; i++) {
			/* se o jogador não tiver dinheiro ou tiver blackjack a sua vez é
			* passada à frente se não tivermos já encontrado o primeiro jogador
			* válido */
			if (!has_money[i] || has_blackjack[i]) {
				// jogadores sem dinheiro ou jogadores com blackjack
				if (!found)
					(*current_player)++;
			}
			else {
				/* encontrámos o primeiro jogador válido, temos de parar de
				 * passar vezes à frente (i.e., parar de incrementar *current_player) */
				found = 1;
			}
			if (has_blackjack[i]) {
				// Se o jogador tem blackjack, tratar já dele.
				/* Atribuir 1 + 1 / BET_FACTION por cento do valor da aposta.
				 * Este valor arredonda sempre para baixo,
				 * porque divisão de inteiros ignora a parte decimal. */
				money[i] += max_bet + (max_bet / BET_FRACTION);
				wins[i] = 1;
				record_or_write_stats(money, initial_money, true, 'w', i);
			}
		}
		// Se por acaso todos os jogadores tiverem blackjack, dar a vez à casa.
		if (num_blackjacks == MAX_PLAYERS) {
			all_blackjack = 1;
		}
	}
	return all_blackjack;
}

/*
 * Função que calcula quem ganha/empata/perde no final da ronda
 * (chamada mesmo quando a casa tem blackjack), mas não existem
 * casos repetidos devido à lógica da função.
 * Por exemplo, quando a casa tem blackjack _E_ um jogador também tem,
 * este jogador é ignorado (já se averiguou que este foi empatado em
 * initial_calc() porque o primeiro loop 'for' loop só corre para os jogadores
 * não busted, sem blackjack e com dinheiro devido ao primeiro if.
 */
void end_calc(
	int player_points[MAX_PLAYERS],
	int *house_points,
	long money[MAX_PLAYERS],
	const long initial_money,
	const long max_bet,
	int is_bust[MAX_PLAYERS+1],
	int has_blackjack[MAX_PLAYERS+1],
	int has_money[MAX_PLAYERS],
	int wins[MAX_PLAYERS],
	int losses[MAX_PLAYERS],
	int draws[MAX_PLAYERS]
)
{
	for (int i = 0; i < MAX_PLAYERS; i++) {
		if (!(is_bust[i] || has_blackjack[i] || !has_money[i])) {
			// Para os jogadores não busted, sem blackjack e com dinheiro
			if (*house_points <= 21) {
				// A casa fez stand com 21 pontos ou menos
				if (player_points[i] < *house_points) {
					// O jogador perdeu, não lhe é devolvido o valor da
					// aposta
					/* Neste caso, como já retirámos o valor da aposta
					* no início da ronda, não é necessário atualizar
					* o dinheiro do jogador */
					losses[i] = 1;
					record_or_write_stats(money, initial_money, true, 'l', i);
				}
				else if (player_points [i] > *house_points) {
					// O jogador ganhou, é lhe devolvido o valor da aposta
					// e ganha o valor da aposta também
					money[i] += 2 * max_bet;
					wins[i] = 1;
					record_or_write_stats(money, initial_money, true, 'w', i);
				}
				else {
					// Empate, o jogador ganha o dinheiro da aposta de volta
					money[i] += max_bet;
					draws[i] = 1;
					record_or_write_stats(money, initial_money, true, 'd', i);
				}
			}
			else {
				// A casa foi busted, devolver o dinheiro da aposta
				// mais o valor da aposta outra vez a todos os jogadores
				// que não foram busted ou não têm blackjack (ou seja,
				// os jogadores neste loop)
				record_or_write_stats(money, initial_money, true, 'w', i);
				money[i] += 2 * max_bet;
				wins[i] = 1;
			}
		}
	}
}

/*
 * Função que grava as estatísticas do jogo e escreve um ficheiro com elas
 * quando se sai do jogo.
 */
void record_or_write_stats(
	long money[MAX_PLAYERS],
	const long initial_money,
	bool record,
	char type,
	int pos
)
{
	// Variáveis internas de contagem
	/* Eu sei que sou horrível por usar statics,
	 * mas facilita! Eu juro! */
	static int wins[MAX_PLAYERS] = {0};
	static int losses[MAX_PLAYERS] = {0};
	static int draws[MAX_PLAYERS] = {0};

	if (record) {
		// guardar dados
		switch(type) {
			case 'w':
				wins[pos]++;
				break;
			case 'l':
				losses[pos]++;
				break;
			case 'd':
				draws[pos]++;
				break;
			default:
				break;
		}
	}
	else {
		// escrever dados
		FILE *file;

		long house_money = 0;

		file = fopen("stats.txt", "w");
		for (int i = 0; i < MAX_PLAYERS; i++) {
			fprintf(file, "%s:\n", playerNames[i]);
			fprintf(file, "\tDinheiro final: %ld %s\n", money[i],
					money[i] == 1 ? "euro" : "euros");
			fprintf(file, "\tJogos ganhos: %d\n", wins[i]);
			fprintf(file, "\tJogos perdidos: %d\n", losses[i]);
			fprintf(file, "\tJogos empatados: %d\n", draws[i]);
			fputs("\n", file);
			// O dinheiro que a casa ganha ou perde é o inverso da soma
			// do dinheiro que os jogadores ganharam ou perderam!
			house_money += -1 * (money[i] - initial_money);
		}

		if (house_money < 0)
			fprintf(file, "Casa perdeu: %ld %s\n", house_money,
					house_money == -1 ? "euro" : "euros");
		else if (house_money > 0)
			fprintf(file, "Casa ganhou: %ld %s\n", house_money,
					house_money == 1 ? "euro" : "euros");
		else
			fprintf(file, "A casa não perdeu nem ganhou dinheiro.\n");

		fclose(file);
	}
}

/* Mini funções de recurso */
void clear_hand(int hand[MAX_CARD_HAND])
{
	for (int i = 0; i < MAX_CARD_HAND; i++)
		hand[i] = 0;
}
void swap(int *a, int *b)
{
	int tmp = 0;
	tmp = *a;
	*a = *b;
	*b = tmp;
}

/*
 * ---------------------------------------------------------------------------
 * 								PARTE GRÁFICA
 * ---------------------------------------------------------------------------
 */

/**
 * RenderTable: Draws the table where the game will be played, namely:
 * -  some texture for the background
 * -  the right part with the IST logo and the student name and number
 * -  squares to define the playing positions of each player
 * -  names and the available money for each player
 * \param money amount of money of each player
 * \param _img surfaces where the table background and IST logo were loaded
 * \param _renderer renderer to handle all rendering in a window
 */
void RenderTable(
	long money[MAX_PLAYERS],
	TTF_Font *_font,
	SDL_Surface *_img[],
	SDL_Renderer *_renderer,
	int current_player,
	int player_points[MAX_PLAYERS],
	int house_points,
	const long initial_money,
	int has_money[MAX_PLAYERS],
	int is_bust[MAX_PLAYERS+1],
	int has_blackjack[MAX_PLAYERS+1],
	int wins[MAX_PLAYERS],
	int losses[MAX_PLAYERS],
	int draws[MAX_PLAYERS]
)
{
    SDL_Color white = {255, 255, 255, 0};
    SDL_Color black = {0, 0, 0, 0};
    SDL_Color red = {255, 0, 0, 0};
    SDL_Texture *table_texture;
    SDL_Rect tableSrc, tableDest;
    char house_points_str[STRING_SIZE];
    int separatorPos = (int)(0.95f*WIDTH_WINDOW); // seperates the left from the right part of the window
    int height;

    // set color of renderer to some color
    SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);

    // clear the window
    SDL_RenderClear(_renderer);

    tableDest.x = tableSrc.x = 0;
    tableDest.y = tableSrc.y = 0;
    tableSrc.w = _img[0]->w;
    tableSrc.h = _img[0]->h;

    tableDest.w = separatorPos;
    tableDest.h = HEIGHT_WINDOW;

    table_texture = SDL_CreateTextureFromSurface(_renderer, _img[0]);
    SDL_RenderCopy(_renderer, table_texture, &tableSrc, &tableDest);

    // render the IST Logo
    height = RenderLogo(separatorPos+24, 0, _img[1], _renderer);

	// Renderizar o texto à direita
	render_student_area(_font, _renderer, separatorPos, height);

	// Renderizar os rectângulos de cada jogador
	render_player_area(_font, _renderer, separatorPos, current_player,
					   money, initial_money, player_points,
					   has_money, is_bust, has_blackjack,
					   wins, losses, draws);

	// Renderizar os pontos da casa
	sprintf(house_points_str,"Casa: %02d pontos", house_points);
	/* Coordenadas para não fazer conflito: a casa pode ter no máximo
	 * MAX_CARD_HAND cartas! */
	RenderText(0, 116, house_points_str, _font, &white, _renderer);
    // Renderizar bust ou blackjack da casa
    if (is_bust[MAX_PLAYERS+1]) {
		RenderText(105, 117, "BUST", _font, &red, _renderer);
	}
	else if (has_blackjack[MAX_PLAYERS+1]) {
		RenderText(105, 117, "BLACKJACK", _font, &black, _renderer);
	}


    // destroy everything
    SDL_DestroyTexture(table_texture);
}

/*
 * Para obter o resultado esperado, o código ficou bastante longo, devido
 * à necessidade de trocar sucessivamente o estilo da fonte entre bold
 * e normal. Esta parte pode-se reduzir utilizando ciclos for para
 * renderizar primeiro todo o texto bold e depois todo o texto normal,
 * utilizando os offsets e margens necessários. No entanto, obtou-se por
 * deixar assim para as intenções deste código ficarem bem claras.
*/
void render_student_area(
	TTF_Font *_font,
	SDL_Renderer *_renderer,
	int separatorPos,
	int height
)
{
    SDL_Color black = {0, 0, 0, 0};
    // render the student name
    // parte a bold
	TTF_SetFontStyle(_font, TTF_STYLE_BOLD);
	RenderText(separatorPos+MARGIN, height, myName_bold[0], _font, &black, _renderer);
	// parte a normal, na mesma linha
	TTF_SetFontStyle(_font, TTF_STYLE_NORMAL);
	height += RenderText(separatorPos+56, height, myName[0], _font, &black, _renderer);

	// 2 novas linhas para o meu nome enorme
	height += RenderText(separatorPos+3*MARGIN, height, myName[1], _font, &black, _renderer);
	height += RenderText(separatorPos+3*MARGIN, height, myName[2], _font, &black, _renderer);

    // this renders the student number
    // uma parte a bold, outra a normal, na mesma linha
	TTF_SetFontStyle(_font, TTF_STYLE_BOLD);
	RenderText(separatorPos+MARGIN, height, myNumber_bold[0], _font, &black, _renderer);
	TTF_SetFontStyle(_font, TTF_STYLE_NORMAL);
	height += RenderText(separatorPos+70, height, myNumber[0], _font, &black, _renderer);
	// istID
	TTF_SetFontStyle(_font, TTF_STYLE_BOLD);
	RenderText(separatorPos+MARGIN, height, myNumber_bold[1], _font, &black, _renderer);
	TTF_SetFontStyle(_font, TTF_STYLE_NORMAL);
	height += RenderText(separatorPos+50, height, myNumber[1], _font, &black, _renderer);

    // 4 * newlines
    height += 4*RenderText(separatorPos+MARGIN, height, " ", _font, &black, _renderer);

    // Fazer render das instruções da mesma maneira
	TTF_SetFontStyle(_font, TTF_STYLE_BOLD);
	height += RenderText(separatorPos+MARGIN, height, instructions_bold[0], _font, &black, _renderer);
	TTF_SetFontStyle(_font, TTF_STYLE_NORMAL);
	// A parte dos controlos do jogo é repetida e por isso resolvida com um ciclo for
    for (int i = 1; i < (int) (sizeof(instructions) / STRING_SIZE); i++) {
		TTF_SetFontStyle(_font, TTF_STYLE_BOLD);
		RenderText(separatorPos+3*MARGIN, height, instructions_bold[i], _font, &black, _renderer);
		TTF_SetFontStyle(_font, TTF_STYLE_NORMAL);
		height += RenderText(separatorPos+53, height, instructions[i], _font, &black, _renderer);
	}
}


// renders the squares + name for each player
// renders the areas for each player: names and money too !
/*
 * Modificações para mostrar os pontos do jogador, se tem blackjack ou bust,
 * se ganhou/empatou/perdeu, e o valor da última diferença no seu dinheiro.
 */
void render_player_area(
	TTF_Font *_font,
	SDL_Renderer *_renderer,
	int separatorPos,
	int current_player,
	long money[MAX_PLAYERS],
	const long initial_money,
	int player_points[MAX_PLAYERS],
	int has_money[MAX_PLAYERS],
	int is_bust[MAX_PLAYERS+1],
	int has_blackjack[MAX_PLAYERS+1],
	int wins[MAX_PLAYERS],
	int losses[MAX_PLAYERS],
	int draws[MAX_PLAYERS]
)
{
    SDL_Color black = {0, 0, 0, 0};
    SDL_Color white = {255, 255, 255, 0};
    SDL_Color red = {255, 0, 0, 0};
    SDL_Color green = {0, 255, 0, 0};
    SDL_Color color = {0, 0, 0, 0}; // cor para a renderização da diferença
    SDL_Rect playerRect;
    char name_money_str[STRING_SIZE];
    char player_points_str[STRING_SIZE];

    // Variáveis internas que guardam os estados anteriores
    static long money_before[MAX_PLAYERS] = {0};
    static long money_diff_before[MAX_PLAYERS] = {0};
    // Variáveis internas que guardam as diferenças entre estados anteriores
    // e atuais
    long money_diff[MAX_PLAYERS] = {0};

	// Construção para a inicialização do dinheiro anterior para o inicial
    static int initial = 1;
    if (initial) {
		for (int i = 0; i < MAX_PLAYERS; i++) {
			money_before[i] = initial_money;
		}
		initial = 0;
	}

    for ( int i = 0; i < MAX_PLAYERS; i++)
    {
    	if (has_money[i]) {
    		// Apenas desenhar o retângulo do jogador se este estiver a jogar
			SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255); // branco
			if (i == current_player) {
				SDL_SetRenderDrawColor(_renderer, 255, 0, 0, 255); // vermelho
			}
			playerRect.x = i*(separatorPos/4-5)+10;
			playerRect.y = (int) (0.55f*HEIGHT_WINDOW);
			playerRect.w = separatorPos/4-5;
			playerRect.h = (int) (0.42f*HEIGHT_WINDOW);

			// Calcular diferença no dinheiro
			money_diff[i] = money[i] - money_before[i];

			// Guardar para a próxima vez
			money_before[i] = money[i];

			if (money_diff[i] != 0) {
				// A diferença mudou, renderizar valor atual
				sprintf(name_money_str,"%s: %ld€ (%+ld)", playerNames[i], money[i], money_diff[i]);
				// E guardar a diferença para fazermos render no else
				money_diff_before[i] = money_diff[i];
				// Renderizar a verde se a diferença for positiva, vermelho se for negativa
				color = money_diff[i] > 0 ? green : red;
			}
			else {
				// A diferença não mudou, renderizar o valor calculado anteriormente
				// (a branco)
				sprintf(name_money_str,"%s: %ld€ (%+ld)", playerNames[i], money[i], money_diff_before[i]);
				color = white;
			}
			RenderText(playerRect.x, playerRect.y-30, name_money_str, _font, &color, _renderer);

			// Renderizar os pontos atuais de cada jogador
			sprintf(player_points_str,"%02d pontos", player_points[i]);
			RenderText(playerRect.x, playerRect.y-46, player_points_str, _font, &white, _renderer);

			// Renderizar BUST ou BLACKJACK
			// Coordenadas a partir do retângulo
			int info_y = playerRect.y - 46;
			int info_x = playerRect.x + 67; // deixar espaço para os pontos

			if (is_bust[i])
				RenderText(info_x, info_y, "BUST", _font, &red, _renderer);
			else if (has_blackjack[i])
				RenderText(info_x, info_y, "BLACKJACK", _font, &black, _renderer);

			// Renderizar WIN, LOSE ou DRAW a underline
			TTF_SetFontStyle(_font, TTF_STYLE_UNDERLINE);
			// Coordenada em x a partir do canto superior esquerdo do retângulo
			int result_x = playerRect.x + playerRect.w - 38;
			if (wins[i])
				RenderText(result_x+8, info_y, "win", _font, &green, _renderer);
			else if (losses[i])
				RenderText(result_x+7, info_y, "lose", _font, &red, _renderer);
			else if (draws[i])
				RenderText(result_x, info_y, "draw", _font, &white, _renderer);
			TTF_SetFontStyle(_font, TTF_STYLE_NORMAL);


			SDL_RenderDrawRect(_renderer, &playerRect);
		}
    }
}

/**
 * RenderHouseCards: Renders cards of the house
 * \param _house vector with the house cards
 * \param _pos_house_hand position of the vector _house with valid card IDs
 * \param _cards vector with all loaded card images
 * \param _renderer renderer to handle all rendering in a window
 */
void RenderHouseCards(int _house[], int _pos_house_hand, SDL_Surface **_cards, SDL_Renderer* _renderer)
{
    int card, x, y;
    int div = WIDTH_WINDOW/CARD_WIDTH;

    // drawing all house cards
    for ( card = 0; card < _pos_house_hand; card++)
    {
        // calculate its position
        x = (div/2-_pos_house_hand/2+card)*CARD_WIDTH + 15;
        y = (int) (0.26f*HEIGHT_WINDOW);
        // render it !
        RenderCard(x, y, _house[card], _cards, _renderer);
    }
    // just one card ?: draw a card face down
    if (_pos_house_hand == 1)
    {
        x = (div/2-_pos_house_hand/2+1)*CARD_WIDTH + 15;
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
void RenderPlayerCards(int _player_cards[][MAX_CARD_HAND], int _pos_player_hand[], SDL_Surface **_cards, SDL_Renderer* _renderer)
{
    int pos, x, y, num_player, card;

    // for every card of every player
    for ( num_player = 0; num_player < MAX_PLAYERS; num_player++)
    {
        for ( card = 0; card < _pos_player_hand[num_player]; card++)
        {
            // draw all cards of the player: calculate its position: only 4 positions are available !
            pos = card % 4;
            x = (int) num_player*((0.95f*WIDTH_WINDOW)/4-5)+(card/4)*12+15;
            y = (int) (0.55f*HEIGHT_WINDOW)+10;
            if ( pos == 1 || pos == 3) x += CARD_WIDTH + 30;
            if ( pos == 2 || pos == 3) y += CARD_HEIGHT+ 10;
            // render it !
            RenderCard(x, y, _player_cards[num_player][card], _cards, _renderer);
        }
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
    int i;
    char filename[STRING_SIZE];

     // loads all cards to an array
    for (i = 0 ; i < MAX_DECK_SIZE; i++ )
    {
        // create the filename !
        sprintf(filename, ".//cartas//carta_%02d.png", i+1);
        // loads the image !
        _cards[i] = IMG_Load(filename);
        // check for errors: deleted files ?
        if (_cards[i] == NULL)
        {
            printf("Unable to load image: %s\n", SDL_GetError());
            exit(EXIT_FAILURE);
        }
    }
    // loads the card back
    _cards[i] = IMG_Load(".//cartas//carta_back.jpg");
    if (_cards[i] == NULL)
    {
        printf("Unable to load image: %s\n", SDL_GetError());
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
	if(!text_surface)
	{
	    printf("TTF_RenderUTF8_Blended: %s\n", TTF_GetError());
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
    _img[0] = IMG_Load("table_texture.png");
    if (_img[0] == NULL)
    {
        printf("Unable to load image: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // load IST logo
    _img[1] = SDL_LoadBMP("ist_logo.bmp");
    if (_img[1] == NULL)
    {
        printf("Unable to load bitmap: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

	// this opens (loads) a font file and sets a size
    *_font = TTF_OpenFont("FreeSerif.ttf", 16);
    if(!*_font)
	{
		printf("TTF_OpenFont: %s\n", TTF_GetError());
		exit(EXIT_FAILURE);
	}
}

/**
 * InitSDL: Initializes the SDL2 graphic library
 */
void InitSDL()
{
    // init SDL library
	if ( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
	{
		printf(" Failed to initialize SDL : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
	}
}

/**
 * InitFont: Initializes the SDL2_ttf font library
 */
void InitFont()
{
	// Init font library
	if(TTF_Init()==-1)
	{
	    printf("TTF_Init: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
	}
}

/**
 * CreateWindow: Creates a window for the application
 * \param width width in px of the window
 * \param height height in px of the window
 * \return pointer to the window created
 */
SDL_Window* CreateWindow(int width, int height)
{
    SDL_Window *window;
    // init window
	window = SDL_CreateWindow( "BlackJack", WINDOW_POSX, WINDOW_POSY, width+EXTRASPACE, height, 0 );
    // check for error !
	if ( window == NULL )
	{
		printf("Failed to create window : %s\n", SDL_GetError());
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
SDL_Renderer* CreateRenderer(int width, int height, SDL_Window *_window)
{
    SDL_Renderer *renderer;
    // init renderer
	renderer = SDL_CreateRenderer( _window, -1, 0 );

	if ( renderer == NULL )
	{
		printf("Failed to create renderer : %s", SDL_GetError());
        exit(EXIT_FAILURE);
	}

	// set size of renderer to the same as window
	SDL_RenderSetLogicalSize( renderer, width+EXTRASPACE, height );

	return renderer;
}
