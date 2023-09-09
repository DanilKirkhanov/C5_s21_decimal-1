
#include "s21_decimal.h"
int s21_negate(s21_decimal value, s21_decimal* result) {
  if (!result) return 1;
  *result = value;
  if (get_bit(value, 127)) {
    set_bit(result, 127, 0);
  } else
    set_bit(result, 127, 1);
  return 0;
}

int s21_truncate(s21_decimal value, s21_decimal* result) {
  if (!result) return 1;
  *result = value;
  div_by_10(result, get_scale(*result));
  return 0;
}

int s21_floor(s21_decimal value, s21_decimal* result) {
  int err = 0;
  if (!result)
    err = 1;
  else {
    if (get_scale(value) == 0)
      *result = value;
    else {
      s21_truncate(value, result);
      if (get_bit(value, 127)) s21_add_1_(result);  // вычитание 1;
    }
  }
  return err;
}

void s21_add_1_(s21_decimal* result) {
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


int s21_round(s21_decimal value, s21_decimal* result) {
  int err = 0;
  if (!result)
    err = 1;
  else {
    int sign = s21_get_sign(value);
    value.bits[3] = s21_set_bit(value.bits[3], 31);
    s21_decimal five = {{5, 0, 0, 0x10000}};
    s21_decimal one = {{1, 0, 0, 0}};
    s21_decimal trun = {{0, 0, 0, 0}};
    s21_decimal sub_dec = {{0, 0, 0, 0}};

    s21_truncate(value, &trun);
    s21_sub(value, trun, &sub_dec);
    if (s21_is_greater_or_equal(five, sub_dec))
      s21_add(trun, one, result);
    else
      *result = trun;
    if (sign)
      result->bits[3] =
          0x80000000;  // зануляем скейл и ставим отрицательный знак
    else
      result->bits[3] = 0;  // зануляем скейл, оставляется положительный знак
  }
  return err;
}