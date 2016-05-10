/* Autor: João Freitas 84093
 * Projecto: Blackjack
 * Entega intermedia: 3 de Abril
 */

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define STRING_SIZE 100       // max size for some strings
#define WIDTH_WINDOW 900      // window width
#define HEIGHT_WINDOW 525     // window height
#define MAX_DECK_SIZE 52      // number of max cards in the deck
#define MAX_CARD_HAND 11      // 11 cards max. that each player can hold
#define CARD_WIDTH 67         // card width
#define CARD_HEIGHT 97        // card height
#define WINDOW_POSX 500       // initial position of the window: x
#define WINDOW_POSY 250       // initial position of the window: y
#define EXTRASPACE 150
#define MARGIN 5
#define MAX_PLAYERS 4         // maximum nuber of players
#define STATS 	3			  // stats:	win/draw/lose
#define LEN	1000			  // maximum length for parameter reading

// declaration of the functions related to graphical issues
void InitEverything(int , int , TTF_Font **, SDL_Surface **, SDL_Window ** , SDL_Renderer ** );
void InitSDL();
void InitFont();
SDL_Window* CreateWindow(int , int );
SDL_Renderer* CreateRenderer(int , int , SDL_Window *);
int RenderText(int , int , const char* , TTF_Font *, SDL_Color *, SDL_Renderer *);
int RenderLogo(int , int , SDL_Surface *, SDL_Renderer * );
void RenderTable(int [], TTF_Font *, SDL_Surface **, SDL_Renderer *, int);
void RenderCard(int , int , int , SDL_Surface **, SDL_Renderer * );
void RenderHouseCards(int [], int , SDL_Surface **, SDL_Renderer * );
void RenderPlayerCards(int [][MAX_CARD_HAND], int [], SDL_Surface **, SDL_Renderer *);
void LoadCards(SDL_Surface **);
void UnLoadCards(SDL_Surface **);

// declaration of game mechanics related functions
int read_param(int *, int*, int *);
void init_deck(int *, int);
void shuffle(int *, int);
void swap(int *, int *);
void first_round(int *, int *, int [], int [], int [], int [][MAX_CARD_HAND], int, int []);
int count_points(int [], int);
int point_index(int);
void hit(int , int *, int [], int [][MAX_CARD_HAND], int [], int [], int);
void hit_house(int *, int *, int [], int [], int *, int, int []);
void finish_game(int *, int [][MAX_CARD_HAND], int [], int [], int *, int []);
void new_game(int [], int [][MAX_CARD_HAND], int [], int *, int [], int , int []);
void take_bets(int [], int, int []);
void pay_bets(int [], int [], int [], int);
void no_money(int [], int [], int );
void status_func(int [], int, int [], int [][STATS], int [], int []);
void stats_txt(int [], int , int [][STATS]);
void reshuffle(int [], int, int *);
int give_card(int [], int [], int *, int *, int);

//Graphics
void render_bust(SDL_Renderer *, TTF_Font *, int []);
void render_blackjack(SDL_Renderer *, TTF_Font *, int []);
void render_points(SDL_Renderer *, TTF_Font *, int []);
void render_house_points(SDL_Renderer *, TTF_Font *, int);
void render_status(SDL_Renderer *, TTF_Font *, int, int [], int, int []);
void render_instructions(SDL_Renderer *, TTF_Font *);



// definition of some strings: they cannot be changed when the program is executed !
const char myName[] = "João Freitas";
const char myNumber[] = "IST84093";
const char *playerNames[] = {"Player 1", "Player 2", "Player 3", "Player 4"};

/**
 * main function: entry point of the program
 * only to invoke other functions !
 */
