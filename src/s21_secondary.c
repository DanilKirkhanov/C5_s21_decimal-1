#include "s21_decimal.h"
void printf_decimal(s21_decimal dec) {
  for (int j = 3; 0 <= j; j--) {
    int src = dec.bits[j];
    if (j == 3)
      printf("scale) ");
    else if (j == 2)
      printf("mantisa) ");
    for (int i = 1; i <= 32; i++) {
      printf("%c", (src & 0x80000000) ? '1' : '0');
      if (j == 3 && (i == 8 || i == 16 || i == 32)) printf(" ");
      // else if (i % 4 == 0 && !(i == 32 && j == 0) && j != 3)
      //   printf(" ");
      src <<= 1;
    }
  }
  printf("\n");
}
int get_bit(s21_decimal src, int index) {
  int bits = src.bits[index / 32];
  int mask = 1;
  mask <<= index % 32;
  return mask & bits ? 1 : 0;
}

void set_bit(s21_decimal* src, int index, int bit) {
  int mask = 1;
  mask <<= (index % 32);
  if (bit) {
    src->bits[index / 32] |= mask;
  } else {
    src->bits[index / 32] &= ~mask;
  }
}

void set_scale(s21_decimal* src, int scale) {
  for (int i = 112; i < 119; i++) {
    set_bit(src, i, scale & 1);
    scale >>= 1;
  }
}

int get_scale(s21_decimal src) {
  src.bits[3] >>= 16;
  return (unsigned char)src.bits[3];
}

void printf_binary(int src) {
  for (int i = 1; i <= 32; i++) {
    printf("%c", (src & 0x80000000) ? '1' : '0');
    if (i % 4 == 0) printf(" ");
    src <<= 1;
  }
  printf("\n");
}

void printf_long_binary(long int src) {
  for (int i = 1; i <= 64; i++) {
    printf("%c", (src & 0x8000000000000000) ? '1' : '0');
    if (i % 4 == 0) printf(" ");
    src <<= 1;
  }
  printf("\n");
}

void div_by_10(s21_decimal* src, int scale) {
  set_scale(src, get_scale(*src) - scale);
  for (; scale != 0; scale--) {
    unsigned long temp = src->bits[2];
    for (int i = 2; i >= 0; i--) {
      src->bits[i] = temp / 10;
      temp = temp % 10;
      temp = (temp << 32) + (unsigned long)src->bits[i - 1];
    }
  }
}
