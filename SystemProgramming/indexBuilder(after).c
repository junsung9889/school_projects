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

int bibleNumber = 0;
FILE *fd = NULL;
dataStorage d;

char indexChar(char c)
{
    if(c >= 'A' && c <= 'Z')
        return c - 63;
    else if(c >= 'a' && c <= 'z')
        return c - 95;
    else if(c == '-')
        return 0;
    else if(c == 39)
        return 1;
    else
        return 0;
}

Trie *createTrie()
{
    Trie *t = malloc(sizeof(Trie));
    memset(t->children, 0, sizeof(t->children));
    t->is_terminal = false;
    t->data = NULL;
    return t;
}

void TrieInsertData(Trie *t, const char * key, int index1, int index2, int pos)
{
    if(*key == '\0')
    {
        t->is_terminal = true;
        if(t->data == NULL)
        {
            t->data = malloc(sizeof(word) * bibleNumber);
            for(int i = 0; i < bibleNumber-1; i++)
                t->data[i].wordNumber = 0;
            t->data[bibleNumber-1].wordNumber = 0;
            t->data[bibleNumber-1].index1 = malloc(sizeof(int) * 100);
            t->data[bibleNumber-1].index2 = malloc(sizeof(int) * 100);
            t->data[bibleNumber-1].pos = malloc(sizeof(int) * 100);
            t->data[bibleNumber-1].memAlloc = 1;
        }
        else if(t->data[bibleNumber-1].wordNumber == -1)
        {
            t->data[bibleNumber-1].wordNumber = 0;
            t->data[bibleNumber-1].index1 = malloc(sizeof(int) * 100);
            t->data[bibleNumber-1].index2 = malloc(sizeof(int) * 100);
            t->data[bibleNumber-1].pos = malloc(sizeof(int) * 100);
            t->data[bibleNumber-1].memAlloc = 1;
        }
        if(t->data[bibleNumber-1].wordNumber >= 100 * t->data[bibleNumber-1].memAlloc)
        {
            t->data[bibleNumber-1].memAlloc++;
            t->data[bibleNumber-1].index1 = realloc(t->data[bibleNumber-1].index1, sizeof(int) * (100 * t->data[bibleNumber-1].memAlloc));
            t->data[bibleNumber-1].index2 = realloc(t->data[bibleNumber-1].index2, sizeof(int) * (100 * t->data[bibleNumber-1].memAlloc));
            t->data[bibleNumber-1].pos = realloc(t->data[bibleNumber-1].pos, sizeof(int) * (100 * t->data[bibleNumber-1].memAlloc));
        }
        t->data[bibleNumber-1].index1[t->data[bibleNumber-1].wordNumber] = index1;
        t->data[bibleNumber-1].index2[t->data[bibleNumber-1].wordNumber] = index2;
        t->data[bibleNumber-1].pos[t->data[bibleNumber-1].wordNumber] = pos;
        t->data[bibleNumber-1].wordNumber++;
    }
    else
    {
        int index = indexChar(*key);
        if(t->children[index] == 0)
            t->children[index] = createTrie();
        TrieInsertData(t->children[index], key + 1, index1, index2, pos);
    }
}

Trie* TrieInsertEmpty(Trie *t, const char * key)
{
    if(*key == 0)
    {
        t->is_terminal = true;
        return t;
    }
    
    int index = indexChar(*key);
    if(t->children[index] == 0)
    {
        t->children[index] = createTrie();
    }
    
    return TrieInsertEmpty(t->children[index], key+1);
}

void fileWriteAllTrie(Trie *t, char* buf, int cnt)
{
    if(t->is_terminal)
    {
        buf[cnt] = '\0';
        fwrite(buf, sizeof(char)*32, 1, fd);
        
        for(int i = 0; i < bibleNumber; i++)
        {
            fwrite(&t->data[i].wordNumber, sizeof(int), 1, fd);
            if(t->data[i].wordNumber > 0)
            {
                fwrite(t->data[i].index1, sizeof(int), t->data[i].wordNumber, fd);
                fwrite(t->data[i].index2, sizeof(int), t->data[i].wordNumber, fd);
                fwrite(t->data[i].pos, sizeof(int), t->data[i].wordNumber, fd);
            }
        }
    }
    for(int i = 0; i < 28; i++)
    {
        if(t->children[i])
        {
            if(i >= 2 && i <= 27) buf[cnt] = i - 2 + 'a';
            else if(i == 0) buf[cnt] = '-';
            else if(i == 1) buf[cnt] = 39;
            fileWriteAllTrie(t->children[i], buf, cnt+1);
        }
    }
}

void freeAllTrie(Trie *t, int bibleNumber)
{

    for(int i = 0; i < 28; i+=7)
    {
        if(t->children[i])
            freeAllTrie(t->children[i], bibleNumber);
        if(t->children[i+1])
            freeAllTrie(t->children[i+1], bibleNumber);
        if(t->children[i+2])
            freeAllTrie(t->children[i+2], bibleNumber);
        if(t->children[i+3])
            freeAllTrie(t->children[i+3], bibleNumber);
        if(t->children[i+4])
            freeAllTrie(t->children[i+4], bibleNumber);
        if(t->children[i+5])
            freeAllTrie(t->children[i+5], bibleNumber);
        if(t->children[i+6])
            freeAllTrie(t->children[i+6], bibleNumber);
    }
    if(t->is_terminal)
    {
        for(int i = 0; i < bibleNumber; i++)
        {
            if(t->data[i].wordNumber != -1 && t->data[i].wordNumber != 0)
            {
                free(t->data[i].index1);
                free(t->data[i].index2);
                free(t->data[i].pos);
            }
        }
        free(t->data);
    }
}

