

#include <stdio.h>
#include <stdlib.h>

#include "s21_decimal.h"

void printf_dec(s21_decimal result) {
  printf("Decimal is %d %d %d %d\n", result.bits[3], result.bits[2],
         result.bits[1], result.bits[0]);
}

void printf_dec_binary(s21_decimal result) {
  for (int i = 127; i >= 0; i--) {
    printf("%d", s21_get_bit(result.bits[i / 32], i));
    if (i % 32 == 0) printf(" | ");
  }
  printf("\n");
}

int s21_get_sign(s21_decimal value) { return value.bits[3] >> 31; }

s21_decimal s21_change_sign(s21_decimal *value) { value->bits[3] ^= 1u << 31; return *value; }

int s21_set_bit(int value, int bit_pos) { return value | (1U << bit_pos); }

int s21_set_zero_bit(int value, int bit_pos) { return value | (0U << bit_pos); }

s21_decimal s21_set_zeroes() {
  s21_decimal result;
  result.bits[0] = 0;
  result.bits[1] = 0;
  result.bits[2] = 0;
  result.bits[3] = 0;

  return result;
}

s21_decimal s21_int128_binary_xor(s21_decimal decimal1, s21_decimal decimal2) {
  s21_decimal result = s21_set_zeroes();
  result.bits[0] = decimal1.bits[0] ^ decimal2.bits[0];
  result.bits[1] = decimal1.bits[1] ^ decimal2.bits[1];
  result.bits[2] = decimal1.bits[2] ^ decimal2.bits[2];
  result.bits[3] = decimal1.bits[3] ^ decimal2.bits[3];

  return result;
}

s21_decimal s21_int128_binary_and(s21_decimal decimal1, s21_decimal decimal2) {
  s21_decimal result = s21_set_zeroes();
  result.bits[0] = decimal1.bits[0] & decimal2.bits[0];
  result.bits[1] = decimal1.bits[1] & decimal2.bits[1];
  result.bits[2] = decimal1.bits[2] & decimal2.bits[2];
  result.bits[3] = decimal1.bits[3] & decimal2.bits[3];

  return result;
}

s21_decimal s21_int128_binary_not(s21_decimal decimal1) {
  s21_decimal result = s21_set_zeroes();
  result.bits[0] = ~decimal1.bits[0];
  result.bits[1] = ~decimal1.bits[1];
  result.bits[2] = ~decimal1.bits[2];
  result.bits[3] = ~decimal1.bits[3];

  return result;
}

s21_decimal s21_int128_binary_or(s21_decimal decimal1, s21_decimal decimal2) {
  s21_decimal result = s21_set_zeroes();
  result.bits[0] = decimal1.bits[0] | decimal2.bits[0];
  result.bits[1] = decimal1.bits[1] | decimal2.bits[1];
  result.bits[2] = decimal1.bits[2] | decimal2.bits[2];
  result.bits[3] = decimal1.bits[3] | decimal2.bits[3];

  return result;
}

int s21_is_decimal_zero(s21_decimal value) {
  int zero = FALSE;
  if (!value.bits[0] && !value.bits[1] && !value.bits[2]) zero = TRUE;
  return zero;
}

int s21_get_bit(int val_check, int position) {
  int get_bit = (val_check >> position) & 1U;
  return get_bit;
}

void s21_set_scale(s21_decimal *value, int scale) {
  int bit = 0;
  int powerBitPos = 0;
  for (int i = 16; i <= 23; i++) {
    bit = s21_get_bit(scale, powerBitPos);
    if (bit == 1)
      value->bits[3] = s21_set_bit(value->bits[3], i);
    else
      value->bits[3] = s21_set_zero_bit(value->bits[3], i);
    powerBitPos++;
  }
}

void s21_make_equal(s21_decimal value_1, s21_decimal *value_2) {
  for (int i = 0; i < 4; i++) {
    value_2->bits[i] = value_1.bits[i];
  }
}

void s21_swap_decimal(s21_decimal *val1, s21_decimal *val2) {
  s21_decimal temp = {0};

  for (int i = 0; i < 4; i++) {
    temp.bits[i] = val1->bits[i];
    val1->bits[i] = val2->bits[i];
    val2->bits[i] = temp.bits[i];
  }
}

int s21_levelling(s21_decimal *value_1, s21_decimal *value_2) {
  int difference = s21_get_scale(*value_1) - s21_get_scale(*value_2);
  if (difference == 0) return 0;

  int ret = 0, scale_res = 0;
  s21_decimal decimalOne = {0};
  s21_make_equal(*value_1, &decimalOne);
  s21_set_scale(&decimalOne, 0);

  s21_decimal decimalTwo = {0};
  s21_make_equal(*value_2, &decimalTwo);
  s21_set_scale(&decimalTwo, 0);

  s21_decimal tenDec = {{10, 0, 0, 0}};

  s21_decimal buffer = {0};

  int sign1 = s21_get_sign(decimalOne);
  int sign2 = s21_get_sign(decimalTwo);
  if (sign1) s21_change_sign(&decimalOne);
  if (sign2) s21_change_sign(&decimalTwo);

  if (difference < 0) {
    scale_res = s21_get_scale(*value_2);
    for (int i = 0; i < ((-1) * difference); i++) {
      ret = s21_binary_multiplication(decimalOne, tenDec, &buffer);
      s21_swap_decimal(&decimalOne, &buffer);
      s21_set_zeroes(&buffer);
    }
  } else if (difference > 0) {
    scale_res = s21_get_scale(*value_1);
    for (int i = 0; i < difference; i++) {
      ret = s21_binary_multiplication(decimalTwo, tenDec, &buffer);
      s21_swap_decimal(&decimalTwo, &buffer);
      s21_set_zeroes(&buffer);
    }
  }

  if (!ret) {
    *value_1 = s21_set_zeroes();
    *value_2 = s21_set_zeroes();

    s21_make_equal(decimalOne, value_1);
    if (sign1) s21_set_bit(value_1->bits[3], 31);
    s21_set_scale(value_1, scale_res);

    s21_make_equal(decimalTwo, value_2);
    if (sign2) s21_set_bit(value_2->bits[3], 31);
    s21_set_scale(value_2, scale_res);
  }

  return ret;
}

