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

FILE *fd = NULL;
dataStorage d;

void indexChar(char *c)
{
    if(*c >= 'A' && *c <= 'Z')
        *c -= 63;
    else if(*c >= 'a' && *c <= 'z')
        *c -= 95;
    else if(*c == '-')
        *c = 0;
    else if(*c == 39)
        *c = 1;
    else
        *c = 0;
}

Trie *createTrie()
{
    Trie *t = malloc(sizeof(Trie));
    memset(t->children, 0, sizeof(t->children));
    t->is_terminal = false;
    t->data = NULL;
    return t;
}

int getBibleNumber(dataStorage *d)
{
    return d->bibleNumber;
}

int *getBibleNumberRef(dataStorage *d)
{
    return &d->bibleNumber;
}

int getCurrentBibleNumber(dataStorage *d)
{
    return getBibleNumber(d)-1;
}

void setBibleNumber(dataStorage *d, int n)
{
    d->bibleNumber = n;
}

Trie* getDataRoot(dataStorage *d)
{
    return d->dataRoot;
}

void setDataRoot(dataStorage *d, Trie *t)
{
    d->dataRoot = t;
}

char** getBibleName(dataStorage *d)
{
    return d->bibleName;
}

char* getBibleNameIndex(dataStorage *d, int i)
{
    return d->bibleName[i];
}

void setBibleName(dataStorage *d, char **c)
{
    d->bibleName = c;
}

void setBibleNameIndex(dataStorage *d, char *c, int i)
{
    d->bibleName[i] = c;
}

int isEndOfWord(Trie *t)
{
    if(t->is_terminal)
        return 1;
    return 0;
}

void setEndOfWord(Trie *t)
{
    t->is_terminal = 1;
}

word *getData(Trie *t)
{
    return t->data;
}

void setData(Trie *t, word *data)
{
    t->data = data;
}

word getDataByIndex(Trie *t, int i)
{
    return t->data[i];
}

word getCurrentData(dataStorage *d, Trie *t)
{
    return t->data[getCurrentBibleNumber(d)];
}

void setDataWordNumber(Trie *t, int i, int wordNumber)
{
    t->data[i].wordNumber = wordNumber;
}

void setCurrentDataWordNumber(dataStorage *d, Trie *t, int wordNumber)
{
    t->data[getCurrentBibleNumber(d)].wordNumber = wordNumber;
}

int getDataWordNumber(Trie *t, int i)
{
    return t->data[i].wordNumber;
}

int getCurrentDataWordNumber(dataStorage *d, Trie *t)
{
    return t->data[getCurrentBibleNumber(d)].wordNumber;
}

void setDataIndex1(Trie *t, int i, int *index1)
{
    t->data[i].index1 = index1;
}

void setCurrentDataIndex1(dataStorage *d, Trie *t, int *index1)
{
    t->data[getCurrentBibleNumber(d)].index1 = index1;
}

int* getDataIndex1(Trie *t, int i)
{
    return t->data[i].index1;
}

int* getCurrentDataIndex1(dataStorage *d, Trie *t)
{
    return t->data[getCurrentBibleNumber(d)].index1;
}

void setDataIndex2(Trie *t, int i, int *index2)
{
    t->data[i].index2 = index2;
}

void setCurrentDataIndex2(dataStorage *d, Trie *t, int *index2)
{
    t->data[getCurrentBibleNumber(d)].index2 = index2;
}

int *getDataIndex2(Trie *t, int i)
{
    return t->data[i].index2;
}

int *getCurrentDataIndex2(dataStorage *d, Trie *t)
{
    return t->data[getCurrentBibleNumber(d)].index2;
}

void setDataPos(Trie *t, int i, int *pos)
{
    t->data[i].pos = pos;
}

void setCurrentDataPos(dataStorage *d, Trie *t, int *pos)
{
    t->data[getCurrentBibleNumber(d)].pos = pos;
}

int *getDataPos(Trie *t, int i)
{
    return t->data[i].pos;
}

int *getCurrentDataPos(dataStorage *d, Trie *t)
{
    return t->data[getCurrentBibleNumber(d)].pos;
}

void setDataMemAlloc(Trie *t, int i, int memAlloc)
{
    t->data[i].memAlloc = memAlloc;
}

void setCurrentDataMemAlloc(dataStorage *d, Trie *t, int memAlloc)
{
    t->data[getCurrentBibleNumber(d)].memAlloc = memAlloc;
}

int getDataMemAlloc(Trie *t, int i)
{
    return t->data[i].memAlloc;
}

