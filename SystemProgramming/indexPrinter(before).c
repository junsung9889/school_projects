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

char indexChar(char c);
Trie* createTrie();
Trie* TrieInsertEmpty(Trie *t, const char * key);
void freeAllTrie(Trie *t, int bibleNumber);
int getBibleNumber(dataStorage *d);
int getCurrentBibleNumber(dataStorage *d);
int *getBibleNumberRef(dataStorage *d);
void setBibleNumber(dataStorage *d, int n);
Trie* getDataRoot(dataStorage *d);
void setDataRoot(dataStorage *d, Trie *t);
char** getBibleName(dataStorage *d);
char* getBibleNameIndex(dataStorage *d, int i);
void setBibleName(dataStorage *d, char **c);
void setBibleNameIndex(dataStorage *d, char *c, int i);
int isEndOfWord(Trie *t);
void setEndOfWord(Trie *t);
word *getData(Trie *t);
void setData(Trie *t, word *data);
word getDataByIndex(Trie *t, int i);
word getCurrentData(dataStorage *d, Trie *t);
void setDataWordNumber(Trie *t, int i, int wordNumber);
void setCurrentDataWordNumber(dataStorage *d, Trie *t, int wordNumber);
int getDataWordNumber(Trie *t, int i);
int getCurrentDataWordNumber(dataStorage *d, Trie *t);
void setDataIndex1(Trie *t, int i, int *index1);
void setCurrentDataIndex1(dataStorage *d, Trie *t, int *index1);
int* getDataIndex1(Trie *t, int i);
int* getCurrentDataIndex1(dataStorage *d, Trie *t);
void setDataIndex2(Trie *t, int i, int *index2);
void setCurrentDataIndex2(dataStorage *d, Trie *t, int *index2);
int *getDataIndex2(Trie *t, int i);
int *getCurrentDataIndex2(dataStorage *d, Trie *t);
void setDataPos(Trie *t, int i, int *pos);
void setCurrentDataPos(dataStorage *d, Trie *t, int *pos);
int *getDataPos(Trie *t, int i);
int *getCurrentDataPos(dataStorage *d, Trie *t);
void setDataMemAlloc(Trie *t, int i, int memAlloc);
void setCurrentDataMemAlloc(dataStorage *d, Trie *t, int memAlloc);
int getDataMemAlloc(Trie *t, int i);
int getCurrentDataMemAlloc(dataStorage *d, Trie *t);
void insertDataToCurrent(dataStorage *d, Trie *t, int index1, int index2, int pos);

void FilePrintAllTrie(Trie *t, char* buf, int cnt)
{
    if(isEndOfWord(t))
    {
        buf[cnt] = '\0';
        int sum = 0;
        for(int i = 0; i < getBibleNumber(&d_printer); i++)
        {
            sum += ((getDataWordNumber(t, i) > 0 ) ? getDataWordNumber(t, i) : 0);
        }
        fprintf(fd_printer, "%s : %d : ", buf, sum);
        for(int i = 0; i < getBibleNumber(&d_printer); i++)
        {
            for(int j = 0; j < getDataWordNumber(t, i); j++)
            {
                fprintf(fd_printer, " %s:%d:%d:%d,", getBibleNameIndex(&d_printer, i), getDataIndex1(t, i)[j], getDataIndex2(t,i)[j], getDataPos(t,i)[j]);
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
    
    setDataRoot(&d_printer, createTrie());
    
    fd_printer = fopen("dataStorage", "rb");
    if(fd_printer != NULL)
    {
        fread(getBibleNumberRef(&d_printer), sizeof(int), 1, fd_printer);
        setBibleName(&d_printer, malloc(sizeof(char*) * getBibleNumber(&d_printer)));
        for(int i = 0; i < getBibleNumber(&d_printer); i++)
            setBibleNameIndex(&d_printer, malloc(sizeof(char) * 32), i);
        for(int i = 0; i < getBibleNumber(&d_printer); i++)
            fread(getBibleNameIndex(&d_printer, i), sizeof(char)*32, 1, fd_printer);
        Trie *t;
        while(1)
        {
            fread(fileReadBuf, sizeof(char)*32, 1, fd_printer);
            if(feof(fd_printer))
                break;
            t = TrieInsertEmpty(d_printer.dataRoot, fileReadBuf);
            setData(t, malloc(sizeof(word) * getBibleNumber(&d_printer)));
            setCurrentDataWordNumber(&d_printer, t, -1);
            for(int i = 0; i < getBibleNumber(&d_printer); i++)
            {
                fread(&fileReadInt, sizeof(int), 1, fd_printer);
                setDataWordNumber(t, i, fileReadInt);
                if(fileReadInt > 0)
                {
                    setDataIndex1(t, i, malloc(sizeof(int) * fileReadInt));
                    setDataIndex2(t, i, malloc(sizeof(int) * fileReadInt));
                    setDataPos(t, i, malloc(sizeof(int) * fileReadInt));
                    fread(getDataIndex1(t,i), sizeof(int), fileReadInt, fd_printer);
                    fread(getDataIndex2(t,i), sizeof(int), fileReadInt, fd_printer);
                    fread(getDataPos(t,i), sizeof(int), fileReadInt, fd_printer);
                }
            }
        }
    }
    else
    {
        return;
    }
    
    fd_printer = fopen(outputFileNm, "w");
    FilePrintAllTrie(getDataRoot(&d_printer), trieBuf, 0);
    fclose(fd_printer);
    
    for(int i = 0; i < getBibleNumber(&d_printer); i++)
        free(getBibleNameIndex(&d_printer, i));
    free(getBibleName(&d_printer));
    freeAllTrie(getDataRoot(&d_printer), getBibleNumber(&d_printer));
}

