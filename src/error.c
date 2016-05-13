void *ecalloc(size_t nmemb, size_t size)
{
	void *p = calloc(nmemb, size);

	if (p == NULL) {
		puts("Erro: Impossível alocar memória.");
		exit(EXIT_FAILURE);
	}

	return p;
}