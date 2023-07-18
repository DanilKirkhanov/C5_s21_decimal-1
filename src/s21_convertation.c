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
  // truncate!!!
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
  if ((dst == NULL || src != src) || (fabs(src) < powl(10.0, -28.0)) ||
      (fabs(src) >= powl(2.0, 96.0))) {
    res = 1;
  } else {
    double number = fabs((double)src);
    int scale = 0;
    // while (scale < 28 && number < 1.0) {
    //   number = number * 10;
    //   scale++;
    // }
      while (number / pow(10.0, 7) < 1.0 && scale < 28) {
      number *= 10;
      scale++;
    }
      if (fmod(floor(number), 10.0) >= 5) {
      number += 10 - fmod(number, 10.0);
      number = round(number);
    }
      if (scale > 0) {
      number /= 10;
      scale--;
    }
      while (scale > 0 && (fmod(number, 10.0) < pow(10.0, -8))) {
      number /= 10;
      number = round(number);
      scale--;
    }
    for (int i = 0; i < 96; i++)
    s21_setBit(dst, i, s21_getBitToFloat(number, i));
    if (src < 0) s21_setSign(dst, 1);
    s21_setScale(dst, scale);
  }
  return res;
}

int s21_getBit(s21_decimal num, int place) {
  unsigned int mask = 1u << (place % 32);
  int result = (num.bits[place / 32] & mask) != 0;
  return result;
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

void s21_setScale(s21_decimal *num, int exp) {
  int mask = exp << 16;
  num->bits[3] = mask;
}

int s21_getScale(s21_decimal dst) {
  int mask = 127 << 16;
  int scale = (dst.bits[3] & mask) >> 16;
  return scale;
}

void s21_setScaleToZero(s21_decimal *number) {
  int firstElement = (number->bits[0]);
  int scaleMask = 0xFF0000;
  firstElement = firstElement & (~scaleMask);
}

void s21_ZeroDecimal(s21_decimal *dst) {
  dst->bits[0] = 0;
  dst->bits[1] = 0;
  dst->bits[2] = 0;
  dst->bits[3] = 0;
}
