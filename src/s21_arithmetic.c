

#include <stdio.h>

#include "s21_decimal.h"

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  s21_code_result code_result = S21_RESULT_OK;

  if (!result) {
    code_result = S21_RESULT_ERROR;
  } else if (s21_valid_decimal(value_1) || s21_valid_decimal(value_2)) {
    code_result = S21_RESULT_ERROR;
  } else {
    int sign1 = s21_get_sign(value_1), sign2 = s21_get_sign(value_2);
    s21_levelling(&value_1, &value_2);
    if (sign1 == sign2) {
      *result = s21_binary_additional(value_1, value_2, &code_result);
    } else {
      if (sign1) s21_change_sign(&value_1);
      if (sign2) s21_change_sign(&value_2);
      if (s21_is_greater_or_equal(value_1, value_2)) {
        *result = s21_binary_sub(value_1, value_2, &code_result);
        if (sign1) s21_change_sign(result);
      } else {
        *result = s21_binary_sub(value_2, value_1,
                                 &code_result);  // SUBTRACTION FUNCTION
        if (sign2) s21_change_sign(result);
      }
    }
  }
  return code_result;
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

int s21_mul(s21_decimal value_1, s21_decimal value_2,
                 s21_decimal *result) {
  s21_code_result code_result;

  if (!result) {
    code_result = S21_RESULT_ERROR;
  } else if (s21_valid_decimal(value_1) || s21_valid_decimal(value_2)) {
    code_result = S21_RESULT_ERROR;
  } else {
    *result = s21_set_zeroes();
    // s21_decimal temp = s21_set_zeroes();

    int value1_sign = s21_get_sign(value_1);
    int value2_sign = s21_get_sign(value_2);

    if (value1_sign == S21_POSITIVE && value2_sign == S21_POSITIVE) {
      code_result = s21_help_multiply(value_1, value_2, result);

    } else if (value1_sign == S21_POSITIVE && value2_sign == S21_NEGATIVE) {
      code_result =
          s21_help_multiply(value_1, s21_change_sign(&value_2), result);
      s21_change_sign(result);

    } else if (value1_sign == S21_NEGATIVE && value2_sign == S21_POSITIVE) {
      code_result =
          s21_help_multiply(value_2, s21_change_sign(&value_1), result);
      s21_change_sign(result);

    } else if (value1_sign == S21_NEGATIVE && value2_sign == S21_NEGATIVE) {
      code_result =
          s21_help_multiply(value_2, s21_change_sign(&value_1), result);
    }
  }
  return code_result;
}

int s21_help_multiply(s21_decimal value_1, s21_decimal value_2,
                      s21_decimal *result) {
  s21_code_result code_result = S21_RESULT_OK;

  int scale = (s21_get_scale(value_1) + s21_get_scale(value_2)) << 16;

  value_1.bits[3] = 0;  // s21_set_zeroes_bits3(value_1);
  value_2.bits[3] = 0;  // s21_set_zeroes_bits3(value_2);

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

int s21_sub(s21_decimal value_1, s21_decimal value_2,
                    s21_decimal *result) {
  s21_code_result code_result = S21_RESULT_OK;
  if (!result) {
    code_result = S21_RESULT_ERROR;
  } else if (s21_valid_decimal(value_1) || s21_valid_decimal(value_2)) {
    code_result = S21_RESULT_ERROR;
  } else {
    int sign1 = s21_get_sign(value_1), sign2 = s21_get_sign(value_2);
    s21_levelling(&value_1, &value_2);
    if (sign1 == sign2) {
      if (sign1) {
        if (s21_is_less_or_equal(value_1, value_2)) {
          *result = s21_binary_sub(value_1, value_2, &code_result);
        } else {
          *result = s21_binary_sub(value_2, value_1, &code_result);
          s21_change_sign(result);
        }
      } else {
        if (s21_is_greater_or_equal(value_1, value_2))
          *result = s21_binary_sub(value_1, value_2, &code_result);
        else {
          *result = s21_binary_sub(value_2, value_1, &code_result);
          s21_change_sign(result);
        }
      }
    } else {
      if (sign1)
        *result = s21_binary_additional(value_1, value_2, &code_result);
      else
        *result = s21_binary_additional(value_1, value_2, &code_result);
    }
  }
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

int s21_div(s21_decimal value_1, s21_decimal value_2,
                 s21_decimal *result) {
  s21_code_result code_result = S21_RESULT_OK;

  if (!result) {
    code_result = S21_RESULT_ERROR;
  } else if (s21_valid_decimal(value_1) || s21_valid_decimal(value_2)) {
    code_result = S21_RESULT_ERROR;
  } else {
    if (s21_is_decimal_zero(value_2)) {
      code_result = S21_DIV_ZERO;
    } else {
      int sign1 = s21_get_sign(value_1), sign2 = s21_get_sign(value_2);
      int result_int_3 = value_1.bits[3];
      if (sign1) s21_change_sign(&value_1);
      if (sign2) s21_change_sign(&value_2);
      *result = s21_binary_division(value_1, value_2, &code_result);
      result->bits[3] = result_int_3;
      if (sign1 && sign2)
        s21_change_sign(result);
      else if (sign1 ^ sign2) {
        if (!s21_get_sign(*result)) s21_change_sign(result);
      }
      int scale = s21_get_scale(value_1) - s21_get_scale(value_2);
      s21_set_scale(result, (scale > 0) ? scale : 0);
    }
  }
  return code_result;
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
      remains = s21_binary_sub(remains, value_2, code_result);  // SUBTRACTION
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
