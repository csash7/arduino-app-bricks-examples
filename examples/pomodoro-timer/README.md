# Pomodoro Timer

The **Pomodoro Timer** example turns the UNO Q into a focus assistant. Python® orchestrates alternating focus and break sessions
while the Arduino sketch renders the remaining time on the built-in LED matrix, highlights break periods, and toggles the onboard
LED as a status indicator.

## Description

This application showcases a richer Router Bridge workflow. Python® keeps track of the Pomodoro schedule (25-minute focus, 5-minute
break by default, with an optional long break after four sessions). Every second it sends the remaining minutes and seconds,
colon blink state, progress bar length, and whether the current phase is a break. The microcontroller displays the countdown on
the LED matrix, draws a bottom progress bar, and turns the onboard LED on during breaks.

## Bricks Used

**This example does not use any Bricks.** It demonstrates custom coordination between Python® and Arduino through Router Bridge.

## Hardware and Software Requirements

### Hardware

- Arduino UNO Q (x1)
- USB-C® cable (for power and programming) (x1)

### Software

- Arduino App Lab

## How to Use the Example

1. Optionally customize the session lengths by setting any of these environment variables before running the app:
   - `POMODORO_WORK_MINUTES` (default `25`)
   - `POMODORO_SHORT_BREAK` (default `5`)
   - `POMODORO_LONG_BREAK` (default `15`)
   - `POMODORO_SESSIONS_BEFORE_LONG_BREAK` (default `4`)
2. Run the App from Arduino App Lab.
3. Follow the countdown on the LED matrix. Break phases light the top row and turn the onboard LED on.

## How it Works

During execution the following loop repeats every second:

- **Python® manages the session timeline.** It counts elapsed seconds, switches between focus, short break, and long break
  sessions, and formats the remaining time as `MMSS`.
- **Router Bridge carries the state.** The script calls the `update_timer` function on the microcontroller with the digits,
  colon blink flag, progress column count, and a break indicator.
- **Arduino renders the feedback.** The sketch draws the four digits, keeps the colon blinking, fills the bottom progress bar, and
  lights the top row plus the onboard LED when a break is active.

## Understanding the Code

### 🔧 Backend (`python/main.py`)

- Stores the current session in a dataclass with total and elapsed seconds.
- Alternates between focus and break sessions, including a configurable long break cycle.
- Sends the countdown digits, colon blink state, progress columns, and break flag to the microcontroller via `Bridge.call`.

### 🔧 Hardware (`sketch/sketch.ino`)

- Registers `update_timer` to accept updates from Python®.
- Uses `MatrixDigits::renderFourDigitFrame` to build the LED matrix frame with optional top-row highlighting for breaks.
- Mirrors the break state on the UNO Q built-in LED while continuously updating the LED matrix.
