#include "s21_decimal.h"

int get_bit(s21_decimal src, int index) {
  int bits = src.bits[index / 32];
  int mask = 1;
  mask <<= index % 32;
  return mask & bits ? 1 : 0;
}

void set_bit(s21_decimal *src, int index, int bit) {
  int mask = 1;
  mask <<= (index % 32);
  if (bit) {
    src->bits[index / 32] |= mask;
  } else {
    src->bits[index / 32] &= ~mask;
  }
}

void s21_set_scale(s21_decimal *src, int scale) {
  for (int i = 112; i < 119; i++) {
    set_bit(src, i, scale & 1);
    scale >>= 1;
  }
}

int get_scale(s21_decimal src) {
  src.bits[3] >>= 16;
  return (unsigned char)src.bits[3];
}

void div_by_10(s21_decimal *src, int scale) {
  s21_set_scale(src, get_scale(*src) - scale);
  for (; scale != 0; scale--) {
    unsigned long temp = src->bits[2];
    for (int i = 2; i >= 0; i--) {
      src->bits[i] = temp / 10;
      temp = temp % 10;
      temp = (temp << 32) + (unsigned long)src->bits[i - 1];
    }
  }
}

int s21_Double_GetSign(s21_double_decimal value) {
  return (value.bits[5] >> 31);
}

void s21_Double_SubScale(s21_double_decimal *value) { value->bits[5] += 1; }

void s21_Double_SetSign(s21_double_decimal *value, int bit) {
  unsigned number = 1;
  if (bit == 0)
    value->bits[5] = ~(~value->bits[5] | number << 31);
  else
    value->bits[5] = value->bits[5] | number << 31;
}

s21_double_decimal s21_Double_ShiftLeft(s21_double_decimal value, int inex) {
  for (int i = 4; i != -1; i--) {
    int remain = 0;
    if (i != 0 && ((value.bits[i - 1] >> (32 - inex)) != 0))
      remain = value.bits[i - 1] >> (32 - inex);
    value.bits[i] = (value.bits[i] << inex) | remain;
  }
  return value;
}

void s21_Double_SetBit(s21_double_decimal *dec, int index, int bit) {
  int num_int = index / 32;
  int num_bit = index % 32;
  if (bit == 1)
    dec->bits[num_int] |= (1u << num_bit);
  else
    dec->bits[num_int] &= (~((1u) << num_bit));
}

int s21_Double_GetBit(s21_double_decimal dec, int index) {
  int error = -1;
  if (index >= 0 && index <= 191) {
    int num_int = index / 32;
    int num_bit = index % 32;
    error = !!(dec.bits[num_int] & (1u << num_bit));
  }
  return error;
}

s21_double_decimal s21_Double_Summ(s21_double_decimal double_value_1,
                                   s21_double_decimal double_value_2) {
  s21_double_decimal value = {};
  value.bits[5] = double_value_1.bits[5];
  int flag_plus_1 = 0;
  for (int i = 0; i != 160; i++) {
    int number = s21_Double_GetBit(double_value_1, i) +
                 s21_Double_GetBit(double_value_2, i) + flag_plus_1;
    s21_Double_SetBit(&value, i, number % 2);
    flag_plus_1 = number / 2;
  }
  return value;
}

s21_decimal s21_binary_additional(s21_decimal value1, s21_decimal value2,
                                  s21_code_result *code_result) {
  while (!s21_is_decimal_zero(value2)) {
    s21_decimal temp = s21_int128_binary_and(value1, value2);
    value1 = s21_int128_binary_xor(value1, value2);
    value2 = s21_shift_left(temp, 1, code_result);
  }
  return value1;
}

int s21_help_multiply(s21_decimal value_1, s21_decimal value_2,
                      s21_decimal *result) {
  s21_code_result code_result = S21_RESULT_OK;
  int scale = (s21_get_scale(value_1) + s21_get_scale(value_2)) << 16;
  value_1.bits[3] = 0;
  value_2.bits[3] = 0;
  code_result = s21_binary_multiplication(value_1, value_2, result);
  result->bits[3] = scale;
  if (scale > 28) code_result = S21_RESULT_TOOSMALL;
  return code_result;
}

int s21_binary_multiplication(s21_decimal value_1, s21_decimal value_2,
                              s21_decimal *result) {
  s21_decimal res = s21_set_zeroes();
  s21_code_result code_result = S21_RESULT_OK;
  int significant_bit = s21_decimal_find_not_zero_bit(value_2);
  for (int i = 0; i <= significant_bit; i++) {
    if (s21_decimal_what_bit_is_it(value_2, i) != 0) {
      res = s21_binary_additional(res, value_1, &code_result);
    }
    value_1 = s21_shift_left(value_1, 1, &code_result);
  }
  *result = res;
  return code_result;
}

s21_decimal s21_binary_sub(s21_decimal value_1, s21_decimal value_2,
                           s21_code_result *code_result) {
  while (!s21_is_decimal_zero(value_2)) {
    s21_decimal temp =
        s21_int128_binary_and(s21_int128_binary_not(value_1), value_2);
    value_1 = s21_int128_binary_xor(value_1, value_2);
    value_2 = s21_shift_left(temp, 1, code_result);
  }
  return value_1;
}

