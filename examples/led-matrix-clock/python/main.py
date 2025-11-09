# SPDX-FileCopyrightText: Copyright (C) 2025 ARDUINO SA <http://www.arduino.cc>
#
# SPDX-License-Identifier: MPL-2.0

from __future__ import annotations

import os
import time
from datetime import datetime, timezone
from datetime import tzinfo as TzInfo

from arduino.app_utils import *  # noqa: F401,F403

try:
    from zoneinfo import ZoneInfo
except ImportError:  # pragma: no cover - Python < 3.9 fallback
    ZoneInfo = None  # type: ignore

TIMEZONE_ENV = os.environ.get("CLOCK_TIMEZONE", "UTC")


def _detect_timezone() -> TzInfo:
    if ZoneInfo is not None:
        try:
            return ZoneInfo(TIMEZONE_ENV)
        except Exception:
            pass
    local = datetime.now().astimezone().tzinfo
    if local is not None:
        return local
    return timezone.utc


tzinfo = _detect_timezone()
colon_on = True


def loop() -> None:
    global colon_on

    now = datetime.now(tzinfo)
    digits = now.strftime("%H%M")
    colon_on = not colon_on
    seconds = now.second

    Bridge.call("update_time", digits, colon_on, seconds)

    sleep_duration = 1.0 - (now.microsecond / 1_000_000)
    if sleep_duration < 0.05:
        sleep_duration = 0.05
    time.sleep(sleep_duration)


App.run(user_loop=loop)
