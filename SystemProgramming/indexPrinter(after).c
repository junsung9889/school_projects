//-----------------------------------------------------------
//
// SWE2024: System Programming Lab (2019 Fall)
//
// Skeleton code for PA #1
// September 24, 2019
//
// Hyungmin Cho
// Computing Platforms Lab
// Sungkyunkwan University
//
// Original Authors:
// Embedded Software Laboratory
// Sungkyunkwan University
//
// Student Name  : Choi Se Hyun
// Student ID No.: 2016310487
//------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <string.h>

typedef enum {false, true} bool;

typedef struct word {
    int wordNumber;
    int memAlloc;
    int *index1;
    int *index2;
    int *pos;
} word;

typedef struct Trie{
    bool is_terminal;
    word *data;
    struct Trie* children[28];
} Trie;

typedef struct dataStorage {
    int bibleNumber;
    char **bibleName;
    Trie *dataRoot;
} dataStorage;

FILE *fd_printer = NULL;
dataStorage d_printer;

Trie* createTrie();
Trie* TrieInsertEmpty(Trie *t, const char * key);
void freeAllTrie(Trie *t, int bibleNumber);

void FilePrintAllTrie(Trie *t, char* buf, int cnt)
{
    if(t->is_terminal)
    {
        buf[cnt] = '\0';
        int sum = 0;
        for(int i = 0; i < d_printer.bibleNumber; i++)
        {
            sum += ((t->data[i].wordNumber > 0 ) ? t->data[i].wordNumber : 0);
        }
        fprintf(fd_printer, "%s : %d : ", buf, sum);
        for(int i = 0; i < d_printer.bibleNumber; i++)
        {
            for(int j = 0; j < t->data[i].wordNumber; j++)
            {
                fprintf(fd_printer, " %s:%d:%d:%d,", d_printer.bibleName[i], t->data[i].index1[j], t->data[i].index2[j], t->data[i].pos[j]);
            }
        }
        fprintf(fd_printer, "\n");
    }
    for(int i = 0; i < 28; i++)
    {
        if(t->children[i])
        {
            if(i >= 2 && i <= 27) buf[cnt] = i - 2 + 'a';
            else if(i == 0) buf[cnt] = '-';
            else if(i == 1) buf[cnt] = 39;
            FilePrintAllTrie(t->children[i], buf, cnt+1);
        }
    }
}

void indexPrinter(char* outputFileNm) {
    char trieBuf[32] = {0};
    char fileReadBuf[32] = {0};
    int fileReadInt = 0;
    
    d_printer.dataRoot = createTrie();
    
    fd_printer = fopen("dataStorage", "rb");
    if(fd_printer != NULL)
    {
        fread(&d_printer.bibleNumber, sizeof(int), 1, fd_printer);
        d_printer.bibleName = malloc(sizeof(char*) * d_printer.bibleNumber);
        for(int i = 0; i < d_printer.bibleNumber; i++)
            d_printer.bibleName[i] = malloc(sizeof(char) * 32);
        for(int i = 0; i < d_printer.bibleNumber; i++)
            fread(d_printer.bibleName[i], sizeof(char)*32, 1, fd_printer);
        Trie *t;
        while(1)
        {
            fread(fileReadBuf, sizeof(char)*32, 1, fd_printer);
            if(feof(fd_printer))
                break;
            t = TrieInsertEmpty(d_printer.dataRoot, fileReadBuf);
            t->data = malloc(sizeof(word) * d_printer.bibleNumber);
            t->data[d_printer.bibleNumber-1].wordNumber = -1;
            for(int i = 0; i < d_printer.bibleNumber; i++)
            {
                fread(&fileReadInt, sizeof(int), 1, fd_printer);
                t->data[i].wordNumber = fileReadInt;
                if(fileReadInt > 0)
                {
                    t->data[i].index1 = malloc(sizeof(int) * fileReadInt);
                    t->data[i].index2 = malloc(sizeof(int) * fileReadInt);
                    t->data[i].pos = malloc(sizeof(int) * fileReadInt);
                    fread(t->data[i].index1, sizeof(int), fileReadInt, fd_printer);
                    fread(t->data[i].index2, sizeof(int), fileReadInt, fd_printer);
                    fread(t->data[i].pos, sizeof(int), fileReadInt, fd_printer);
                }
            }
        }
    }
    else
    {
        return;
    }
    
    fd_printer = fopen(outputFileNm, "w");
    FilePrintAllTrie(d_printer.dataRoot, trieBuf, 0);
    fclose(fd_printer);
    
    for(int i = 0; i < d_printer.bibleNumber; i++)
        free(d_printer.bibleName[i]);
    free(d_printer.bibleName);
    freeAllTrie(d_printer.dataRoot, d_printer.bibleNumber);
}