void indexBuilder(char* inputFileNm) {
    int inputCnt = 0;
    int curIndex1 = 0, curIndex2 = 0;
    
    char dummyFileChar[32] = "EndOfFile";
    int dummyFileInt = -1;
    
    char trieBuf[32] = {0};
    char fileReadBuf[32] = {0};
    int fileReadInt = 0;
    char bibleName[32] = {0};
    
    char *buf = NULL;
    char *savedBuf;
    char *savedFirstCur = NULL;
    char *cur;
    
    ssize_t line_size;
    size_t buf_size;
    
    d.dataRoot = createTrie();
    
    strcpy(bibleName, inputFileNm);
    bibleName[strlen(bibleName)-4] = '\0';
    
    fd = fopen("dataStorage", "rb");
    if(fd != NULL)
    {
        fread(&bibleNumber, sizeof(int), 1, fd);
        d.bibleNumber = ++bibleNumber;
        d.bibleName = malloc(sizeof(char*) * bibleNumber);
        for(int i = 0; i < d.bibleNumber; i++)
        {
            d.bibleName[i] = malloc(sizeof(char) * 32);
            memset(d.bibleName[i], 0, sizeof(char) * 32);
        }
        for(int i = 0; i < d.bibleNumber-1; i++)
            fread(d.bibleName[i], sizeof(char)*32, 1, fd);
        for(int i = 0; i < d.bibleNumber-1; i++)
        {
            if(!strcmp(bibleName, d.bibleName[i]))
            {
                fclose(fd);
                free(d.dataRoot);
                for(int j = 0; j < d.bibleNumber; j++)
                    free(d.bibleName[j]);
                free(d.bibleName);
                return;
            }
        }
        Trie *t;
        while(1)
        {
            fread(fileReadBuf, sizeof(char)*32, 1, fd);
            if(feof(fd))
                break;
            t = TrieInsertEmpty(d.dataRoot, fileReadBuf);
            t->data = malloc(sizeof(word) * bibleNumber);
            t->data[bibleNumber-1].wordNumber = -1;
            for(int i = 0; i < bibleNumber-1; i++)
            {
                fread(&fileReadInt, sizeof(int), 1, fd);
                t->data[i].wordNumber = fileReadInt;
                if(fileReadInt > 0)
                {
                    t->data[i].index1 = malloc(sizeof(int) * fileReadInt);
                    t->data[i].index2 = malloc(sizeof(int) * fileReadInt);
                    t->data[i].pos = malloc(sizeof(int) * fileReadInt);
                    fread(t->data[i].index1, sizeof(int), fileReadInt, fd);
                    fread(t->data[i].index2, sizeof(int), fileReadInt, fd);
                    fread(t->data[i].pos, sizeof(int), fileReadInt, fd);
                }
            }
        }
        fclose(fd);
    }
    else
    {
        bibleNumber = 1;
        d.bibleNumber = 1;
        d.bibleName = malloc(sizeof(char*));
        d.bibleName[0] = malloc(sizeof(char) * 32);
    }
    strcpy(d.bibleName[bibleNumber-1], bibleName);
    
    fd = fopen(inputFileNm, "r");
    if(fd == NULL)
    {
        for(int i = 0; i < d.bibleNumber; i++)
            free(d.bibleName[i]);
        free(d.bibleName);
        freeAllTrie(d.dataRoot, bibleNumber);
        return;
    }
    line_size = getdelim(&buf, &buf_size, '1', fd);
    fseek(fd, -1, SEEK_CUR);
    while((line_size = getline(&buf, &buf_size, fd)) != -1)
    {
        inputCnt = 0;
        savedBuf = buf;
        savedFirstCur = NULL;
        while((cur = strtok(buf, " ,.:?!;()\n")) && strlen(cur) != 0)
        {
            buf = NULL;
            if(inputCnt == 0) curIndex1 = atoi(cur);
            else if(inputCnt == 1) curIndex2 = atoi(cur);
            else
            {
                if(!savedFirstCur)
                    savedFirstCur = cur;
                TrieInsertData(d.dataRoot, cur, curIndex1, curIndex2, (int)(cur-savedFirstCur));
            }
            inputCnt++;
        }
        buf = savedBuf;
    }
    fclose(fd);
    
    fd = fopen("dataStorage", "wb");
    fwrite(&d.bibleNumber, sizeof(int), 1, fd);
    for(int i = 0; i < d.bibleNumber; i++)
        fwrite(d.bibleName[i], sizeof(char)*32, 1, fd);
    fileWriteAllTrie(d.dataRoot, trieBuf, 0);
    fclose(fd);
    
    for(int i = 0; i < d.bibleNumber; i++)
        free(d.bibleName[i]);
    free(d.bibleName);
    freeAllTrie(d.dataRoot, d.bibleNumber);
}