int main( int argc, char* args[] )
{
	//variable declaration
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    TTF_Font *serif = NULL;
    SDL_Surface *cards[MAX_DECK_SIZE+1], *imgs[2];
    SDL_Event event;
    int delay = 300;
    int quit = 0;
    int money[MAX_PLAYERS] = {0};
	
	int bet = 0;
	int decks = 0;
	int money_player = 0;

	int player_cards[MAX_PLAYERS][MAX_CARD_HAND] = {{0}};
	int pos_player_hand[MAX_PLAYERS] = {0};
    int house_cards[MAX_CARD_HAND] = {0};
    int pos_house_hand = 0;
    int deck[6*MAX_DECK_SIZE] = {0};
    
    int player_count = 0;
    int index = 0;

    int player_points[MAX_PLAYERS] = {0};
	int house_points = 0;
    
    int blackjack[MAX_PLAYERS+1] = {0};
    int bankrupt[MAX_PLAYERS] = {0};
    int status_count[MAX_PLAYERS][STATS] = {{0}};
    int current_status[MAX_PLAYERS] = {0};


    srand(456);
    
    money_player = read_param(money, &decks, &bet);	//read game parameters
    //initialize vector with orderer vector
    init_deck(deck, decks);	
    //shuffle cards
    shuffle(deck, decks);	
    //deal cards in first round  
    first_round(&index, &pos_house_hand, house_cards, pos_player_hand, deck, player_cards, decks, bankrupt);	
    //count player points
    new_game(player_points, player_cards, pos_player_hand, &house_points, house_cards, pos_house_hand, blackjack);	
    // take the players bets
    take_bets(money, bet, bankrupt);
    // initialize graphics
	InitEverything(WIDTH_WINDOW, HEIGHT_WINDOW, &serif, imgs, &window, &renderer); 
	// loads the cards images+
    LoadCards(cards); 

 	//ENGINE!
    while( quit == 0 )
    {
    	// while there's events to handle
        while( SDL_PollEvent( &event ) )
        {
        	// Exit game if all players are bankrupt
        	if (bankrupt[0] && bankrupt[1] && bankrupt[2] && bankrupt[3]) {
        		stats_txt(money, money_player, status_count);
            	quit=1;
            }

            //  Jumps to next player allowed to play
           	while ((blackjack[player_count] || player_points[player_count] >= 21 || bankrupt[player_count]) && player_count < MAX_PLAYERS) {
           		// end game if player is last player allowed to play
           		if (player_count == MAX_PLAYERS -1) {
               		hit_house(&index, &pos_house_hand, house_cards, deck, &house_points, decks, blackjack);
              	  	house_points = count_points(house_cards, pos_house_hand);
              	  	status_func(player_points, house_points, current_status, status_count, bankrupt, blackjack);
               		pay_bets(money, blackjack, current_status, bet);
               		
           		}
           		//next player
           		player_count++;
        	}
            if( event.type == SDL_QUIT )
            {
            	//quit game
            	stats_txt(money, money_player, status_count);
            	quit=1;
            }
            else if ( event.type == SDL_KEYDOWN )
            {
                switch ( event.key.keysym.sym )
                {
                    case SDLK_s:
                        // stand !
                        // todo
                    	// Controls stand; only works before end of gmae
                    	if (!bankrupt[player_count] && player_count <= MAX_PLAYERS-1) {
                    		// finish game when last player does stand
                        	if (player_count == MAX_PLAYERS-1) {
                        		hit_house(&index, &pos_house_hand, house_cards, deck, &house_points, decks, blackjack);
                               	house_points = count_points(house_cards, pos_house_hand);	
                               	status_func(player_points, house_points, current_status, status_count, bankrupt, blackjack);
                               	pay_bets(money, blackjack, current_status, bet);
                               	
                           	}
                        }
                        //next player
                        player_count++;

                        break;

                    case SDLK_h:
                        // hit !
						// Controls hit; only works before end of round
                        if (player_count<MAX_PLAYERS && pos_player_hand[player_count]<=11) {
                        	// give player a card and recount points
                            hit(player_count, &index, pos_player_hand, player_cards, deck, bankrupt, decks);
                            player_points[player_count] = count_points(player_cards[player_count], pos_player_hand[player_count]);
                        }
                        break;

                    case SDLK_n:
                        //new game !
                    	// Controls new game; only words on end of round
                        if (player_count >= MAX_PLAYERS) {
                            finish_game(&player_count, player_cards, pos_player_hand, house_cards, &pos_house_hand, blackjack);
                            no_money(bankrupt, money, bet);
                            first_round(&index, &pos_house_hand, house_cards, pos_player_hand, deck, player_cards, decks, bankrupt);
                        	take_bets(money, bet, bankrupt);
                        	new_game(player_points, player_cards, pos_player_hand, &house_points, house_cards, pos_house_hand, blackjack);
                        }
                        break;

                    case SDLK_q:
                    	//quit game when q is pressed
                    	stats_txt(money, money_player, status_count);
                        quit = 1;
                    default:
                        break;
                }
            }
        }

        // render game table
        RenderTable(money, serif, imgs, renderer, player_count);
        // render player points
        render_points(renderer, serif, player_points);
        // render house points
        render_house_points(renderer, serif, house_points);
        // render house cards
        RenderHouseCards(house_cards, pos_house_hand, cards, renderer);
        // render player cards
        RenderPlayerCards(player_cards, pos_player_hand, cards, renderer);
        // render blackjack
    	render_blackjack(renderer, serif, blackjack);
    	// render bust
    	render_bust(renderer, serif, player_points);
    	render_status(renderer, serif, player_count, current_status, house_points, blackjack);
    	render_instructions(renderer, serif);
        // render in the screen all changes above
        SDL_RenderPresent(renderer);
    	// add a delay
		SDL_Delay( delay );
    }

    // free memory allocated for images and textures and close everything including fonts
    UnLoadCards(cards);
    TTF_CloseFont(serif);
    SDL_FreeSurface(imgs[0]);
    SDL_FreeSurface(imgs[1]);
    SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return EXIT_SUCCESS;
}


