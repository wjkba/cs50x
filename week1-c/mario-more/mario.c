#include <stdio.h>
#include <cs50.h>
void printBlocksRow(int,int);

int main(void){
  int height = get_int("Height: ");
  while (height < 1 || height > 8){
    height = get_int("Height: ");
  }


  int blocks = 1;
  int space = height-1;
  for(int i = 0; i<height; i++){
    printBlocksRow(blocks,space);
    blocks++;
    space--;
  }
}

void printBlocksRow(int blocks, int space){
  for(int i = 0; i<space; i++){
    printf(" ");
  }
  for(int i = 0; i<blocks; i++){
    printf("#");
  }
  printf("  ");
  for(int i = 0; i<blocks; i++){
    printf("#");
  }
  printf("\n");
}
