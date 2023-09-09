#ifndef S21_HELPERSNEW_H
#define S21_HELPERSNEW_H

#include <math.h>
#include "s21_decimal.h"
#include "s21_arithmetic.h"


#define BITS_DECIMAL 128
#define INT_BITS 32

enum comparision { FALSE, TRUE };

void printf_dec(s21_decimal result);
void printf_dec_binary(s21_decimal result);

int s21_get_sign(s21_decimal value);
int s21_get_bit(int val_check, int position);
// void s21_levelling(s21_decimal *value1, s21_decimal *value2);
// void s21_change_sign(s21_decimal *value);
void s21_compare(s21_decimal value1, s21_decimal value2);
// s21_decimal s21_binary_sub(s21_decimal value1, s21_decimal value2);

s21_decimal s21_set_zeroes();

int s21_get_scale(s21_decimal value);
int s21_change_scale(s21_decimal *value1, s21_decimal *value2, int difference);
void s21_set_scale(s21_decimal *value, int scale);
s21_decimal s21_int128_binary_xor(s21_decimal decimal1, s21_decimal decimal2);
s21_decimal s21_int128_binary_and(s21_decimal decimal1, s21_decimal decimal2);
s21_decimal s21_int128_binary_not(s21_decimal decimal1);
s21_decimal s21_int128_binary_or(s21_decimal decimal1, s21_decimal decimal2);
int s21_is_decimal_zero(s21_decimal value);
s21_decimal s21_change_sign(s21_decimal *value);
s21_decimal s21_shift_left(s21_decimal value, int shift_num,
                           s21_code_result *code_result);  // 3d parameter!!!!
s21_decimal s21_int128_one_shift_left(s21_decimal value);
s21_decimal s21_shift_right(s21_decimal value, int shift_num,
                            s21_code_result *code_result);  // 3d parameter!!!!
s21_decimal s21_int128_one_shift_right(s21_decimal value);

int s21_is_decimal_zero(s21_decimal value);
void s21_set_scale(s21_decimal *value, int scale);
int s21_set_bit(int value, int bit_pos);
int s21_decimal_what_bit_is_it(s21_decimal value, int index);

int s21_valid_decimal(s21_decimal value);
int s21_should_be_empty(s21_decimal value);
int s21_decimal_find_not_zero_bit(s21_decimal value);
int s21_set_zero_bit(int value, int bit_pos);

void s21_swap_decimal(s21_decimal *val1, s21_decimal *val2);
// void s21_make_equal(s21_decimal *val1, s21_decimal *val2);
int s21_levelling(s21_decimal *value_1, s21_decimal *value_2);
// void s21_set_zeroes(s21_decimal *result);

#endif
