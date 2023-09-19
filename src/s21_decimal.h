#ifndef S21_DECIMAL_H
#define S21_DECIMAL_H

#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  unsigned int bits[4];
} s21_decimal;

typedef struct {
  unsigned int bits[6];
} s21_double_decimal;

typedef enum {
  S21_RESULT_OK = 0,
  S21_RESULT_TOOLARGE = 1,
  S21_RESULT_TOOSMALL = 2,
  S21_DIV_ZERO = 3,
  S21_RESULT_ERROR = 4
} s21_code_result;

#define S21_POSITIVE 0
#define S21_NEGATIVE 1

#define BITS_DECIMAL 128
#define INT_BITS 32

int s21_is_equal(s21_decimal dec_1, s21_decimal dec_2);
int s21_is_less(s21_decimal dec_1, s21_decimal dec_2);
int s21_is_greater_or_equal(s21_decimal dec_1, s21_decimal dec_2);
int s21_is_not_equal(s21_decimal dec_1, s21_decimal dec_2);
int s21_is_greater(s21_decimal dec_1, s21_decimal dec_2);
int s21_is_less_or_equal(s21_decimal dec_1, s21_decimal dec_2);

int s21_floor(s21_decimal value, s21_decimal *result);
int s21_round(s21_decimal value, s21_decimal *result);
int s21_truncate(s21_decimal value, s21_decimal *result);
int s21_negate(s21_decimal value, s21_decimal *result);

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

int s21_from_int_to_decimal(int src, s21_decimal *dst);
int s21_from_decimal_to_int(s21_decimal src, int *dst);
int s21_from_decimal_to_float(s21_decimal src, float *dst);
int s21_from_float_to_decimal(float src, s21_decimal *dst);

s21_decimal s21_binary_additional(s21_decimal value1, s21_decimal value2,
                                  s21_code_result *code_result);
s21_decimal s21_binary_sub(s21_decimal value_1, s21_decimal value_2,
                           s21_code_result *code_result);
int s21_help_multiply(s21_decimal value_1, s21_decimal value_2,
                      s21_decimal *result);
int s21_binary_multiplication(s21_decimal value_1, s21_decimal value_2,
                              s21_decimal *result);

void s21_ZeroDecimal(s21_decimal *dst);
int s21_getBit(s21_decimal num, int place);
int s21_getSign(s21_decimal value);
void s21_setBit(s21_decimal *value, int index, int bit);
void s21_setSign(s21_decimal *value, int sign);
void s21_setScale(s21_decimal *num, int exp);
int s21_getScale(s21_decimal dst);
int s21_GetBitToFloat(double number, int bit_number);
int get_bit(s21_decimal src, int index);
void set_bit(s21_decimal *src, int index, int bit);
int get_scale(s21_decimal src);
void div_by_10(s21_decimal *src, int scale);

void s21_add_1_(s21_decimal *result);

s21_decimal s21_binary_division(s21_decimal value_1, s21_decimal value_2,
                                s21_code_result *code_result);
int s21_needs_for_division(s21_decimal value_1, s21_decimal value_2);

enum comparision { FALSE, TRUE };

int s21_get_sign(s21_decimal value);

s21_decimal s21_set_zeroes();

int s21_get_scale(s21_decimal value);
void s21_set_scale(s21_decimal *value, int scale);
s21_decimal s21_int128_binary_xor(s21_decimal decimal1, s21_decimal decimal2);
s21_decimal s21_int128_binary_and(s21_decimal decimal1, s21_decimal decimal2);
s21_decimal s21_int128_binary_not(s21_decimal decimal1);
s21_decimal s21_int128_binary_or(s21_decimal decimal1, s21_decimal decimal2);
int s21_is_decimal_zero(s21_decimal value);
s21_decimal s21_change_sign(s21_decimal *value);
s21_decimal s21_shift_left(s21_decimal value, int shift_num,
                           s21_code_result *code_result);
s21_decimal s21_int128_one_shift_left(s21_decimal value);
s21_decimal s21_shift_right(s21_decimal value, int shift_num,
                            s21_code_result *code_result);
s21_decimal s21_int128_one_shift_right(s21_decimal value);

int s21_is_decimal_zero(s21_decimal value);
void s21_set_scale(s21_decimal *value, int scale);
int s21_set_bit(int value, int bit_pos);
int s21_decimal_what_bit_is_it(s21_decimal value, int index);

int s21_valid_decimal(s21_decimal value);
int s21_should_be_empty(s21_decimal value);
int s21_decimal_find_not_zero_bit(s21_decimal value);

int s21_double_getSign(s21_double_decimal value);
int s21_double_to_decimal(s21_decimal value, s21_double_decimal *double_value);
s21_double_decimal s21_Double_Summ(s21_double_decimal double_value_1,
                                   s21_double_decimal double_value_2);
int s21_double_common_denominator(s21_double_decimal *value_1,
                                  s21_double_decimal *value_2);

void s21_swap_decimal(s21_decimal *val1, s21_decimal *val2);
int s21_levelling(s21_decimal *value_1, s21_decimal *value_2);

int s21_needs_for_division(s21_decimal value_1, s21_decimal value_2);

#endif