int getCurrentDataMemAlloc(dataStorage *d, Trie *t)
{
    return t->data[getCurrentBibleNumber(d)].memAlloc;
}

void insertDataToCurrent(dataStorage *d, Trie *t, int index1, int index2, int pos)
{
    getCurrentDataIndex1(d, t)[getCurrentDataWordNumber(d, t)] = index1;
    getCurrentDataIndex2(d, t)[getCurrentDataWordNumber(d, t)] = index2;
    getCurrentDataPos(d, t)[getCurrentDataWordNumber(d, t)] = pos;
    setCurrentDataWordNumber(d, t, getCurrentDataWordNumber(d, t)+1);
}

void TrieInsertData(Trie *t, char *key, int index1, int index2, int pos)
{
    if(*key == '\0')
    {
        setEndOfWord(t);
        if(getData(t) == NULL)
        {
            setData(t, malloc(sizeof(word) * getBibleNumber(&d)));
            for(int i = 0; i < getCurrentBibleNumber(&d); i++)
                setDataWordNumber(t, i, 0);
            setCurrentDataWordNumber(&d, t, 0);
            setCurrentDataIndex1(&d, t, malloc(sizeof(int) * 100));
            setCurrentDataIndex2(&d, t, malloc(sizeof(int) * 100));
            setCurrentDataPos(&d, t, malloc(sizeof(int) * 100));
            setCurrentDataMemAlloc(&d, t, 1);
        }
        
        else if(getCurrentDataWordNumber(&d, t) == -1)
        {
            setCurrentDataWordNumber(&d, t, 0);
            setCurrentDataIndex1(&d, t, malloc(sizeof(int) * 100));
            setCurrentDataIndex2(&d, t, malloc(sizeof(int) * 100));
            setCurrentDataPos(&d, t, malloc(sizeof(int) * 100));
            setCurrentDataMemAlloc(&d, t, 1);
        }
        
        if(getCurrentDataWordNumber(&d, t) >= 100 * getCurrentDataMemAlloc(&d, t))
        {
            setCurrentDataMemAlloc(&d, t, getCurrentDataMemAlloc(&d, t)+1);
            setCurrentDataIndex1(&d, t, realloc(getCurrentDataIndex1(&d, t), sizeof(int) * (100 * getCurrentDataMemAlloc(&d, t))));
            setCurrentDataIndex2(&d, t, realloc(getCurrentDataIndex2(&d, t), sizeof(int) * (100 * getCurrentDataMemAlloc(&d, t))));
            setCurrentDataPos(&d, t, realloc(getCurrentDataPos(&d, t), sizeof(int) * (100 * getCurrentDataMemAlloc(&d, t))));
        }
        
        insertDataToCurrent(&d, t, index1, index2, pos);
    }
    else
    {
        indexChar(key);
        if(t->children[*key] == 0)
            t->children[*key] = createTrie();
        TrieInsertData(t->children[*key], key + 1, index1, index2, pos);
    }
}

Trie* TrieInsertEmpty(Trie *t, char *key)
{
    if(*key == 0)
    {
        setEndOfWord(t);
        return t;
    }
    
    indexChar(key);
    if(t->children[*key] == 0)
    {
        t->children[*key] = createTrie();
    }
    
    return TrieInsertEmpty(t->children[*key], key+1);
}

