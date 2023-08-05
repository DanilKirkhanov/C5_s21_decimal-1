#ifndef S21_DECIMAL_H
#define S21_DECIMAL_H

#include <math.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>

s21_decimal zero = {0, 0};

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
int s21_from_float_to_decimal(float src, s21_decimal *dst);
int s21_GetBitToFloat(double number, int bit_number);
void s21_scaleEqul(s21_decimal *value_1, s21_decimal *value_2);
int s21_equalWithoutSign(s21_decimal value_1, s21_decimal value_2);

int s21_is_equal(s21_decimal dec_1, s21_decimal dec_2); // не готов :(
int s21_is_less(s21_decimal dec_1, s21_decimal dec_2); // не готов :(
int s21_is_greater_or_equal(s21_decimal dec_1, s21_decimal dec_2);
int s21_is_not_equal(s21_decimal dec_1, s21_decimal dec_2);
int s21_is_greater(s21_decimal dec_1, s21_decimal dec_2);
int s21_is_less_or_equal(s21_decimal dec_1, s21_decimal dec_2);

#endif  // S21_DECIMAL_H