//Read parameters: number of deck, initial amount of money, bet value
int read_param(int *money, int *decks, int *bet)
{
    int money_player = 0;
    char buffer[LEN] = {0};
    int check = 0;

    //Read number of decks
    do {
    	printf("Number of decks to use(2-6): ");
    	fgets(buffer, LEN, stdin);
    	check = sscanf(buffer, "%d", decks);
    	if (*decks>6 || *decks<2 || check != 1) {
        	puts("Error: Invalid number of decks!");
    	}
	}	while (*decks>6 || *decks<2 || check != 1);

	//Read amount of money
	do {
		printf("Initial amount of money per player[>=10]: ");
		fgets(buffer, LEN, stdin);
		check = sscanf(buffer, "%d", &money_player);
	    if (money_player<10 || check != 1){
	        puts("Error: Invalid amount of money!");
	    }
	} while (money_player<10 || check != 1);

	//Read bet value
	do {
	    printf("Bet value[<=%d]: ", (int) (0.2*money_player));
	    fgets(buffer, LEN, stdin);
	    check = sscanf(buffer, "%d", bet);
	    if (*bet<1 || *bet>0.2 * money_player || check != 1) {
	        puts("Error: Invalid bet value");
	    }
	} while (*bet<1 || *bet > 0.2*money_player || check != 1);

	//Assign money to each player
    for (int i=0; i<MAX_PLAYERS; ++i)
        money[i] = money_player;

    return money_player;
}


//creates new deck with sorted cards
void init_deck(int deck[], int decks)
{
    for (int i=0; i < decks; ++i) {
        for (int j=0; j < 52; ++j) {
            deck[j + i*MAX_DECK_SIZE] = j;
        }
    }
}

//shuffles the array using Fisher-Yates algorithm
void shuffle(int *deck, int decks)
{
    int n;
    
    for (int i = decks * MAX_DECK_SIZE - 1; i>0; --i) {
        n = rand() % i;
        swap(&deck[i], &deck[n]);
    }
}

