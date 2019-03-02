#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <memory.h>
#include <ctype.h>

#define MAX_FILE_LINES 110000
#define MAX_SUPPORTED_ANAGARMS 20

char *concatAnagrams(char **anagrams, char *separator) {
    char *commaSeparatedAnagrams = calloc(MAX_SUPPORTED_ANAGARMS, sizeof(char **));

    int x = 0;
    while(anagrams[x]) {
        strcat(strcat(commaSeparatedAnagrams, separator), anagrams[x]);
        x++;
    }

    return commaSeparatedAnagrams;
}

char *toLowerWord(char *string) {
    size_t i, s = strlen(string);
    for (i = 0; i < s; i++)
        string[i] = (char) tolower(string[i]);
    return string;
}

int compareChars(const void *a, const void *b) {
    return *(char *)a - *(char *)b;
}

char **findAnagrams(char *word, char **wordList) {
    size_t wordLen = strlen(word);
    char **anagrams = (char **) calloc(MAX_SUPPORTED_ANAGARMS, sizeof(char*));
    char *inputWord = calloc(wordLen, sizeof(char *));
    char *findedWord = calloc(wordLen, sizeof(char *));

    strcpy(inputWord, toLowerWord(word));
    qsort(inputWord, wordLen, sizeof(char), compareChars);

    int x = 0;
    int y = 0;
    while (wordList[x]) {
        size_t xWordLen = strlen(wordList[x]);
        if(wordLen == xWordLen) {
            strcpy(findedWord, toLowerWord(wordList[x]));
            qsort(findedWord, xWordLen, sizeof(findedWord[0]), compareChars);

            if(strcmp(inputWord, findedWord) == 0 && strcmp(word, wordList[x]) != 0) {
                anagrams[y] = wordList[x];
                y++;
            };
        }
        x++;
    }

    return anagrams;
}

char *removeLineEnd(char *line) {
    size_t strlength = strlen(line);
    if( line[strlength-1] == '\n' )
        line[strlength-1] = 0;
    strlength = strlen(line);
    if( line[strlength-1] == '\r' )
        line[strlength-1] = 0;

    return line;
}

char **readWordsFromFile(char *filename) {
    char **wordList = (char **) calloc(MAX_FILE_LINES, sizeof(char *));
    FILE *fp;
    char *line = NULL;
    size_t len = 0;

    fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("file %s not found", filename);
        exit(EXIT_FAILURE);
    }

    int x = 0;
    while ((getline(&line, &len, fp)) != -1) {
        char *processedLine = removeLineEnd(line);

        wordList[x] = (char *) calloc(strlen(processedLine), sizeof(char *));
        strcpy(wordList[x], processedLine);
        x++;
    }

    fclose(fp);

    return wordList;
}

int main(int argc, char *argv[]) {
    struct timeval stop, start;
    gettimeofday(&start, NULL);
    char *anagramsFile = argv[1];
    char *word = argv[2];

    if(argc != 3) {
        printf("2 arguments are required, arg1 path to file, arg2 is string");
        exit(EXIT_FAILURE);
    }

    char **wordList = readWordsFromFile(anagramsFile);
    char **anagrams = findAnagrams(word, wordList);

    free(wordList);

    gettimeofday(&stop, NULL);
    long runTime = stop.tv_usec - start.tv_usec;
    printf("%ld%s\n", runTime, concatAnagrams(anagrams, ","));
    return EXIT_SUCCESS;
}
