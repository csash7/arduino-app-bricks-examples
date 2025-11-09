// SPDX-FileCopyrightText: Copyright (C) 2025 ARDUINO SA <http://www.arduino.cc>
//
// SPDX-License-Identifier: MPL-2.0

#include <Arduino_LED_Matrix.h>
#include <Arduino_RouterBridge.h>
#include <string.h>

Arduino_LED_Matrix matrix;

static uint16_t frame_rows[8];

static const uint8_t DIGITS[10][8] = {
    {0b111, 0b101, 0b101, 0b101, 0b101, 0b101, 0b111, 0b000}, // 0
    {0b010, 0b110, 0b010, 0b010, 0b010, 0b010, 0b111, 0b000}, // 1
    {0b111, 0b001, 0b001, 0b111, 0b100, 0b100, 0b111, 0b000}, // 2
    {0b111, 0b001, 0b001, 0b111, 0b001, 0b001, 0b111, 0b000}, // 3
    {0b101, 0b101, 0b101, 0b111, 0b001, 0b001, 0b001, 0b000}, // 4
    {0b111, 0b100, 0b100, 0b111, 0b001, 0b001, 0b111, 0b000}, // 5
    {0b111, 0b100, 0b100, 0b111, 0b101, 0b101, 0b111, 0b000}, // 6
    {0b111, 0b001, 0b001, 0b010, 0b010, 0b010, 0b010, 0b000}, // 7
    {0b111, 0b101, 0b101, 0b111, 0b101, 0b101, 0b111, 0b000}, // 8
    {0b111, 0b101, 0b101, 0b111, 0b001, 0b001, 0b111, 0b000}  // 9
};

static const uint8_t DIGIT_OFFSETS[4] = {0, 3, 7, 10};

static inline void clearFrame()
{
  memset(frame_rows, 0, sizeof(frame_rows));
}

static inline void setPixel(uint8_t x, uint8_t y)
{
  if (x >= 13 || y >= 8) {
    return;
  }
  frame_rows[y] |= (1u << (15 - x));
}

static void drawDigit(uint8_t digit, uint8_t offset)
{
  if (digit > 9) {
    return;
  }

  for (uint8_t row = 0; row < 8; ++row) {
    uint8_t pattern = DIGITS[digit][row];
    for (uint8_t col = 0; col < 3; ++col) {
      if (pattern & (1u << (2 - col))) {
        setPixel(offset + col, row);
      }
    }
  }
}

static void drawColon(bool colon_on)
{
  if (!colon_on) {
    return;
  }

  setPixel(6, 2);
  setPixel(6, 5);
}

static void flushFrame()
{
  uint32_t packed[4];
  for (uint8_t i = 0; i < 4; ++i) {
    uint32_t top = ((uint32_t)frame_rows[i * 2]) << 16;
    uint32_t bottom = frame_rows[i * 2 + 1];
    packed[i] = top | bottom;
  }
  matrix.loadFrame(packed);
}

static void update_clock(int hour, int minute, bool colon_on)
{
  const uint8_t digits[4] = {
    static_cast<uint8_t>((hour / 10) % 10),
    static_cast<uint8_t>(hour % 10),
    static_cast<uint8_t>((minute / 10) % 10),
    static_cast<uint8_t>(minute % 10),
  };

  clearFrame();
  for (uint8_t i = 0; i < 4; ++i) {
    drawDigit(digits[i], DIGIT_OFFSETS[i]);
  }
  drawColon(colon_on);
  flushFrame();
}

void setup()
{
  matrix.begin();
  matrix.clear();

  Bridge.begin();
  Bridge.provide("update_clock", update_clock);
}

void loop()
{
  // Rendering is event-driven through Router Bridge callbacks.
}