int s21_get_scale(s21_decimal value) {
  int power = value.bits[3] << 1;
  power = power >> 17;

  return power;
}

s21_decimal s21_shift_left(s21_decimal value, int shift_num,
                           s21_code_result *code_result) {
  s21_decimal result = value;
  result.bits[3] = 0;

  while (shift_num > 0 && !(*code_result)) {
    result = s21_int128_one_shift_left(result);
    shift_num--;
  }

  if (result.bits[3])
    *code_result = S21_RESULT_TOOLARGE;
  else
    *code_result = S21_RESULT_OK;
  result.bits[3] = value.bits[3];

  return result;
}

s21_decimal s21_int128_one_shift_left(s21_decimal value) {
  s21_decimal result = {0};

  int zero_bit = s21_decimal_what_bit_is_it(value, INT_BITS - 1);
  unsigned int result0 = value.bits[0];
  result0 = result0 << 1;
  result.bits[0] = result0;

  int first_bit = s21_decimal_what_bit_is_it(value, BITS_DECIMAL - 65);
  unsigned int result1 = value.bits[1];
  result1 = result1 << 1;
  result.bits[1] = result1;

  int second_bit = s21_decimal_what_bit_is_it(value, BITS_DECIMAL - 33);
  unsigned int result2 = value.bits[2];
  result2 = result2 << 1;
  result.bits[2] = result2;

  unsigned int result3 = value.bits[3];
  result3 = result3 << 1;
  result.bits[3] = result3;

  if (zero_bit) {
    result.bits[1] = s21_set_bit(result.bits[1], 0);
  }
  if (first_bit) {
    result.bits[2] = s21_set_bit(result.bits[2], 0);
  }

  if (second_bit) {
    result.bits[3] = s21_set_bit(result.bits[3], 0);
  }

  return result;
}

s21_decimal s21_shift_right(s21_decimal value, int shift_num,
                            s21_code_result *code_result) {
  s21_decimal result = value;
  result.bits[3] = 0;

  while (shift_num > 0 && !(*code_result)) {
    result = s21_int128_one_shift_right(result);
    shift_num--;
  }

  if (result.bits[3])
    *code_result = S21_RESULT_TOOLARGE;
  else
    *code_result = S21_RESULT_OK;
  result.bits[3] = value.bits[3];
  return result;
}

s21_decimal s21_int128_one_shift_right(s21_decimal value) {
  s21_decimal result = {0};

  int third_bit = s21_decimal_what_bit_is_it(value, BITS_DECIMAL - INT_BITS);
  unsigned int bit3 = value.bits[3];
  bit3 = bit3 >> 1;
  result.bits[3] = bit3;

  int second_bit = s21_decimal_what_bit_is_it(value, INT_BITS + INT_BITS);
  unsigned int bit2 = value.bits[2];
  bit2 = bit2 >> 1;
  result.bits[2] = bit2;

  int first_bit = s21_decimal_what_bit_is_it(value, INT_BITS);
  unsigned int bit1 = value.bits[3];
  bit1 = bit1 >> 1;
  result.bits[1] = bit1;

  unsigned int bit0 = value.bits[0];
  bit0 = bit0 >> 1;
  result.bits[0] = bit0;

  if (third_bit) {
    result.bits[2] = s21_set_bit(result.bits[2], INT_BITS - 1);
  }
  if (second_bit) {
    result.bits[1] = s21_set_bit(result.bits[1], INT_BITS - 1);
  }
  if (first_bit) {
    result.bits[0] = s21_set_bit(result.bits[0], INT_BITS - 1);
  }

  return result;
}

int s21_decimal_what_bit_is_it(s21_decimal value, int index) {
  int num = value.bits[index / INT_BITS];
  int bit_order = index % INT_BITS;

  int result = !!(num & (1U << bit_order));
  return result;
}


int s21_valid_decimal(s21_decimal value) {
  int flag = 1;

  int power = s21_get_scale(value);
  if (power < 0 || power > 28) flag = 0;

  if (s21_should_be_empty(value)) flag = 0;

  return flag;
}

int s21_should_be_empty(s21_decimal value) {
  int flag = 1;

  for (int i = 0; i < 16; i++) {
    if (s21_decimal_what_bit_is_it(value, i + 96)) flag = 0;
  }

  for (int i = 24; i <= 30; i++) {
    if (s21_decimal_what_bit_is_it(value, i + 96)) flag = 0;
  }

  return flag;
}

int s21_decimal_find_not_zero_bit(s21_decimal value) {
  int result = -1;

  for (int i = 95; i >= 0; i--) {
    if (s21_decimal_what_bit_is_it(value, i)) {
      result = i;
      break;
    }
  }
  return result;
}
