#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "Tools.h"

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

int stringComparer(char **psLeft, char **psRight)
{
	return strcmp(*psLeft, *psRight);
}

int btLoad(FILE *file, char ***pasWords, size_t *piAllocatedCount, char *sLine)
{
	char *sCursor = sLine;
	size_t iUsedCount = 0;

	while (!feof(file))
	{
		if (fgets(sCursor, INT_MAX, file) == 0) // буфер уже подходящего размера, всё посчитано заранеее
			break;

		size_t len = strlen(sCursor);
		while (sCursor[len - 1] == '\n' || sCursor[len - 1] == '\r')
		{
			sCursor[--len] = 0;
		}

		if (iUsedCount == *piAllocatedCount)
		{
			size_t newAllocatedCount = max(2, *piAllocatedCount * 8 / 5);
			void *newPPWords = realloc(*pasWords, newAllocatedCount * sizeof(char*));
			if (!newPPWords)
			{
				return EXIT_FAILURE;
			}

			*piAllocatedCount = newAllocatedCount;
			*pasWords = newPPWords;
		}

		(*pasWords)[iUsedCount] = sCursor;
		iUsedCount++;
		sCursor += len + 1;
	}
	
	realloc(*pasWords, iUsedCount * sizeof(char*));
	*piAllocatedCount = iUsedCount;

	shellSort(*pasWords, iUsedCount, sizeof(char*), stringComparer);

	return 0;
}

void btLoop(char **asWords, size_t iCount)
{
	size_t len = 0;
	char *sLine = calloc(1, sizeof(char));
	for (;;)
	{
		ssize_t lastIndex = getline(&sLine, &len, stdin) - 1;
		if (sLine[lastIndex] == '\n')
			sLine[lastIndex] = '\0';

		if (strcmp(sLine, "exit") == 0)
			break;

		ssize_t index = binarySearchMore(asWords, iCount, sizeof(char*), &sLine, stringComparer);
		if (index == -1 && strcmp(asWords[iCount - 1], sLine) == 0)
		{
			index == iCount;
		}

		index--;

		if (index >= 0 && strcmp(asWords[index], sLine) == 0)
			fprintf(stdout, "YES\n");
		else
			fprintf(stdout, "NO\n");
	}

	free(sLine);
}

int btMain(const FILE *file, size_t iFileSize)
{
	char *pLine = calloc((size_t)iFileSize, sizeof(char));
	char **asWords = NULL;	
	size_t iCount = 0;

	int result = btLoad(file, &asWords, &iCount, pLine);
	if (result)
		return result;

	fclose(file);

	btLoop(asWords, iCount);

	free(asWords);
	free(pLine);
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

	fpos_t iFileSize;
	fseek(file, 0, SEEK_END);
	fgetpos(file, &iFileSize);
	fseek(file, 0, SEEK_SET);

	return btMain(file, iFileSize);

	return 0;
}