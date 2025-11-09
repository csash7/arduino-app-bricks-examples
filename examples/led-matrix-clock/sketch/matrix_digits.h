/*
 * SPDX-FileCopyrightText: Copyright (C) 2025 ARDUINO SA <http://www.arduino.cc>
 *
 * SPDX-License-Identifier: MPL-2.0
 */

#pragma once

#include <stdint.h>

namespace MatrixDigits {

constexpr uint8_t SEG_A = 1 << 0;
constexpr uint8_t SEG_B = 1 << 1;
constexpr uint8_t SEG_C = 1 << 2;
constexpr uint8_t SEG_D = 1 << 3;
constexpr uint8_t SEG_E = 1 << 4;
constexpr uint8_t SEG_F = 1 << 5;
constexpr uint8_t SEG_G = 1 << 6;

constexpr uint8_t DIGIT_SEGMENTS[10] = {
    SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,              // 0
    SEG_B | SEG_C,                                              // 1
    SEG_A | SEG_B | SEG_D | SEG_E | SEG_G,                      // 2
    SEG_A | SEG_B | SEG_C | SEG_D | SEG_G,                      // 3
    SEG_F | SEG_G | SEG_B | SEG_C,                              // 4
    SEG_A | SEG_F | SEG_G | SEG_C | SEG_D,                      // 5
    SEG_A | SEG_F | SEG_E | SEG_D | SEG_C | SEG_G,              // 6
    SEG_A | SEG_B | SEG_C,                                      // 7
    SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G,      // 8
    SEG_A | SEG_B | SEG_C | SEG_D | SEG_F | SEG_G,              // 9
};

inline void setPixel(uint8_t &column, uint8_t row) {
    if (row < 8) {
        column |= static_cast<uint8_t>(1U << (7 - row));
    }
}

inline void addHorizontal(uint8_t (&columns)[3], uint8_t row) {
    setPixel(columns[0], row);
    setPixel(columns[1], row);
    setPixel(columns[2], row);
}

inline void addVertical(uint8_t (&columns)[3], uint8_t columnIndex, uint8_t rowStart, uint8_t rowEnd) {
    for (uint8_t row = rowStart; row <= rowEnd; ++row) {
        setPixel(columns[columnIndex], row);
    }
}

inline void renderDigit(uint8_t value, uint8_t (&columns)[3]) {
    columns[0] = 0;
    columns[1] = 0;
    columns[2] = 0;

    if (value > 9) {
        return;
    }

    uint8_t segments = DIGIT_SEGMENTS[value];

    if (segments & SEG_A) {
        addHorizontal(columns, 1);
    }
    if (segments & SEG_F) {
        addVertical(columns, 0, 2, 3);
    }
    if (segments & SEG_B) {
        addVertical(columns, 2, 2, 3);
    }
    if (segments & SEG_G) {
        addHorizontal(columns, 4);
    }
    if (segments & SEG_E) {
        addVertical(columns, 0, 5, 6);
    }
    if (segments & SEG_C) {
        addVertical(columns, 2, 5, 6);
    }
    if (segments & SEG_D) {
        addHorizontal(columns, 7);
    }
}

inline void addColon(uint8_t &column) {
    setPixel(column, 3);
    setPixel(column, 5);
}

inline void renderFourDigitFrame(const char *digits,
                                 bool colonOn,
                                 uint8_t progressColumns,
                                 bool highlightTop,
                                 uint32_t (&frame)[4]) {
    uint8_t columns[12] = {0};

    for (uint8_t i = 0; i < 4; ++i) {
        char ch = digits[i];
        if (ch < '0' || ch > '9') {
            continue;
        }
        uint8_t digitColumns[3];
        renderDigit(static_cast<uint8_t>(ch - '0'), digitColumns);
        uint8_t baseIndex = i * 3;
        columns[baseIndex + 0] = digitColumns[0];
        columns[baseIndex + 1] = digitColumns[1];
        columns[baseIndex + 2] = digitColumns[2];
    }

    if (colonOn) {
        addColon(columns[5]);
        addColon(columns[6]);
    }

    if (highlightTop) {
        for (uint8_t col = 0; col < 12; ++col) {
            setPixel(columns[col], 0);
        }
    }

    if (progressColumns > 12) {
        progressColumns = 12;
    }
    for (uint8_t col = 0; col < progressColumns; ++col) {
        setPixel(columns[col], 7);
    }

    for (uint8_t group = 0; group < 4; ++group) {
        frame[group] = (static_cast<uint32_t>(columns[group * 3 + 0]) << 24) |
                       (static_cast<uint32_t>(columns[group * 3 + 1]) << 16) |
                       (static_cast<uint32_t>(columns[group * 3 + 2]) << 8);
    }
}

}  // namespace MatrixDigits

