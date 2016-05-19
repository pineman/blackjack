#include <stdlib.h>
#include <stdio.h>

void *ecalloc(size_t nmemb, size_t size)
{
	void *memory = calloc(nmemb, size);

	if (memory == NULL) {
		fprintf(stderr, "Erro: Impossível alocar memória.\n");
		exit(EXIT_FAILURE);
	}

	return memory;
}

FILE *efopen(const char *path, const char *mode)
{
	FILE *file = fopen(path, mode);

	if (file == NULL) {
		fprintf(stderr, "Erro: Impossível abrir ficheiro %s.\n", path);
		exit(EXIT_FAILURE);
	}

	return file;
}