s21_decimal s21_binary_division(s21_decimal value_1, s21_decimal value_2,
                                s21_code_result *code_result) {
  s21_decimal result = {0};
  s21_decimal remains = {0};

  s21_decimal one = {{1, 0, 0, 0}};

  for (int i = 95; i >= 0; i--) {
    remains = s21_shift_left(remains, 1, code_result);
    remains = s21_int128_binary_or(
        remains,
        s21_int128_binary_and(s21_shift_right(value_1, i, code_result), one));
    if (s21_needs_for_division(remains, value_2)) {
      result =
          s21_int128_binary_or(result, s21_shift_left(one, i, code_result));
      remains = s21_binary_sub(remains, value_2, code_result);
    }
  }
  return result;
}

int s21_needs_for_division(s21_decimal value_1, s21_decimal value_2) {
  int flag = TRUE;
  int sign1 = s21_get_sign(value_1), sign2 = s21_get_sign(value_2);
  s21_levelling(&value_1, &value_2);
  if (s21_is_equal(value_1, value_2)) {
    flag = TRUE;
  } else if (sign1 && !sign2) {
    flag = FALSE;
  } else if (!sign1 && sign2) {
    flag = TRUE;
  } else if (sign1 && sign2) {
    for (int i = 2; i >= 0 && flag; i--) {
      if (value_1.bits[i] > value_2.bits[i]) flag = FALSE;
    }
  } else {
    for (int i = 2; i >= 0 && flag; i--) {
      if (value_1.bits[i] < value_2.bits[i]) flag = FALSE;
    }
  }
  return flag;
}

int s21_getBit(s21_decimal dec, int index) {
  int error = -1;
  if (index >= 0 && index <= 127) {
    int num_int = index / 32;
    int num_bit = index % 32;
    error = !!(dec.bits[num_int] & (1u << num_bit));
  }
  return error;
}

int s21_getSign(s21_decimal value) {
  int sign = s21_getBit(value, 127);
  return sign;
}

void s21_setBit(s21_decimal *value, int index, int bit) {
  int mask = 1u << (index % 32);
  if (bit == 0)
    value->bits[index / 32] = value->bits[index / 32] & ~mask;
  else
    value->bits[index / 32] = value->bits[index / 32] | mask;
}

void s21_setSign(s21_decimal *value, int sign) { s21_setBit(value, 127, sign); }

void s21_setScale(s21_decimal *dec, int scale) {
  for (int i = 112; i < 119; i++) {
    s21_setBit(dec, i, scale & 1);
    scale >>= 1;
  }
}

int s21_getScale(s21_decimal dec) {
  int result = (char)(dec.bits[3] >> 16);
  return result;
}

void s21_add_1_(s21_decimal *result) {
  if ((unsigned long int)result->bits[0] != 4294967295)
    result->bits[0] += 1;
  else {
    result->bits[0] = 0;
    if ((unsigned long int)result->bits[1] != 4294967295)
      result->bits[1] += 1;
    else {
      result->bits[1] = 0;
      result->bits[2] += 1;
    }
  }
}
void s21_ZeroDecimal(s21_decimal *dst) {
  dst->bits[0] = 0;
  dst->bits[1] = 0;
  dst->bits[2] = 0;
  dst->bits[3] = 0;
}
int s21_GetBitToFloat(double number, int bit_number) {
  int result = -1;
  for (int i = 0; i < bit_number; i++) number = floor(number / 2);
  result = (int)fmod(number, 2.0);
  return result;
}

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

int s21_get_sign(s21_decimal value) { return value.bits[3] >> 31; }

s21_decimal s21_change_sign(s21_decimal *value) {
  value->bits[3] ^= 1u << 31;
  return *value;
}

int s21_set_bit(int value, int bit_pos) { return value | (1U << bit_pos); }

int s21_is_decimal_zero(s21_decimal value) {
  int zero = FALSE;
  if (!value.bits[0] && !value.bits[1] && !value.bits[2]) zero = TRUE;
  return zero;
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
    s21_set_scale(value_1, scale_res);
    s21_make_equal(decimalTwo, value_2);
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

int s21_double_getSign(s21_double_decimal value) {
  return (value.bits[5] >> 31);
}

int s21_double_common_denominator(s21_double_decimal *value_1,
                                  s21_double_decimal *value_2) {
  int error = 0;
  int sign_1 = s21_Double_GetSign(*value_1);
  int sign_2 = s21_Double_GetSign(*value_2);
  if (sign_1 != sign_2)
    sign_1 == 1 ? s21_Double_SetSign(value_1, 0)
                : s21_Double_SetSign(value_2, 0);
  if (value_1->bits[5] != value_2->bits[5]) {
    if (value_1->bits[5] > value_2->bits[5])
      s21_double_common_denominator(value_2, value_1);
    else {
      value_1->bits[5] = value_1->bits[5] >> 16;
      value_2->bits[5] = value_2->bits[5] >> 16;
      while (value_1->bits[5] != value_2->bits[5]) {
        s21_Double_SubScale(value_1);
        s21_double_decimal one = s21_Double_ShiftLeft(*value_1, 3);
        s21_double_decimal two = s21_Double_ShiftLeft(*value_1, 1);
        *value_1 = s21_Double_Summ(one, two);
      }
      value_1->bits[5] = value_1->bits[5] << 16;
      value_2->bits[5] = value_2->bits[5] << 16;
    }
  }
  if (sign_1 != sign_2)
    sign_1 == 1 ? s21_Double_SetSign(value_1, 1)
                : s21_Double_SetSign(value_2, 1);
  return error;
}

int s21_double_to_decimal(s21_decimal value, s21_double_decimal *double_value) {
  int error = 0;
  if (double_value) {
    double_value->bits[0] = value.bits[0];
    double_value->bits[1] = value.bits[1];
    double_value->bits[2] = value.bits[2];
    double_value->bits[5] = value.bits[3];
  } else
    error = 1;
  return error;
}