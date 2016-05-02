#include <stdio.h>
#include <stdlib.h>

#define	MAX	64

typedef struct config {
	int num_decks;
	int num_players;
	char **type_players;
	char *player_names[MAX];
	int *money;
	int *bets;
} config;
	
config *read_config(char *filename)
{
	config *parameters = NULL;
	parameters = (config *) malloc(sizeof(config));

	FILE *fp = NULL;
	fp = fopen(filename, "r");
	
	if (fp == NULL) {
		puts("Erro: Impossivel abrir ficheiro");
		exit(EXIT_FAILURE);
	}

	char buffer[MAX];

	fgets(buffer, MAX, fp);
	sscanf(buffer, "%d-%d", &(parameters->num_decks), &(parameters->num_players));

	// Quero fixed size para parameter->type_player[i] de 3*sizeof(char *)
	// Como distinguir mini strings
	for (int i=0; fgets(buffer, MAX, fp) != NULL; i++) {
		parameters->type_players = (char **) realloc(parameters->type_players, (i+1) * sizeof(char **));
		parameters->type_players[i] = (char *) malloc(64 * sizeof(char *));
		sscanf(buffer, "%s", parameters->type_players[i]);

	}
	
	return parameters;
}

int main()
{
	config *parameter = NULL;
	parameter = read_config("parametros.txt");
	printf("%d\n", parameter->num_decks);
	printf("%d\n", parameter->num_players);
	printf("%s\n", parameter->type_players[3]);
	return EXIT_SUCCESS;
}

