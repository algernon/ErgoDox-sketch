/* -*- mode: c++ -*-
 * ErgoDox-Sketch -- algernon's ErgoDox EZ Sketch
 * Copyright (C) 2018  Gergely Nagy
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Kaleidoscope.h"

#include "Kaleidoscope-CycleTimeReport.h"
#include "Kaleidoscope-Escape-OneShot.h"
#include "Kaleidoscope-HostOS.h"
#include "Kaleidoscope/HostOS-select.h"
#include "Kaleidoscope-KeyLogger.h"
#include "Kaleidoscope-Leader.h"
#include "Kaleidoscope-LangPack-Hungarian.h"
#include "Kaleidoscope-Macros.h"
#include "Kaleidoscope-MouseKeys.h"
#include "Kaleidoscope-OneShot.h"
#include "Kaleidoscope-Qukeys.h"
#include "Kaleidoscope-Steno.h"
#include "Kaleidoscope-Syster.h"
#include "Kaleidoscope-TapDance.h"
#include "Kaleidoscope-Unicode.h"

#include "MouseWrapper.h"

#include "keymap.h"

#include "ActiveModStatusLED.h"

KALEIDOSCOPE_INIT_PLUGINS(
  HostOS,
  Qukeys,
  Leader,
  Unicode,
  TapDance,
  OneShot,
  Syster,
  EscapeOneShot,
  ActiveModStatusLED,
  Macros,
  Hungarian,
  MouseKeys,
  CycleTimeReport
);

/* TapDance */

static void TD_TMUX(uint8_t tapCount, byte row, byte col, kaleidoscope::TapDance::ActionType tapDanceAction) {
  if (tapDanceAction != kaleidoscope::TapDance::Release)
    return;

  if (tapCount == 1) {
    Macros.play(MACRO(Tr(LALT(Key_Spacebar))));
  } else {
    Macros.play(MACRO(Tr(LCTRL(Key_A))));
  }
}

static void TD_TMUXPane(uint8_t tapCount, byte row, byte col, kaleidoscope::TapDance::ActionType tapDanceAction) {
  if (tapDanceAction != kaleidoscope::TapDance::Release)
    return;

  if (tapCount == 1) {
    Macros.play(MACRO(Tr(LALT(Key_Spacebar)), Tr(Key_P)));
  } else {
    Macros.play(MACRO(Tr(LALT(Key_Spacebar)), Tr(Key_Z)));
  }
}

void tapDanceAction(uint8_t tapDanceIndex, byte row, byte col, uint8_t tapCount, kaleidoscope::TapDance::ActionType tapDanceAction) {
  switch (tapDanceIndex) {
  case RESET:
    if (tapCount < 4) {
      ErgoDox.setStatusLED(tapCount, true);
    } else {
      ErgoDox.setStatusLED(1, false);
      ErgoDox.setStatusLED(2, false);
      ErgoDox.setStatusLED(3, false);
      ErgoDox.resetDevice();
    }
    return;

  case TMUX:
    return TD_TMUX(tapCount, row, col, tapDanceAction);
  case TMUXP:
    return TD_TMUXPane(tapCount, row, col, tapDanceAction);

  case LPB: {
    if (tapCount < 3) {
      return tapDanceActionKeys(tapCount, tapDanceAction,
                                Key_LeftBracket,
                                Key_LeftParen);
    } else {
      if (tapDanceAction == kaleidoscope::TapDance::Release)
        return Unicode.type(0x300c);
      return;
    }
  }
  case RPB: {
    if (tapCount < 3) {
      return tapDanceActionKeys(tapCount, tapDanceAction,
                                Key_RightBracket,
                                Key_RightParen);
    } else {
      if (tapDanceAction == kaleidoscope::TapDance::Release)
        return Unicode.type(0x300d);
      return;
    }
  }

  case COLON:
    return tapDanceActionKeys(tapCount, tapDanceAction,
                              LSHIFT(Key_Semicolon),
                              Key_Semicolon);
  }
}

/* CycleTimeReport */

void cycleTimeReport(void) {
  Serial.print(F("# average loop time: "));
  Serial.println(CycleTimeReport.average_loop_time);
}

/** MAIN **/

void setup() {
  Serial.begin(9600);

  QUKEYS(
    kaleidoscope::Qukey(0, 0, 2, ShiftToLayer(FN))      // MO(FN)/Tab
  );

  Kaleidoscope.setup();

  MouseWrapper.speedLimit = 64;
  MouseKeys.wheelDelay = 35;
  MouseKeys.accelDelay = 35;

  TapDance.time_out = 100;

  Qukeys.setTimeout(200);
  Qukeys.setReleaseDelay(20);

  /* Boot led animation */
  for (byte i = 1; i < 4; i++) {
    ErgoDox.setStatusLED(i, true);
  }

  for (int i = 255; i > 15; i--) {
    for (byte l = 1; l < 4; l++) {
      ErgoDox.setStatusLEDBrightness(l, i);
    }
    delay(5);
  }

  delay(1000);

  for (int i = 15; i > 0; i--) {
    for (byte l = 1; l < 4; l++) {
      ErgoDox.setStatusLEDBrightness(l, i);
    }
    delay(10);
  }

  for (byte i = 1; i < 4; i++) {
    ErgoDox.setStatusLED(i, false);
  }
}

void loop() {
  Kaleidoscope.loop();
}
