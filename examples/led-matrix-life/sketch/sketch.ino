// SPDX-FileCopyrightText: Copyright (C) 2025 ARDUINO SA <http://www.arduino.cc>
//
// SPDX-License-Identifier: MPL-2.0

#include <Arduino_LED_Matrix.h>
#include <Arduino_RouterBridge.h>

Arduino_LED_Matrix matrix;

void display_frame(uint32_t r0, uint32_t r1, uint32_t r2, uint32_t r3)
{
  const uint32_t frame[4] = {r0, r1, r2, r3};
  matrix.loadFrame(frame);
}

void setup()
{
  matrix.begin();
  matrix.clear();

  Bridge.begin();
  Bridge.provide("display_frame", display_frame);
}

void loop()
{
  // New frames arrive through the Router Bridge.
}
