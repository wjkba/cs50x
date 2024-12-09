#include <cs50.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

int getLength(long number);

int main(void) {
  long input;
  int length;

  do{
      input = get_long("Number: ");
      length = getLength(input);
      if (length != 13 && length != 15 && length != 16) {
          printf("INVALID\n");
          return 0;
      }
  } while (length != 13 && length != 15 && length != 16);

  //input = 4003600000000014;
  length = getLength(input);

  int sumaCyfrRazyDwa = 0;
  int sumaReszty = 0;
  for (int i = 1; i < length; i+=2) {
    int cyfra = (input / (long)pow(10, i)) % 10;
    if(cyfra*2 < 10) sumaCyfrRazyDwa += cyfra*2;
    else{
      sumaCyfrRazyDwa += (cyfra*2)/10;
      sumaCyfrRazyDwa += (cyfra*2)%10;
    }
  }

  for (int i = 0; i < length; i+=2){
    int cyfra = (input / (long)pow(10, i)) % 10;
    sumaReszty += cyfra;
  }

  int sumaObu = sumaCyfrRazyDwa + sumaReszty;

  // printf("%i\n", sumaCyfrRazyDwa);
  // printf("%i\n", sumaReszty);
  // printf("%i\n", sumaObu);

  int pierwsza = (input / (long)pow(10, length-1)) % 10;
  int pierwszeDwie = (input / (long)pow(10, length - 2)) % 100;

  if (sumaObu % 10 == 0) {
    if (pierwsza == 4 && (length == 13 || length == 16)) {
        printf("VISA\n");
    } else if (pierwszeDwie >= 51 && pierwszeDwie <= 55 && length == 16) {
        printf("MASTERCARD\n");
    } else if ((pierwszeDwie == 34 || pierwszeDwie == 37) && length == 15) {
        printf("AMEX\n");
    } else {
        printf("INVALID\n");
    }
  } else printf("INVALID\n");
}

int getLength(long number) {
  int length = 0;
  while (number > 0) {
    number = number / 10;
    length++;
  }
  return length;
}
