// SPDX-FileCopyrightText: Copyright (C) 2025 ARDUINO SA <http://www.arduino.cc>
//
// SPDX-License-Identifier: MPL-2.0

#include <Arduino_LED_Matrix.h>
#include <Arduino_RouterBridge.h>

#include "matrix_digits.h"

Arduino_LED_Matrix matrix;

void update_timer(String digits, bool colon_on, int progress_columns, bool break_mode);

void setup() {
  matrix.begin();
  matrix.clear();

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  Bridge.begin();
  Bridge.provide("update_timer", update_timer);
}

void loop() {
  delay(10);
}

void update_timer(String digits, bool colon_on, int progress_columns, bool break_mode) {
  if (digits.length() != 4) {
    return;
  }

  if (progress_columns < 0) {
    progress_columns = 0;
  }
  if (progress_columns > 12) {
    progress_columns = 12;
  }

  uint32_t frame[4];
  MatrixDigits::renderFourDigitFrame(digits.c_str(), colon_on, static_cast<uint8_t>(progress_columns), break_mode, frame);
  matrix.loadFrame(frame);

  digitalWrite(LED_BUILTIN, break_mode ? LOW : HIGH);
}
