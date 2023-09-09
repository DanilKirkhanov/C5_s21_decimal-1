
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