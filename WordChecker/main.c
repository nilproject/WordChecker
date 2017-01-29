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

ssize_t readLine(char *lineptr, FILE *stream)
{
	if (lineptr == NULL)
		return -1;

	if (stream == NULL)
		return -1;
	
	int c;

	c = fgetc(stream);
	if (c == EOF)
		return -1;

	char *p = lineptr;
	while (c != EOF)
	{
		*p++ = c;

		c = fgetc(stream);
		if (c == '\n')
			break;
	}

	*p++ = '\0';
	return (p - lineptr) / sizeof(char) - 1;
}

void load(FILE *file, HashSet *set, char *line)
{
	char *cLine = line;

	while (!feof(file))
	{
		ssize_t len = readLine(cLine, file);

		if (!hashSet_insert(set, cLine, false))
		{
			printf("\nout-of-memory");
			return EXIT_FAILURE;
		}

		cLine += len + 1;
	}

	fclose(file);
}

void loop(HashSet *set)
{
	size_t len = 0;
	char *tline = calloc(1, sizeof(char));
	for (;;)
	{
		ssize_t lastIndex = getline(&tline, &len, stdin) - 1;
		if (tline[lastIndex] == '\n')
			tline[lastIndex] = '\0';

		if (strcmp(tline, "exit") == 0)
			break;

		if (hashSet_contains(set, tline))
			fprintf(stdout, "YES\n");
		else
			fprintf(stdout, "NO\n");
	}

	free(tline);
}

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

	fpos_t fileSize;
	fseek(file, 0, SEEK_END);
	fgetpos(file, &fileSize);
	fseek(file, 0, SEEK_SET);

	char *line = calloc(fileSize, sizeof(char));
	HashSet *set = hashSet_create();
	
	load(file, set, line);

	loop(set);

	hashSet_free(set, false);
	free(line);

	return 0;
}