//Swaps the value of two variables
void swap(int *a, int *b)
{
    int aux;
    aux = *a;
    *a = *b;
    *b = aux;
}


// Assigns two cards for each player and the house at the beginning of each game
void first_round(
	int *index, 
	int *pos_house_hand, 
	int house_cards[], 
	int pos_player_hand[], 
	int deck[], 
	int hand[][MAX_CARD_HAND], 
	int decks, 
	int bankrupt[]
)
{   
    for (int i=0; i<2; ++i) {
        for (int j=0; j<MAX_PLAYERS; ++j) {
        	reshuffle(deck, decks, index);
			//check for bankrupcy
			if (!bankrupt[j]){
            	hand[j][i] = deck[*index];
            	pos_player_hand[j] = i+1;
            	(*index)++;
            }
            else
            	pos_player_hand[j] = 0;
            
        }
        //check for end of deck
        if (*index == decks * MAX_DECK_SIZE -1) {
    		*index = 0;
    		init_deck(deck, decks);
			shuffle(deck, decks);
		}
        house_cards[i] = deck[*index]; 
        (*index)++;
    }
    *pos_house_hand = 1;
}


//counts the house or the player points
int count_points(int arr[], int pos)
{
    int points = 0;
    int num_ace = 0;
    for (int i=0; i<pos; ++i) {
        if (arr[i]%13 == 12)
            ++num_ace; //counts the number of aces
        points += point_index((arr[i])); //adds the cards points
    }
   	//change ace value to 1 if points>21
    while (num_ace != 0 && points > 21) {
        points -= 10;
        --num_ace;
    }

    return points;
} 


//Converts the value of a card to its corresponding value
int point_index(int val)
{
    int point;
    val %= 13;
    if (val == 12)
        point = 11;
    else if (val <12 && val>8)
        point = 10;
    else
        point = val + 2;

    return point;
}


//takes a card from the array
int give_card(int deck[], int hand[], int *pos, int *index, int decks)
{
	reshuffle(deck, decks, index);
	hand[*pos] = deck[*index];
	++(*index);
	++(*pos);

	return deck[*index-1];
}

//Gives a card to a player
void hit(
	int player_count, 
	int *index, 
	int pos_player_hand[],
    int player_cards[][MAX_CARD_HAND], 
    int deck[], 
    int bankrupt[],
    int decks
)
{
	if (!bankrupt[player_count]){
		give_card(deck, player_cards[player_count], &(pos_player_hand[player_count]), index, decks);
	}
}


//Assigns cards to house after every player made stand
void hit_house(
	int *index, 
	int *pos_house_hand, 
	int house_cards[], 
	int deck[], 
	int *house_points, 
	int decks,
	int blackjack[]
)
{
	int num_ace = 0;

	*pos_house_hand = 2; //turns the face down card, up
	//house blackjack
	if (*house_points == 21);
		blackjack[5] = 1;
	//Gives the house cards until it has 17 or more points
    while ((*house_points = count_points(house_cards, *pos_house_hand)) < 17) {
    	give_card(deck, house_cards, pos_house_hand, index, decks);
    }

    // count the number of aces
    for (int i=0; i<*pos_house_hand; ++i){
        	if (house_cards[i]%13 == 12)
        		++num_ace;
    }

    // soft hand: give a house a extra card if ace + 6 points
    if (num_ace >= 1 && *house_points == 17) {
    	give_card(deck, house_cards, pos_house_hand, index, decks);
    	*house_points = count_points(house_cards, *pos_house_hand);
    }
}


