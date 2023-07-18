#ifndef S21_DECIMAL_H
#define S21_DECIMAL_H

#include <math.h>
#include <stdio.h>

typedef struct {
  unsigned int bits[4];
} s21_decimal;

void s21_ZeroDecimal(s21_decimal *dst);
int s21_getBit(s21_decimal num, int place);
int s21_getSign(s21_decimal value);
void s21_setBit(s21_decimal *value, int index, int bit);
void s21_setSign(s21_decimal *value, int sign);
void s21_setScale(s21_decimal *num, int exp);
int s21_getScale(s21_decimal dst);
void s21_setScaleToZero(s21_decimal *number);
int s21_from_int_to_decimal(int src, s21_decimal *dst);
int s21_from_decimal_to_int(s21_decimal src, int *dst);
int s21_from_decimal_to_float(s21_decimal src, float *dst);
#endif  // S21_DECIMAL_H