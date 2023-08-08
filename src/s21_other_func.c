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

