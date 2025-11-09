# LED Matrix Game of Life

The **LED Matrix Game of Life** example runs Conway's Game of Life on the UNO Q. A Python® script simulates the cellular automaton while the microcontroller renders each generation on the 13x8 LED matrix.

This project showcases how to stream continuously changing data from Linux to the microcontroller using the Router Bridge.

## Description

- Python maintains a 13×8 grid of cells and updates it according to the classic Game of Life rules.
- After every generation the script encodes the matrix into four 32-bit values and sends them to the sketch through the Router Bridge.
- The sketch receives the packed rows and updates the LED matrix immediately.
- When the pattern stabilizes or all cells die out, the Python script seeds the grid with a fresh random layout to keep the animation moving.

## Bricks Used

- `router_bridge`: enables the bidirectional communication between the Python runtime and the microcontroller firmware.

## Hardware and Software Requirements

### Hardware

- Arduino UNO Q (x1)
- USB-C® cable for power and programming (x1)

### Software

- Arduino App Lab

## How to Use the Example

1. Run the App from Arduino App Lab.
2. The LED matrix fills with a random Game of Life pattern.
3. Watch new generations evolve roughly twice per second. When the pattern settles, the system automatically seeds a new random starting state.

## How It Works

### Python (`python/main.py`)

- Computes the next generation using a toroidal (wrap-around) grid for smoother behavior on the small display.
- Encodes two rows into a `uint32_t` following the same format expected by `Arduino_LED_Matrix::loadFrame` and sends four packed words to the sketch via `Bridge.call("display_frame", ...)`.
- Detects still lifes or empty patterns and reseeds the grid to keep the animation lively.

### Sketch (`sketch/sketch.ino`)

- Provides the `display_frame` function that accepts four packed `uint32_t` values from Python.
- Directly hands the frame to the LED matrix driver to paint the latest generation.

The example highlights how to offload compute-heavy work (simulation) to Python while letting the microcontroller handle real-time LED updates.
