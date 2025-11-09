# LED Matrix Clock

The **LED Matrix Clock** example turns the UNO Q's built-in LED matrix into a digital clock that updates every second. The Python® runtime keeps track of the current time and sends hour, minute and blink state information to the microcontroller. The microcontroller renders compact digits on the LED matrix and handles colon blinking locally.

This project demonstrates how Python code running on the UNO Q's MPU can continuously push data to the STM32 microcontroller using the Router Bridge.

## Description

- The Python script reads the system clock once per second.
- It calls into the sketch through the Router Bridge to update the displayed time and whether the colon should blink.
- The sketch receives the latest hours, minutes and colon state and draws four 3x7 digits inside the 13x8 LED matrix.
- The colon toggles on every even second, resulting in a traditional digital clock appearance.

## Bricks Used

- `router_bridge`: enables communication between the Python application and the microcontroller sketch.

## Hardware and Software Requirements

### Hardware

- Arduino UNO Q (x1)
- USB-C® cable for power and programming (x1)

### Software

- Arduino App Lab

## How to Use the Example

1. Run the App from Arduino App Lab.
2. The LED matrix immediately shows the current time in 24-hour format.
3. Watch the colon blink every second while the minutes advance automatically.

## How It Works

### Python (`python/main.py`)

- Grabs the current system time using `datetime`.
- Sends the hour, minute and a blinking colon flag to the sketch by calling `Bridge.call("update_clock", hour, minute, colon_on)`.
- Sleeps briefly between updates to keep time and colon blinking in sync.

### Sketch (`sketch/sketch.ino`)

- Provides the `update_clock` function to the Router Bridge so Python can call it.
- Converts each digit into a 3x7 bitmap packed into the format expected by `Arduino_LED_Matrix::loadFrame`.
- Draws the blinking colon and pushes the combined frame to the LED matrix driver.

The example is a compact reference of how to drive the LED matrix from Python-sourced data while keeping the rendering workload on the MCU side.
