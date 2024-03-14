#ifndef __HCOMPRESS_H
#define __HCOMPRESS_H

#include "linkedList.h"
#include <stdio.h>

typedef struct tnode {
  int fr;
  int c;
  struct tnode* left;
  struct tnode* right;
  struct tnode* parent;
} tnode;

int getCodeLength(tnode* leafNodes);
unsigned short getCode(tnode* leafNodes);
void freeTree(tnode* node);
tnode* createFreqTable(char* leafNodes);
tnode* createHuffmanTree(tnode* leafNodes);
int treeHeight(tnode* TreeRoot);
void printPreorder(tnode* node, int level);
void printCode(tnode* node);
void decodeFile(char* filename, tnode* treeRoot);
void encodeFile(char* filename, tnode* leafNodes);

#endif
