#include "s21_decimal.h"

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  s21_ZeroDecimal(dst);
  int res = 1;
  int max_int = 2147483647;
  if (src <= max_int && dst) {
    if (src < 0) {
      s21_setSign(dst, 1);
      src = -src;
    }
    dst->bits[0] = src;
    res = 0;
  }
  return res;
}

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  int res = 1;
  s21_truncate(src, &src);
  if (src.bits[1] == 0 && src.bits[2] == 0 && dst) {
    *dst = src.bits[0];
    if (s21_getSign(src)) {
      *dst = -*dst;
    }
    int scale = s21_getScale(src);
    while (scale) {
      *dst = *dst / 10;
      scale--;
    }
    res = 0;
  }
  return res;
}

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  int res = 1;
  if (dst) {
    double result = 0;
    for (int i = 0; i < 96; i++) {
      if (s21_getBit(src, i) == 1) result = result + pow(2, i);
    }
    int scale = s21_getScale(src);
    while (scale) {
      result = result / 10;
      scale--;
    }
    *dst = (float)result;
    if (s21_getSign(src)) {
      *dst = *dst * (-1);
    }
    res = 0;
  }
  return res;
}

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  s21_ZeroDecimal(dst);
  int res = 0;
  if (dst == NULL || src != src)
    res = 1;
  else if ((fabsf(src) < 1e-28) || (fabsf(src) >= powf(2, 96)))
    res = 1;
  else {
    int scale = 0;
    double number = fabs(src);
    while (number < 1.0 && scale < 28) {
      number *= 10;
      scale++;
    }
    while (number / 1e7 < 1.0 && scale < 28) {
      number *= 10;
      scale++;
    }
    number = floor(number);
    if (fmod(number, 10.0) >= 5) {
      number += 10 - fmod(number, 10.0);
      number = round(number);
    }
    if (scale > 0) {
      number /= 10;
      scale--;
    }
    while (scale > 0 && (fmod(number, 10.0) < 1e-8)) {
      number /= 10;
      number = round(number);
      scale--;
    }
    for (int i = 0; i < 96; i++) {
      s21_setBit(dst, i, s21_GetBitToFloat(number, i));
    }
    if (src < 0) {
      s21_setSign(dst, 1);
    }
    s21_setScale(dst, scale);
  }
  return res;
}