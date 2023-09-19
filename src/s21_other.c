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
      if (get_bit(value, 127)) s21_add_1_(result);
    }
  }
  return err;
}

int s21_round(s21_decimal value, s21_decimal* result) {
  int err = 0;
  if (!result)
    err = 1;
  else {
    int sign = s21_get_sign(value);
    s21_setBit(&value, 127, 0);
    s21_decimal trun = {{0, 0, 0, 0}};
    s21_decimal sub_dec = {{0, 0, 0, 0}};

    s21_truncate(value, &trun);
    if (s21_get_scale(value) != 0) div_by_10(&value, s21_get_scale(value) - 1);
    s21_sub(value, trun, &sub_dec);
    if (sub_dec.bits[0] >= 5) s21_add_1_(&trun);
    *result = trun;
    if (sign)
      result->bits[3] = 0x80000000;
    else
      result->bits[3] = 0;
  }
  return err;
}
