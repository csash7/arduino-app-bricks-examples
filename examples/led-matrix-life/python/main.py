# SPDX-FileCopyrightText: Copyright (C) 2025 ARDUINO SA <http://www.arduino.cc>
#
# SPDX-License-Identifier: MPL-2.0

import random
import time
from typing import List

from arduino.app_utils import App, Bridge

ROWS = 8
COLS = 13

Grid = List[List[int]]


def random_grid() -> Grid:
    """Create a new random grid with roughly half the cells alive."""
    return [[random.randint(0, 1) for _ in range(COLS)] for _ in range(ROWS)]


def count_neighbors(grid: Grid, row: int, col: int) -> int:
    """Count living neighbors using wrap-around edges."""
    total = 0
    for dr in (-1, 0, 1):
        for dc in (-1, 0, 1):
            if dr == 0 and dc == 0:
                continue
            r = (row + dr) % ROWS
            c = (col + dc) % COLS
            total += grid[r][c]
    return total


def next_generation(grid: Grid) -> Grid:
    """Compute the next grid state following standard Game of Life rules."""
    new_grid = [[0] * COLS for _ in range(ROWS)]
    for r in range(ROWS):
        for c in range(COLS):
            neighbors = count_neighbors(grid, r, c)
            if grid[r][c]:
                new_grid[r][c] = 1 if neighbors in (2, 3) else 0
            else:
                new_grid[r][c] = 1 if neighbors == 3 else 0
    return new_grid


def encode_rows(grid: Grid) -> List[int]:
    """Pack the 8x13 grid into four uint32 words expected by loadFrame."""
    packed: List[int] = []
    for base_row in range(0, ROWS, 2):
        upper = 0
        lower = 0
        for col in range(COLS):
            bit = 1 << (15 - col)
            if grid[base_row][col]:
                upper |= bit
            if grid[base_row + 1][col]:
                lower |= bit
        packed.append((upper << 16) | lower)
    return packed


def is_static(current: Grid, nxt: Grid) -> bool:
    """Return True if the grid stopped changing or went dark."""
    if all(sum(row) == 0 for row in nxt):
        return True
    return current == nxt


def send_frame(grid: Grid) -> None:
    frame = encode_rows(grid)
    Bridge.call("display_frame", frame[0], frame[1], frame[2], frame[3])


def loop_runner():
    state = random_grid()
    send_frame(state)

    while True:
        time.sleep(0.5)
        nxt = next_generation(state)
        if is_static(state, nxt):
            nxt = random_grid()
        send_frame(nxt)
        state = nxt


App.run(user_loop=loop_runner)
