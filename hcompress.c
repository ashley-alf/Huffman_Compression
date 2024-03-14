#include "linkedList.h"
#include "hcompress.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/*
Author: Ashley Alfaro

Date: October 28, 2022

Purpose of Program: Encode and Decode a file using
bit manipulation, pointers, structures, malloc,
trees and linked lists.

*/


int main(int argc, char *argv[]) {
  //  Check the make sure the input parameters are correct
  if (argc != 3) {
    printf("Error: The correct format is \"hcompress -e filename\" or \"hcompress -d filename.huf\"\n"); fflush(stdout);
    exit(1);
  }
  //Create the frequency table by reading the generic file
  tnode* leafNodes = createFreqTable("decind.txt");
  //  Create the huffman tree from the frequency table
  tnode* treeRoot = createHuffmanTree(leafNodes);

  if (strcmp(argv[1], "-e") == 0) {
    // Pass the leafNodes since it will process bottom up
    encodeFile(argv[2], leafNodes);
    printf("Successfully encoded file \n");

  }
  //
  else { // decode
    // Pass the tree root since it will process top down
    decodeFile(argv[2], treeRoot);
    printf("Successfully decoded file \n");

  }
  //free the leaf nodes
  free(leafNodes);
  //free the parent nodes
  freeTree(treeRoot);
  return 0;
}
//HELPER METHODS
//method to get the code of a leaf node
unsigned short getCode(tnode* n){
  unsigned short temp = 0;
  tnode* node = n;
  int length = 0;
  //Compute encoding and bitcount
  while(node->parent != NULL){
    tnode* parent = node->parent;
    if(parent->left == node){
      length++;
    }//if statement for parent left
    //add 1
    else if(parent->right == node){
      temp |= (1 << length);
      length++;

    }//if statement for parent right
    node = node->parent;
  }
  return temp;

}
//method to get the length from a leaf node to the root
int getCodeLength(tnode* n){
  tnode* node = n;
  int length = 0;
  //Compute encoding and bitcount
  while(node->parent != NULL){
    tnode* parent = node->parent;
    if(parent->left == node){
      length++;
    }//if statement for parent left
    //add 1
    else if(parent->right == node){
      length++;

    }//if statement for parent right
    node = node->parent;
  }
  return length;

}
//Method that frees the parent nodes of the tree
void freeTree(tnode* node){
  if(node->left == NULL && node->right == NULL){
    return;
  }
  freeTree(node->left);
  freeTree(node->right);
  free(node);

}
//method that prints the tree in pre-order
void printPreorder(tnode* node, int level){
  if (node == NULL)
  return;
  for(int i=0; i < level; i++)
  {
    printf("  ");
  }
  printf("%d: %d\n", node->c, node->fr);
  printPreorder(node->left, level+1);
  printPreorder(node->right, level+1);
}
//method that returns the height of the tree
int treeHeight(tnode* root){
  if(root == NULL)
  return -1;
  int h1 = treeHeight(root->left);
  int h2 = treeHeight(root->right);
  //find the max of both nodes and follow it
  if(h1 > h2){
    return 1+h1;
  }
  return 1+h2;
}
//method that gives you the code of a character in the tree (not reversed though)
void printCode(tnode* node){
  if(node->parent == NULL)
  return;
  if(node->parent->left == node){
    printf("0");
  }
  else{
    printf("1");
  }
  printCode(node->parent);
}

