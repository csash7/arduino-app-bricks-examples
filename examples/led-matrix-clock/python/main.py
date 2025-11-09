# SPDX-FileCopyrightText: Copyright (C) 2025 ARDUINO SA <http://www.arduino.cc>
#
# SPDX-License-Identifier: MPL-2.0

from datetime import datetime
import time

from arduino.app_utils import App, Bridge

LAST_SENT = None


def send_time_update():
    """Grab the current time and update the LED matrix clock."""
    global LAST_SENT

    now = datetime.now()
    current = (now.hour, now.minute, now.second)

    # Only send an update when the minute or colon blink state changes.
    if current != LAST_SENT:
        colon_on = (now.second % 2) == 0
        Bridge.call("update_clock", now.hour, now.minute, colon_on)
        LAST_SENT = current



def loop():
    send_time_update()
    # Poll 5 times per second so the colon blinking stays responsive.
    time.sleep(0.2)


App.run(user_loop=loop)
