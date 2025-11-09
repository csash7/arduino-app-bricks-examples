# LED Matrix Clock

The **LED Matrix Clock** example turns the UNO Q LED matrix into a minimalistic digital clock. Python® keeps the current time and
sends updates to the microcontroller through Router Bridge. The Arduino sketch renders a four-digit time with a blinking colon
and a seconds progress bar directly on the built-in 12×8 LED matrix.

## Description

The application combines Python® date handling with custom LED matrix rendering on the microcontroller. Python® reads the system
clock using a configurable timezone, toggles the colon every second, and reports the current seconds count. The Arduino sketch
receives the four digits and renders them using a compact 7-segment inspired font. A progress bar at the bottom of the matrix
tracks the passing seconds inside the current minute.

## Bricks Used

**This example does not use any Bricks.** It demonstrates direct Router Bridge communication between Python® and Arduino.

## Hardware and Software Requirements

### Hardware

- Arduino UNO Q (x1)
- USB-C® cable (for power and programming) (x1)

### Software

- Arduino App Lab

**Tip:** You can also run this example with the UNO Q in SBC mode using a [USB-C® hub](https://store.arduino.cc/products/usb-c-to-hdmi-multiport-adapter-with-ethernet-and-usb-hub),
mouse, keyboard, and display.

## How to Use the Example

1. Optionally set the `CLOCK_TIMEZONE` environment variable (for example `Europe/Rome`) before running the app.
2. Run the App from Arduino App Lab.
3. Watch the LED matrix show the current time with a blinking colon and a bottom-row seconds bar.

## How it Works

Once the application is running, the device performs the following operations every second:

- **Python® tracks the time zone aware clock.** The script reads the system clock using the provided timezone (default `UTC`) and
  prepares the four digits along with a colon blink flag and the seconds count.
- **Router Bridge transfers the update.** The Python® loop calls the `update_time` function exposed by the Arduino sketch and
  passes the digits, colon state, and seconds.
- **Arduino renders the matrix.** The sketch builds a bitmap for the four digits, overlays the colon across the gap between the
  hour and minute digits, and lights up the bottom row proportionally to the seconds value before loading the frame into the LED
  matrix driver.

## Understanding the Code

### 🔧 Backend (`python/main.py`)

- Detects the timezone using `zoneinfo` when available and falls back to the board's local timezone.
- Formats the time as `HHMM`, toggles the colon bit every loop, and sends the digits, colon flag, and seconds via `Bridge.call`.
- Sleeps just enough to align with the next second boundary for smooth updates.

### 🔧 Hardware (`sketch/sketch.ino`)

- Registers the `update_time` Router Bridge function during `setup()`.
- Converts the incoming `HHMM` string into a bitmap using `MatrixDigits::renderFourDigitFrame`.
- Uses the helper to render 7-segment style digits, blink the colon, and draw a seconds progress bar on the bottom row before
  calling `matrix.loadFrame()`.
