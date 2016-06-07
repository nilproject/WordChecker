#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "HashSet.h"

#if _WIN32 || _WIN64

static ssize_t getline(char **lineptr, size_t *n, FILE *stream)
{
	char *bufptr = NULL;
	char *tempBuf = NULL;
	char *p = bufptr;
	ptrdiff_t size;
	int c;
	bool needFree = false;

	if (lineptr == NULL)
		return -1;

	if (stream == NULL)
		return -1;

	if (n == NULL)
		return -1;

	bufptr = *lineptr;
	size = *n;

	c = fgetc(stream);
	if (c == EOF)
		return -1;

	if (bufptr == NULL)
	{
		bufptr = malloc(128);
		if (bufptr == NULL)
			return -1;

		size = 128;
	}

	p = bufptr;
	while (c != EOF)
	{
		if ((p - bufptr) > (size - 1))
		{
			size = size + 128;
			tempBuf = realloc(bufptr, size);
			if (tempBuf == NULL)
			{
				if (needFree)
					free(bufptr);

				return -1;
			}

			p = tempBuf + (p - bufptr);
			bufptr = tempBuf;
			*lineptr = bufptr;
		}

		*p++ = c;
		if (c == '\n')
			break;

		c = fgetc(stream);
	}

	*p++ = '\0';
	*n = size;

	return (ptrdiff_t)(p - bufptr) - 1;
}

#endif

int main(int argc, char **argv)
{
	if (argc < 2)
	{
		printf("Missing file name");
		return EXIT_FAILURE;
	}

	FILE *file = fopen(argv[1], "r");

	if (file == NULL)
	{
		printf("Unable to open file");
		return EXIT_FAILURE;
	}

	HashSet *set = hashSet_create();
	char *line = calloc(1, sizeof(char));
	size_t len = 0;

	while (!feof(file))
	{
		ssize_t lastIndex = getline(&line, &len, file) - 1;

		if (line[lastIndex] == '\n')
			line[lastIndex] = '\0';

		if (!hashSet_insert(set, line, true))
		{
			printf("\nout-of-memory");
			getchar();
			return EXIT_FAILURE;
		}
	}

	fclose(file);

	for (;;)
	{
		ssize_t lastIndex = getline(&line, &len, stdin) - 1;
		if (line[lastIndex] == '\n')
			line[lastIndex] = '\0';

		if (strcmp(line, "exit") == 0)
			break;

		if (hashSet_contains(set, line))
			fprintf(stdout, "YES\n");
		else
			fprintf(stdout, "NO\n");
	}

	hashSet_free(set, true);
	free(line);

	return 0;
}