//sets some variables to 0 so they can be used again in new game
void finish_game(
	int *player_count, 
	int player_cards[][MAX_CARD_HAND], 
	int pos_player_hand[], 
	int house_cards[], 
	int *pos_house_hand, 
	int blackjack[]
)
{
    if (*player_count >= MAX_PLAYERS) {
        for (int j=0; j < MAX_PLAYERS; ++j) {
            for (int i=0; i < pos_player_hand[j]; ++i) {
                player_cards[j][i] = 0;
            }
            pos_player_hand[MAX_PLAYERS] = 0;
            blackjack[j] = 0;
        }
        for (int i=0; i < *pos_house_hand; ++i)
            house_cards[i] = 0;
        *pos_house_hand = 0;
        *player_count = 0;
    }
}


//counts the points in the beginning of every game
void new_game(
	int player_points[], 
	int player_cards[][MAX_CARD_HAND], 
	int pos_player_hand[], 
	int *house_points, 
	int house_cards[], 
	int pos_house_hand, 
	int blackjack[]
)
{
    for (int i=0; i < MAX_PLAYERS; ++i) {
        player_points[i] = count_points(player_cards[i], pos_player_hand[i]);
        *house_points = count_points(house_cards, pos_house_hand);
        if (player_points[i] == 21) // checks for blackjack
            blackjack[i] = 1;
    }
}


//renders bust text when a players goes over 21 points
void render_bust(SDL_Renderer *_renderer, TTF_Font *_font, int player_points[])
{
    SDL_Rect rect;
    char bust[] = "BUST";
    for (int i = 0; i<MAX_PLAYERS; ++i) {
    	rect.x = 80 + 208*i;
    	rect.y = 380;
    	rect.w = 70;
    	rect.h = 30;
    	if (player_points[i] > 21) {
    		SDL_SetRenderDrawColor(_renderer, 255, 0, 0, 255 );
    		SDL_RenderFillRect(_renderer, &rect);
    		SDL_SetRenderDrawColor(_renderer, 255, 0, 0, 255 );
    		SDL_RenderDrawRect(_renderer, &rect);
    		SDL_Color white = { 255, 255, 255}; // red
    		RenderText(94+(208*i), 381, bust, _font, &white, _renderer);
    	}
    }
}


// renders blackjack text when player gets 21 points in the first two cards
void render_blackjack(SDL_Renderer *_renderer, TTF_Font *_font, int blackjack[])
{
    
    SDL_Rect rect;
    char blackjack_txt[] = "BLACKJACK";
    for (int i=0; i<MAX_PLAYERS; ++i) {
    	rect.x = 55 + 208*i;
    	rect.y = 380;
    	rect.w = 115;
    	rect.h = 30;
    	if (blackjack[i]) {
    		SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255 );
    		SDL_RenderFillRect(_renderer, &rect);
    		SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255 );
    		SDL_RenderDrawRect(_renderer, &rect);
    		SDL_Color white = { 255, 255, 255}; // red
    		RenderText(64+(208*i), 381, blackjack_txt, _font, &white, _renderer);
   		}
   	}
}


//renders the text with the player points
void render_points(SDL_Renderer *_renderer, TTF_Font *_font, int player_points[])
{
    SDL_Rect rect;
    char points[10];
    rect.x = 30;
    rect.y = 255;
    rect.w = 105;
    rect.h = 20;
    for (int i=0; i<MAX_PLAYERS; ++i) {
    	SDL_Color white = { 255, 255, 255};
    	sprintf(points,"%d points", player_points[i]);
    	RenderText(rect.x+208*i, rect.y, points, _font, &white, _renderer);
    }

}


//render the text with the house points
void render_house_points(SDL_Renderer *_renderer, TTF_Font *_font, int house_points)
 {
 	SDL_Rect rect;
 	char points[10];
 	rect.x = 20;
 	rect.y = 130;
 	SDL_Color white = {255, 255, 255};
 	sprintf(points, "House: %d points", house_points);
 	RenderText(rect.x, rect.y, points, _font, &white, _renderer);
 }


// takes the value of the bet from every player in the beginning of a game
void take_bets(int money[], int bet, int bankrupt[])
{
	for (int i=0; i<MAX_PLAYERS; i++) {
		if (!bankrupt[i])
			money[i] -= bet;
	}
}

