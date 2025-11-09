// SPDX-FileCopyrightText: Copyright (C) 2025 ARDUINO SA <http://www.arduino.cc>
//
// SPDX-License-Identifier: MPL-2.0

#include <Arduino_LED_Matrix.h>
#include <Arduino_RouterBridge.h>

#include "matrix_digits.h"

Arduino_LED_Matrix matrix;

void update_time(String digits, bool colon_on, int seconds);

void setup() {
  matrix.begin();
  matrix.clear();

  Bridge.begin();
  Bridge.provide("update_time", update_time);
}

void loop() {
  delay(10);
}

void update_time(String digits, bool colon_on, int seconds) {
  if (digits.length() != 4) {
    return;
  }

  if (seconds < 0) {
    seconds = 0;
  }

  uint8_t progress_columns = static_cast<uint8_t>((seconds % 60) * 12 / 60);
  uint32_t frame[4];
  MatrixDigits::renderFourDigitFrame(digits.c_str(), colon_on, progress_columns, false, frame);
  matrix.loadFrame(frame);
}
