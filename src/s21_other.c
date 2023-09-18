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
    s21_setBit(&value, 127, 0);
    // s21_decimal five = {{5, 0, 0, 0}};
    // s21_set_scale(&five, 1);
    // s21_decimal one = {{1, 0, 0, 0}};
    s21_decimal trun = {{0, 0, 0, 0}};
    s21_decimal sub_dec = {{0, 0, 0, 0}};

    s21_truncate(value, &trun);
    if (s21_get_scale(value) != 0) div_by_10(&value, s21_get_scale(value) - 1);
    s21_sub(value, trun, &sub_dec);
    if (sub_dec.bits[0] >= 5) s21_add_1_(&trun);
    *result = trun;
    if (sign)
      result->bits[3] =
          0x80000000;  // зануляем скейл и ставим отрицательный знак
    else
      result->bits[3] = 0;  // зануляем скейл, оставляется положительный знак
  }
  return err;
}

// int main() {
//   s21_decimal src1, src2, result, origin;
//   int value_type_result, value_type_origin;
//   // src1 = -545454512454545.35265454545645;
//   // src2 = 54564654;
//   src1.bits[0] = 0b10000010111000100101101011101101;
//   src1.bits[1] = 0b11111001111010000010010110101101;
//   src1.bits[2] = 0b10110000001111101111000010010100;
//   src1.bits[3] = 0b10000000000011100000000000000000;
//   src2.bits[0] = 0b00000011010000001001011100101110;
//   src2.bits[1] = 0b00000000000000000000000000000000;
//   src2.bits[2] = 0b00000000000000000000000000000000;
//   src2.bits[3] = 0b00000000000000000000000000000000;

//   origin.bits[0] = 0b01111111101000011100001110111111;
//   origin.bits[1] = 0b11111001111010000010010110101101;
//   origin.bits[2] = 0b10110000001111101111000010010100;
//   origin.bits[3] = 0b10000000000011100000000000000000;
//   value_type_result = s21_add(src1, src2, &result);
// }