//pays the players if they win, returns bets if they draw
void pay_bets(
	int money[],  
	int blackjack[], 
	int current_status[], 
	int bet
)
{
    for (int i=0; i<MAX_PLAYERS; ++i) {
    	if (current_status[i] == 1)
    		money[i] += 2*bet;
    	else if (current_status[i] == 2)
    		money[i] += bet;

    	if (blackjack[i])
    		money[i] += bet/2;
    }

}


// flag to check if a player as enough money to play
void no_money(int bankrupt[], int money[], int bet)
{
	for (int i=0; i<MAX_PLAYERS; i++) {
		if (money[i] < bet)
			bankrupt[i] = 1;
		else
			bankrupt[i] = 0;
	}
}


//count wins, draws and defeats
void status_func(
	int player_points[], 
	int house_points, 
	int current_status[],
	int status_count[][STATS], 
	int bankrupt[],
	int blackjack[]
)
{ 
	/* win = 1
	 * draw = 2
	 * defeat = 3
	 * bankrupt = 4
	 */

	for (int i=0; i<MAX_PLAYERS; ++i) {
		if (bankrupt[i])
			current_status[i] = 4;
		else if ( player_points[i] > 21 ) //player bust
			current_status[i] = 3;
		else if ( house_points > 21 )	// house bust 
			current_status[i] = 1;
		else if ( player_points[i] < house_points )	//house wins with more points
			current_status[i] = 3;
		else if (player_points[i] > house_points) //player wins with more points
			current_status[i] = 1;
		else if ((player_points[i] == house_points) && !blackjack[i]) //draw
			current_status[i] = 2;
		else if (blackjack[i] && !blackjack[4]) //player wins with blackjack
			current_status[i] = 1;
		else if (blackjack[4] && blackjack[i]) // house and player have blackjack: draw
			current_status[i] = 2;
		else if (blackjack[4] && !blackjack[i]) //house wind with blackjack
			current_status[i] = 3;
	}

	//sums the number of wins, draws and defeats of each players
	for (int i=0; i<MAX_PLAYERS; i++) {
		if (current_status[i] < 4)
			status_count[i][current_status[i] - 1]++;
	}
}


//writes the stats.txt file
void stats_txt(int money[], int money_player, int status_count[][STATS])
{
	// count the amount of money the house lost
	int house_money = 0;
	for (int i=0; i<MAX_PLAYERS; ++i) {
		house_money += money_player - money[i];
	}
	
	// create the stats file
	FILE *stats;
	stats = fopen("stats.txt", "w");

	//write the number of wins, draws, defeats per player
	fprintf(stats, "Player\t\tWins\tDraws\tDefeats\t\tMoney\n");
	for (int i=0; i<MAX_PLAYERS; ++i) {
		fprintf(stats, "%s\t%d\t%d\t%d\t\t%d\n", playerNames[i], status_count[i][0], status_count[i][1], status_count[i][2], money[i]);
	}

	// 
	int i;
	char *mode[2] = {"lost", "won"}; 
	if (house_money<0)
		i = 0;
	else if (house_money>=0)
		i = 1;

	if (house_money < 0)
		house_money *= -1;

	// write the house money
	fprintf(stats, "\nThe house %s %d euros.", mode[i], house_money);	

	// close file
	fclose(stats);
}


// reshuffle deck when there's no more cards
void reshuffle(int deck[], int decks, int *index)
{
	if (*index == decks * MAX_DECK_SIZE -1) { //check for end of deck
    	*index = 0;
    	init_deck(deck, decks);
		shuffle(deck, decks);
	}
}