void fileWriteAllTrie(Trie *t, char* buf, int cnt)
{
    if(isEndOfWord(t))
    {
        buf[cnt] = '\0';
        fwrite(buf, sizeof(char)*32, 1, fd);
        
        for(int i = 0; i < getBibleNumber(&d); i++)
        {
            fwrite(&t->data[i].wordNumber, sizeof(int), 1, fd);
            if(getDataWordNumber(t, i) > 0)
            {
                fwrite(getDataIndex1(t, i), sizeof(int), getDataWordNumber(t, i), fd);
                fwrite(getDataIndex2(t, i), sizeof(int), getDataWordNumber(t, i), fd);
                fwrite(getDataPos(t, i), sizeof(int), getDataWordNumber(t, i), fd);
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
    for(int i = 0; i < 28; i+=4)
    {
        if(t->children[i])
            freeAllTrie(t->children[i], bibleNumber);
        if(t->children[i+1])
            freeAllTrie(t->children[i+1], bibleNumber);
        if(t->children[i+2])
            freeAllTrie(t->children[i+2], bibleNumber);
        if(t->children[i+3])
            freeAllTrie(t->children[i+3], bibleNumber);
    }
    if(isEndOfWord(t))
    {
        for(int i = 0; i < bibleNumber; i++)
        {
            if(getDataWordNumber(t, i) != -1 && getDataWordNumber(t, i) != 0)
            {
                if(getDataIndex1(t, i)) free(getDataIndex1(t, i));
                if(getDataIndex2(t, i)) free(getDataIndex2(t, i));
                if(getDataPos(t, i)) free(getDataPos(t, i));
            }
        }
        free(getData(t));
    }
}

void indexBuilder(char* inputFileNm) {
    int inputCnt = 0;
    int curIndex1 = 0, curIndex2 = 0;
    
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
    
    setDataRoot(&d, createTrie());
    
    strcpy(bibleName, inputFileNm);
    bibleName[strlen(bibleName)-4] = '\0';
    
    fd = fopen("dataStorage", "rb");
    if(fd != NULL)
    {
        fread(getBibleNumberRef(&d), sizeof(int), 1, fd);
        setBibleNumber(&d, getBibleNumber(&d)+1);
        setBibleName(&d, malloc(sizeof(char*) * getBibleNumber(&d)));
        for(int i = 0; i < getBibleNumber(&d); i++)
        {
            setBibleNameIndex(&d, malloc(sizeof(char) * 32), i);
            memset(getBibleNameIndex(&d, i), 0, sizeof(char) * 32);
        }
        for(int i = 0; i < getCurrentBibleNumber(&d); i++)
            fread(getBibleNameIndex(&d, i), sizeof(char)*32, 1, fd);
        for(int i = 0; i < getCurrentBibleNumber(&d); i++)
        {
            if(!strcmp(bibleName, getBibleNameIndex(&d, i)))
            {
                fclose(fd);
                free(getDataRoot(&d));
                for(int j = 0; j < getBibleNumber(&d); j++)
                    free(getBibleNameIndex(&d, j));
                free(getBibleName(&d));
                return;
            }
        }
        Trie *t;
        while(1)
        {
            fread(fileReadBuf, sizeof(char)*32, 1, fd);
            if(feof(fd))
                break;
            t = TrieInsertEmpty(getDataRoot(&d), fileReadBuf);
            setData(t, malloc(sizeof(word) * getBibleNumber(&d)));
            setCurrentDataWordNumber(&d, t, -1);
            for(int i = 0; i < getCurrentBibleNumber(&d); i++)
            {
                fread(&fileReadInt, sizeof(int), 1, fd);
                setDataWordNumber(t, i, fileReadInt);
                if(fileReadInt > 0)
                {
                    setDataIndex1(t, i, malloc(sizeof(int) * fileReadInt));
                    setDataIndex2(t, i, malloc(sizeof(int) * fileReadInt));
                    setDataPos(t, i, malloc(sizeof(int) * fileReadInt));
                    fread(getDataIndex1(t, i), sizeof(int), fileReadInt, fd);
                    fread(getDataIndex2(t, i), sizeof(int), fileReadInt, fd);
                    fread(getDataPos(t, i), sizeof(int), fileReadInt, fd);
                }
            }
        }
        fclose(fd);
    }
    else
    {
        setBibleNumber(&d, 1);
        setBibleName(&d, malloc(sizeof(char*)));
        setBibleNameIndex(&d, malloc(sizeof(char) * 32), 0);
    }
    strcpy(getBibleNameIndex(&d, getCurrentBibleNumber(&d)), bibleName);
    
    fd = fopen(inputFileNm, "r");
    if(fd == NULL)
    {
        for(int i = 0; i < getBibleNumber(&d); i++)
            free(getBibleNameIndex(&d, i));
        free(getBibleName(&d));
        freeAllTrie(getDataRoot(&d), getBibleNumber(&d));
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
                TrieInsertData(getDataRoot(&d), cur, curIndex1, curIndex2, (int)(cur-savedFirstCur));
            }
            inputCnt++;
        }
        buf = savedBuf;
    }
    fclose(fd);
    
    fd = fopen("dataStorage", "wb");
    fwrite(getBibleNumberRef(&d), sizeof(int), 1, fd);
    for(int i = 0; i < getBibleNumber(&d); i++)
        fwrite(getBibleNameIndex(&d, i), sizeof(char)*32, 1, fd);
    fileWriteAllTrie(getDataRoot(&d), trieBuf, 0);
    fclose(fd);
    
    for(int i = 0; i < getBibleNumber(&d); i++)
        free(getBibleNameIndex(&d, i));
    free(getBibleName(&d));
    freeAllTrie(getDataRoot(&d), getBibleNumber(&d));
}

