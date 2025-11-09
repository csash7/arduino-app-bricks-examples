# SPDX-FileCopyrightText: Copyright (C) 2025 ARDUINO SA <http://www.arduino.cc>
#
# SPDX-License-Identifier: MPL-2.0

from __future__ import annotations

import os
import time
from dataclasses import dataclass

from arduino.app_utils import *  # noqa: F401,F403

WORK_MINUTES = int(os.environ.get("POMODORO_WORK_MINUTES", "25"))
SHORT_BREAK_MINUTES = int(os.environ.get("POMODORO_SHORT_BREAK", "5"))
LONG_BREAK_MINUTES = int(os.environ.get("POMODORO_LONG_BREAK", "15"))
SESSIONS_BEFORE_LONG_BREAK = int(os.environ.get("POMODORO_SESSIONS_BEFORE_LONG_BREAK", "4"))


@dataclass
class Session:
    name: str
    total_seconds: int
    is_break: bool
    elapsed: int = 0

    @property
    def remaining(self) -> int:
        remaining = self.total_seconds - self.elapsed
        return remaining if remaining > 0 else 0


current_session: Session | None = None
colon_on = True
completed_focus_sessions = 0


def start_session(name: str, minutes: int, is_break: bool) -> None:
    global current_session, colon_on

    seconds = max(1, minutes * 60)
    current_session = Session(name=name, total_seconds=seconds, is_break=is_break)
    colon_on = True
    phase = "Break" if is_break else "Focus"
    print(f"Starting {phase.lower()} session: {minutes} minute(s)")


def select_next_session() -> None:
    global completed_focus_sessions

    if current_session is None:
        start_session("focus", WORK_MINUTES, False)
        return

    if current_session.is_break:
        start_session("focus", WORK_MINUTES, False)
    else:
        completed_focus_sessions += 1
        if completed_focus_sessions % SESSIONS_BEFORE_LONG_BREAK == 0:
            start_session("long_break", LONG_BREAK_MINUTES, True)
        else:
            start_session("short_break", SHORT_BREAK_MINUTES, True)


def _format_digits(remaining: int) -> str:
    minutes = remaining // 60
    seconds = remaining % 60
    return f"{minutes:02d}{seconds:02d}"


def _progress_columns(session: Session) -> int:
    if session.total_seconds <= 0:
        return 12
    progress = (session.elapsed * 12) // session.total_seconds
    if progress > 12:
        progress = 12
    return progress


def loop() -> None:
    global colon_on

    if current_session is None:
        start_session("focus", WORK_MINUTES, False)
        return

    if current_session.elapsed > current_session.total_seconds:
        select_next_session()
        return

    digits = _format_digits(current_session.remaining)
    colon_on = not colon_on
    progress = _progress_columns(current_session)

    Bridge.call("update_timer", digits, colon_on, progress, current_session.is_break)

    current_session.elapsed += 1
    time.sleep(1)


App.run(user_loop=loop)