//REQUIRED METHODS
tnode* createFreqTable(char* file){
  //make an array of nodes that holds all ASCCI
  tnode* table = (tnode*)malloc(128*sizeof(tnode));
  //make a file to open it
  FILE* f;
  f = fopen(file, "r");
  if (NULL == f) {
    printf("file can't be opened \n");
  }
  //instantiate all the frequencies of the table to 0
  for(int i = 0; i<128; i++){
    table[i].fr = 0;
  }
  //get the first character of the file
  char ch = fgetc(f);

  //traverse through the entire file
  while(ch != EOF){
    //the character will be instantiated with it's respective decimal value
    int index = ch;
    //instantiate the character to it's decimal value
    table[index].c = index;
    //increase the frequency each time we find the same character
    table[index].fr++;
    //move onto the next character
    ch = fgetc(f);
  }

  //instantiate the ending character ASCCI code 4
  table[4].c = 4;
  table[4].fr = 1;
  return table;
}
tnode* createHuffmanTree(tnode* leafNodes){
  //Create the list
  LinkedList* listy = llCreate();
  //traverse the list of all ASCCI values
  for(int i = 0; i<128; i++) {
    //only add it to listy if that character was found in the file
    if(leafNodes[i].fr != 0){
      //add it in order
      add_in_order(&listy, &leafNodes[i]);
    }
  }
  //now that we have our list traverse it to create the tree
  while(listy->next != NULL){
    //get the first smallest value and the second one
    //remove them from the list
    tnode* first = removeFirst(&listy);
    tnode* second = removeFirst(&listy);
    //get their sum
    int sum = first->fr + second->fr;
    //create their parent node
    tnode* parent = (tnode*) malloc (sizeof(tnode));
    //Connect all nodes together to form a subtree
    //the frequency of the parent is their sum
    parent->fr = sum;
    parent->left = first;
    parent->right = second;
    first -> parent = parent;
    second -> parent = parent;
    //add the parent back into the list
    add_in_order(&listy, parent);
  }
  tnode* root = removeFirst(&listy);
  //free theList once you have the tree
  llFree(listy);
  return root;
}
void encodeFile(char* filename, tnode* leafNodes){
  //Make the file name ->decind.txt.huf
  FILE* readFile; //decind.txt
  FILE* writeFile; //decind.txt.huf

  char file[strlen(filename)+5]; //add 5 so we can count the .huf + \0
  const char* extension = ".huf";
  strcpy(file, filename);
  strcpy(file+strlen(filename), extension);
  printf("%s\n", file);

  //open/create the files
  readFile = fopen(filename, "r");
  writeFile = fopen(file,"w");


  //variable that keeps track of how many bits we've read
  //if the file does not exist or we
  if(readFile == NULL) {
    printf("File can't be opened \n");
    return;
  }
  //variable that keeps track of how many bits the character has
  int bitCount =0;
  //variable that will be written to the file
  unsigned char toWrite = 0;
  //How many bits we have left to write on the byte to send
  int bitsWritten = 7;
  //temp variable that goes through each characters
  unsigned short temp = 0;
  // get the first character of the file
  char ch = fgetc(readFile);

  //read the entire file
  while(ch != EOF){
    int index = ch;
    //get the code of that character from leaf to root and reverse it
    temp = getCode(&leafNodes[index]);
    //get the length of that character
    bitCount = getCodeLength(&leafNodes[index]);

    //variable that keeps track of the shifting
    int shift = bitCount -1;
    //go through the entire code of the character
    for(int i=0; i < bitCount ;i++){
      //get the value of each bits using bit manipulation
      short bitToSet = (temp >> shift);
      char bit = bitToSet & 1;
      toWrite |= (bit << bitsWritten);
      //decrease the shift so we can look at the next bit
      shift--;
      //decrease bitsWritten so we know how much space we can add to the byte
      bitsWritten--;
      //once we have enough bits (8 = 1 byte) send it to file
      if(bitsWritten == -1){
        //Write toWrite to file
        fwrite(&toWrite,sizeof(toWrite),1, writeFile);
        //reset everything
        bitsWritten = 7;
        toWrite = 0;
      }//if bitsWritten
    }//for loop
    //get the next character
    ch = fgetc(readFile);
  }

  //add the encoding for the terminating char to toWrite
  temp = getCode(&leafNodes[4]);
  bitCount = getCodeLength(&leafNodes[4]);
  int shift = bitCount -1;
  for(int i=0; i < bitCount;i++){
    //bit manipulation
    short bitToSet = (temp >> shift);
    char bit = bitToSet & 1;
    toWrite |= (bit << bitsWritten);
    shift--;
    bitsWritten--;

    if(bitsWritten == -1)
    {
      //write to file if we don't have enough space
      fwrite(&toWrite,sizeof(toWrite),1, writeFile);
      bitsWritten = 7;
      toWrite = 0;
    }
    if(i == (bitCount - 1)){
      //write to file if we are done reading from the terminating character
      fwrite(&toWrite,sizeof(toWrite),1, writeFile);
    }
  }
  //terminating character
  fclose(writeFile);
  fclose(readFile);
}//encodeFile
void decodeFile(char* filename, tnode* treeRoot){

  //name file
  FILE* readFile; //decind.txt.huf
  FILE* writeFile; //decind.txt.huf.dec
  char file[strlen(filename)+5];
  const char* extension = ".dec";
  strcpy(file, filename);
  strcpy(file+strlen(filename), extension);
  printf("%s\n", file);
  //open/create the files
  readFile = fopen(filename, "r");
  writeFile = fopen(file,"w");

  //variable to keep track of the shifting
  int sLeft = 7;
  //start at the treeRoot
  tnode* temp = treeRoot;
  //get the first byte
  char bitcode = fgetc(readFile);
  //variable that declares when to end the loop
  int flag = 1;

  while(flag){
    sLeft = 7;
    //go through the entire byte
    for(int i = 0; i< 8; i++){
      //bit manipulation to get the bits in order
      unsigned char mask = (1 << sLeft);
      unsigned char bit = bitcode & mask;
      sLeft--;
      //traverse the tree until we find the leaf node
      if(bit == 0){
        temp = temp->left;
      }
      else{
        temp = temp->right;
      }
      //we have found a leaf node
      if(temp->left == NULL && temp->right == NULL){
        //if it is not the terminating character write it to file
        if(temp->c != 4){
          fwrite(&temp->c,sizeof(temp->c),1, writeFile);
          temp = treeRoot;
        }
        else{
          //we have found the terminating character therefore exit the while
          //loop and the for loop
          flag = 0;
          i = 8;

        }
      }
    }//for loop
    //only get the next character if the flag is not true
    //else we might get a segmentation fault.
    if(flag != 0){
      bitcode = fgetc(readFile);
    }
  }// while flag
  //close files
  fclose(writeFile);
  fclose(readFile);

}//decodeFile
