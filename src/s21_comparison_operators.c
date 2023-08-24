#include "s21_decimal.h"

int s21_is_equal(s21_decimal dec_1, s21_decimal dec_2) {
  int result = 1;
  // s21_decimal zero = {{0, 0, 0, 0}};
  if ((dec_1.bits[0] | dec_1.bits[1] | dec_1.bits[2] | dec_2.bits[0] |
       dec_2.bits[1] | dec_2.bits[2]) != 0) {
    if (s21_getScale(dec_1) != s21_getScale(dec_2))
      s21_scaleEqul(&dec_1, &dec_2);
    if (dec_1.bits[0] != dec_2.bits[0]) {
      result = 0;
    } else if (dec_1.bits[1] != dec_2.bits[1]) {
      result = 0;
    } else if (dec_1.bits[2] != dec_2.bits[2]) {
      result = 0;
    } else if (s21_getSign(dec_1) != s21_getSign(dec_2)) {
      result = 0;
    } else {
      result = 1;
    }
  }
  return result;
}

int s21_is_less(s21_decimal dec_1, s21_decimal dec_2) {
  int result = 0;
//   if ((dec_1.bits[0] | dec_1.bits[1] | dec_1.bits[2] | dec_2.bits[0] |
//        dec_2.bits[1] | dec_2.bits[2]) != 0) {
    s21_decimal zero = {{0, 0, 0, 0}};
    int sign_1 = s21_getSign(dec_1);
    int sign_2 = s21_getSign(dec_2);
    if (s21_getScale(dec_1) != s21_getScale(dec_2)) {
      s21_scaleEqul(&dec_1, &dec_2);
    }
    if (s21_equalWithoutSign(dec_1, zero) == 1 &&
        s21_equalWithoutSign(dec_2, zero) == 1) {
      result = 0;
    } else if (&dec_1 < &dec_2) {
      result = 0;
    } else if (&dec_1 > &dec_2) {
      result = 1;
    } else {
      result = s21_less_without_mod(dec_1, dec_2);
      if (sign_1 && sign_2 && result == 1) {
        result = 0;
      } else if (sign_1 && sign_2 && result == 0) {
        result = 1;
      }
    }
//  }
  return result;
}

int s21_is_greater(s21_decimal dec_1, s21_decimal dec_2) {
  return !s21_is_less(dec_1, dec_2) && !s21_is_equal(dec_1, dec_2);
}

int s21_is_greater_or_equal(s21_decimal dec_1, s21_decimal dec_2) {
  return (!(s21_is_less(dec_1, dec_2))) | s21_is_equal(dec_1, dec_2);
}

int s21_is_less_or_equal(s21_decimal dec_1, s21_decimal dec_2) {
  return s21_is_less(dec_1, dec_2) | s21_is_equal(dec_1, dec_2);
}

int s21_is_not_equal(s21_decimal dec_1, s21_decimal dec_2) {
  return !s21_is_equal(dec_1, dec_2);
}
