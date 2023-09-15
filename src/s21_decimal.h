#ifndef S21_DECIMAL_H
#define S21_DECIMAL_H

#include <limits.h>
#include <math.h>
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


s21_decimal s21_binary_additional(s21_decimal value1, s21_decimal value2, s21_code_result *code_result);
s21_decimal s21_binary_sub(s21_decimal value_1, s21_decimal value_2, s21_code_result *code_result);
int s21_help_multiply(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_binary_multiplication(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

void s21_ZeroDecimal(s21_decimal *dst);
int s21_getBit(s21_decimal num, int place);
int s21_getSign(s21_decimal value);
void s21_setBit(s21_decimal *value, int index, int bit);
void s21_setSign(s21_decimal *value, int sign);
void s21_setScale(s21_decimal *num, int exp);
int s21_getScale(s21_decimal dst);
void s21_setScaleToZero(s21_decimal *number);
int s21_GetBitToFloat(double number, int bit_number);
void s21_scaleEqul(s21_decimal *value_1, s21_decimal *value_2);
int s21_equalWithoutSign(s21_decimal value_1, s21_decimal value_2);
int s21_less_without_mod(s21_decimal dec_1, s21_decimal dec_2);
s21_decimal s21_div_integer(s21_decimal src, int div);

void printf_decimal(s21_decimal dec);  // вывести полный децимал
void printf_binary(int src);           // вывести один инт
void printf_long_binary(long int src);  // вывести длинный инт
int get_bit(s21_decimal src, int index);  // взять бит из децимала (от 0 до 127)
void set_bit(s21_decimal *src, int index, int bit);  // назначить бит (0 или 1) из децимала (от 0 до 127)
void set_scale(s21_decimal *dec, int scale);  // назначить степень
int get_scale(s21_decimal src);               // узнать степень
void div_by_10(s21_decimal *src, int scale);  // деление на 10 n раз, степень автоматически
                            // уменьшается на n раз

void s21_add_1_(s21_decimal *result);

// 0 - OK
// 1 - the number is too large or equal to infinity
// 2 - the number is too small or equal to negative infinity
// 3 - division by 0

s21_decimal s21_binary_division(s21_decimal value_1, s21_decimal value_2, s21_code_result *code_result);
int s21_needs_for_division(s21_decimal value_1, s21_decimal value_2);

enum comparision { FALSE, TRUE };

void printf_dec(s21_decimal result);
void printf_dec_binary(s21_decimal result);
int s21_get_sign(s21_decimal value);
int s21_get_bit(int val_check, int position);
// void s21_levelling(s21_decimal *value1, s21_decimal *value2);
// void s21_change_sign(s21_decimal *value);
//void s21_compare(s21_decimal value1, s21_decimal value2);
// s21_decimal s21_binary_sub(s21_decimal value1, s21_decimal value2);

s21_decimal s21_set_zeroes();

int s21_get_scale(s21_decimal value);
//int s21_change_scale(s21_decimal *value1, s21_decimal *value2, int difference);
void s21_set_scale(s21_decimal *value, int scale);
s21_decimal s21_int128_binary_xor(s21_decimal decimal1, s21_decimal decimal2);
s21_decimal s21_int128_binary_and(s21_decimal decimal1, s21_decimal decimal2);
s21_decimal s21_int128_binary_not(s21_decimal decimal1);
s21_decimal s21_int128_binary_or(s21_decimal decimal1, s21_decimal decimal2);
int s21_is_decimal_zero(s21_decimal value);
s21_decimal s21_change_sign(s21_decimal *value);
s21_decimal s21_shift_left(s21_decimal value, int shift_num, s21_code_result *code_result);  // 3d parameter!!!!
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


int s21_double_getSign(s21_double_decimal value);
int s21_double_to_decimal(s21_decimal value, s21_double_decimal *double_value);
void s21_double_set_sign(s21_double_decimal *value, int bit);
s21_double_decimal s21_Double_Summ(s21_double_decimal double_value_1,
                                   s21_double_decimal double_value_2);
int s21_double_get_bit(s21_double_decimal dec, int index);
void s21_double_set_bit(s21_double_decimal *dec, int index, int bit);
int s21_double_common_denominator(s21_double_decimal *value_1,
                                  s21_double_decimal *value_2);
void s21_double_sub_scale(s21_double_decimal *value);
s21_double_decimal s21_double_shiftLeft(s21_double_decimal value, int inex);

void s21_swap_decimal(s21_decimal *val1, s21_decimal *val2);
int s21_levelling(s21_decimal *value_1, s21_decimal *value_2);

int s21_needs_for_division(s21_decimal value_1, s21_decimal value_2);


#endif  // S21_DECIMAL_H