// render if a player won, lost or drawn in the side panel
void render_status(
	SDL_Renderer *_renderer, 
	TTF_Font *_font, 
	int player_count, 
	int current_status[], 
	int house_points, 
	int blackjack[]
)
{
    char *status_txt[] = {"WON", "DRAWN", "LOST", "    "};
    int string_index = 0;
    SDL_Color black = { 0, 0, 0}; // black
    for (int i = 0; i<MAX_PLAYERS; ++i) {
    	if (player_count >= MAX_PLAYERS) {
        	if (current_status[i] == 1)
        		string_index = 0;
        	else if (current_status[i] == 2)
        		string_index = 1;
        	else if (current_status[i] == 3)
        		string_index = 2;
        	else if (current_status[i] == 4)
        		string_index = 3;

        	RenderText(930, 370+30*i, status_txt[string_index], _font, &black, _renderer);
    	}
    	RenderText(865, 370+30*i, playerNames[i], _font, &black, _renderer);
        
    }
}

// render game instructions
void render_instructions(SDL_Renderer *_renderer, TTF_Font *_font)
{
	SDL_Color black = { 0, 0, 0};
	TTF_SetFontStyle(_font, TTF_STYLE_BOLD);
	RenderText(865, 230, "Instructions" , _font, &black, _renderer);
	TTF_SetFontStyle(_font, TTF_STYLE_NORMAL);
	RenderText(865, 250, "<h> - hit" , _font, &black, _renderer);
	RenderText(865, 270, "<s> - stand" , _font, &black, _renderer);
	RenderText(865, 290, "<n> - new game" , _font, &black, _renderer);
	RenderText(865, 310, "<q> - quit game" , _font, &black, _renderer);
}


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
void RenderTable(int _money[], TTF_Font *_font, SDL_Surface *_img[], SDL_Renderer* _renderer, int player_count)
{
    SDL_Color black = { 0, 0, 0 }; // black
    SDL_Color white = { 255, 255, 255 }; // white
    char name_money_str[STRING_SIZE];
    SDL_Texture *table_texture;
    SDL_Rect tableSrc, tableDest, playerRect;
    int separatorPos = (int)(0.95f*WIDTH_WINDOW); // seperates the left from the right part of the window
    int height;
   
    // set color of renderer to some color
    SDL_SetRenderDrawColor( _renderer, 255, 255, 255, 255 );
    
    // clear the window
    SDL_RenderClear( _renderer );

    tableDest.x = tableSrc.x = 0;
    tableDest.y = tableSrc.y = 0;
    tableSrc.w = _img[0]->w;
    tableSrc.h = _img[0]->h;

    tableDest.w = separatorPos;
    tableDest.h = HEIGHT_WINDOW;

    table_texture = SDL_CreateTextureFromSurface(_renderer, _img[0]);
    SDL_RenderCopy(_renderer, table_texture, &tableSrc, &tableDest);
   
    // render the IST Logo
    height = RenderLogo(separatorPos, 0, _img[1], _renderer);
    
    // render the student name
    height += RenderText(separatorPos+3*MARGIN, height, myName, _font, &black, _renderer);
    
    // this renders the student number
    RenderText(separatorPos+3*MARGIN, height, myNumber, _font, &black, _renderer);
    
    
    // renders the areas for each player: names and money too !
    for ( int i = 0; i < MAX_PLAYERS; i++)
    {
    	// renders the squares + name for each player and a red square for the
    	// player currently playing
    	if (i == player_count)
    		SDL_SetRenderDrawColor(_renderer, 255, 0, 0, 255 );
    	else 
    		SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255 );
        playerRect.x = i*(separatorPos/4-5)+10;
        playerRect.y = (int) (0.55f*HEIGHT_WINDOW);
        playerRect.w = separatorPos/4-5;
        playerRect.h = (int) (0.42f*HEIGHT_WINDOW);
        sprintf(name_money_str,"%s -- %d euros", playerNames[i], _money[i]);
        RenderText(playerRect.x+20, playerRect.y-50, name_money_str, _font, &white, _renderer);
        SDL_RenderDrawRect(_renderer, &playerRect);
    }
    
    // destroy everything
    SDL_DestroyTexture(table_texture);
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
