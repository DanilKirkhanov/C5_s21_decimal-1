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
int s21_GetBitToFloat(double number, int bit_number) {
  int result = -1;
  for (int i = 0; i < bit_number; i++) number = floor(number / 2);
  result = (int)fmod(number, 2.0);
  return result;
}

void s21_scaleEqul(s21_decimal *value_1, s21_decimal *value_2) {
  int scale_diff = s21_getScale(*value_1) - s21_getScale(*value_2);
  if (scale_diff > 0) {
    for (int i = 0; i < scale_diff; i++) {
      *value_2 = s21_div_integer(*value_2, 10);
    }
    s21_setScale(value_2, s21_getScale(*value_1));
  } else if (scale_diff < 0) {
    for (int i = 0; i < -scale_diff; i++) {
      *value_1 = s21_div_integer(*value_1, 10);
    }
    s21_setScale(value_1, s21_getScale(*value_2));
  }
}

int s21_equalWithoutSign(s21_decimal value_1, s21_decimal value_2) {
  int res;
  if (value_1.bits[2] != value_2.bits[2]) {
    res = 0;
  } else if (value_1.bits[1] != value_2.bits[1]) {
    res = 0;
  } else if (value_1.bits[0] != value_2.bits[0]) {
    res = 0;
  } else {
    res = 1;
  }
  return res;
}

int s21_less_without_mod(s21_decimal dec_1, s21_decimal dec_2) {
  int result;
  if (dec_1.bits[0] < dec_2.bits[0]) {
    result = 1;
  } else if (dec_1.bits[0] > dec_2.bits[0]) {
    result = 0;
  } else {
    if (dec_1.bits[1] < dec_2.bits[1]) {
      result = 1;
    } else if (dec_1.bits[1] > dec_2.bits[1]) {
      result = 0;
    } else {
      if (dec_1.bits[2] < dec_2.bits[2]) {
        result = 1;
      } else if (dec_1.bits[2] > dec_2.bits[2]) {
        result = 0;
      } else {
        result = 0;
      }
    }
  }
  return result;
}

s21_decimal s21_div_integer(s21_decimal src, int div) {
  s21_decimal res = {{0, 0, 0, 0}};
  // s21_null_decimal(&res);
  long unsigned int remainder = src.bits[2] % div;  // остаток
  res.bits[2] = src.bits[2] / div;
  for (int i = 1; i >= 0; i--) {
    long unsigned int tempbit =
        (long unsigned int)src.bits[i] +
        (remainder * pow(2, 32));  // временный младший бит чтобы не было
                                   // переполнения при добавлении остатка
    remainder = tempbit % div;
    tempbit -= remainder;  // округяем чтобы при делении на 10 число не
                           // округлилось до большего
    res.bits[i] = tempbit / div;
  }
  res.bits[0] += remainder / div;
  res.bits[3] = src.bits[3];
  return res;
}
