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

#include "00-config.h"

#include "Kaleidoscope.h"

#if WITH_CYCLETIMEREPORT
#include "Kaleidoscope-CycleTimeReport.h"
#endif
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

#include "StatusLEDDance.h"

KALEIDOSCOPE_INIT_PLUGINS(
  HostOS,
  Qukeys,
  Leader,
  Unicode,
  TapDance,
  OneShot,
  Syster,
  EscapeOneShot,
  StatusLEDDance,
  Macros,
  Hungarian,
  MouseKeys
#if WITH_CYCLETIMEREPORT
  ,CycleTimeReport
#endif
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
    if (tapDanceAction == kaleidoscope::TapDance::Interrupt ||
        tapDanceAction == kaleidoscope::TapDance::Timeout ||
        tapCount >= 4) {
      StatusLEDDance.disabled = false;
      ErgoDox.setStatusLED(1, false);
      ErgoDox.setStatusLED(2, false);
      ErgoDox.setStatusLED(3, false);

      if (tapCount >= 4)
        ErgoDox.resetDevice();

      return;
    }

    if (tapCount < 4 && tapDanceAction != kaleidoscope::TapDance::Release) {
      StatusLEDDance.disabled = true;
      ErgoDox.setStatusLED(tapCount, true);
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

#if WITH_CYCLETIMEREPORT
/* CycleTimeReport */

void cycleTimeReport(void) {
  Serial.print(F("# average loop time: "));
  Serial.println(CycleTimeReport.average_loop_time);
}
#endif

/* Symbolic Unicode Input */

static const struct {
  const char *symbol;
  uint32_t code;
} symbol_map[] PROGMEM = {
  {"coffee", 0x2615},
  {"lambda", 0x03bb},
  {"poop", 0x1f4a9},
  {"rofl", 0x1f923},
  {"kiss", 0x1f619},
  {"snowman", 0x2603},
  {"heart", 0x2764},
  {"bolt", 0x26a1},
  {"pi", 0x03c0},
  {"mouse", 0x1f401},
  {"micro", 0x00b5},
  {"tm", 0x2122},
  {"family", 0x1f46a},
  {"child", 0x1f476},
};

void typeString(const char *str) {
  Unicode.start();

  for (uint8_t i = 0; str[i]; i++) {
    const char c = str[i];
    Key key = Key_NoKey;

    switch (c) {
    case 'a' ... 'z':
      key.keyCode = Key_A.keyCode + (c - 'a');
      break;
    case 'A' ... 'Z':
      key.keyCode = Key_A.keyCode + (c - 'A');
      break;
    case '1' ... '9':
      key.keyCode = Key_1.keyCode + (c - '1');
      break;
    case '0':
      key.keyCode = Key_0.keyCode;
      break;
    }

    Unicode.input();
    handleKeyswitchEvent(key, 255, 255, IS_PRESSED | INJECTED);
    kaleidoscope::hid::sendKeyboardReport();
    Unicode.input();
    handleKeyswitchEvent(key, 255, 255, WAS_PRESSED | INJECTED);
    kaleidoscope::hid::sendKeyboardReport();
  }
  Unicode.end();
}

void SymUnI_input(const char *symbol) {
  uint32_t code = 0;

  for (uint8_t i = 0; i < sizeof(symbol_map) / sizeof(symbol_map[0]); i++) {
    const char *map_symbol = (const char *)pgm_read_word(&symbol_map[i].symbol);
    if (strcmp(symbol, map_symbol) == 0) {
      code = pgm_read_dword(&symbol_map[i].code);
      break;
    }
  }

  if (code)
    Unicode.type(code);
  else
    typeString(symbol);
}

void systerAction(kaleidoscope::Syster::action_t action, const char *symbol) {
  switch (action) {
  case kaleidoscope::Syster::StartAction:
    Unicode.type(0x2328);
    break;
  case kaleidoscope::Syster::EndAction:
    handleKeyswitchEvent(Key_Backspace, 255, 255, IS_PRESSED | INJECTED);
    kaleidoscope::hid::sendKeyboardReport();
    handleKeyswitchEvent(Key_Backspace, 255, 255, WAS_PRESSED | INJECTED);
    kaleidoscope::hid::sendKeyboardReport();
    break;
  case kaleidoscope::Syster::SymbolAction:
    SymUnI_input(symbol);
    break;
  }
}

/* Leader */

enum {
  LEAD_UNICODE_UCIS,

  LEAD_CSILLA,
  LEAD_KIDS,
  LEAD_GERGO,
  LEAD_SHRUGGY,

  LEAD_BUTTERFLY,
  LEAD_GUI_HELPER,
  LEAD_GUI,
  LEAD_COMPOSE,
};

static void Shruggy(uint8_t seqIndex) {
  ::Unicode.type(0xaf);
  ::Macros.play(MACRO(Tc(Backslash),
                      D(RightShift),
                      Tc(Minus),
                      Tc(9),
                      U(RightShift)));
  ::Unicode.type(0x30c4);
  ::Macros.play(MACRO(D(RightShift),
                      Tc(0),
                      Tc(Minus),
                      U(RightShift),
                      Tc(Slash)));
  ::Unicode.type(0xaf);
}

static void startUCIS(uint8_t seqIndex) {
  handleKeyswitchEvent(SYSTER, 255, 255, IS_PRESSED | INJECTED);
}

static void Kids(uint8_t seqIndex) {
  ::Macros.play(MACRO(Tc(Spacebar), Tr(LSHIFT(Key_7)), Tc(Spacebar)));
  ::Unicode.type(0x1f476);
  ::Unicode.type(0x1f476);
}

static void Butterfly(uint8_t seqIndex) {
  ::Macros.play(MACRO(I(10),
                      D(LeftAlt), W(100), Tc(X), W(100), U(LeftAlt),
                      W(100), W(100),
                      Tc(B), Tc(U), Tc(T), Tc(T), Tc(E), Tc(R), Tc(F), Tc(L), Tc(Y),
                      W(100), W(100),
                      Tc(Enter), W(100),
                      Tc(Y)));
}

static void GUI(uint8_t seqIndex) {
  ::OneShot.inject(OSL(APPSEL), IS_PRESSED);
  ::OneShot.inject(OSL(APPSEL), WAS_PRESSED);
  Serial.println(F("appsel:start"));
}

static void Compose(uint8_t seqIndex) {
  ::Macros.play(MACRO(T(RightAlt)));
}

static void GUIHelper(uint8_t seqIndex) {
  Serial.println(F("appsel:helper"));
}

static void Csilla(uint8_t seqIndex) {
  ::Macros.play(MACRO(Tr(LSHIFT(Key_C)),
                      Tc(S),
                      Tc(I),
                      Tc(L),
                      Tc(L)));

  handleKeyswitchEvent((Key) {
    .raw = kaleidoscope::language::HUN_AA
  }, UNKNOWN_KEYSWITCH_LOCATION, IS_PRESSED | INJECTED);

  ::Macros.play(MACRO(Tc(M),
                      Tc(A),
                      Tc(S),
                      Tc(S),
                      Tc(Z),
                      Tc(O),
                      Tc(N),
                      Tc(Y),
                      Tc(K)));

  handleKeyswitchEvent((Key) {
    .raw = kaleidoscope::language::HUN_AA
  }, UNKNOWN_KEYSWITCH_LOCATION, IS_PRESSED | INJECTED);

  ::Macros.play(MACRO(Tc(M)));
}

static void Gergo(uint8_t seqIndex) {
  ::Macros.play(MACRO(Tr(LSHIFT(Key_G)),
                      Tc(E),
                      Tc(J),
                      Tc(G)));

  handleKeyswitchEvent((Key) {
    .raw = kaleidoscope::language::HUN_ODA
  }, UNKNOWN_KEYSWITCH_LOCATION, IS_PRESSED | INJECTED);
  Keyboard.sendReport();
  handleKeyswitchEvent((Key) {
    .raw = kaleidoscope::language::HUN_ODA
  }, UNKNOWN_KEYSWITCH_LOCATION, IS_PRESSED | INJECTED);
  Keyboard.sendReport();
  handleKeyswitchEvent((Key) {
    .raw = kaleidoscope::language::HUN_ODA
  }, UNKNOWN_KEYSWITCH_LOCATION, IS_PRESSED | INJECTED);
}

static const kaleidoscope::Leader::dictionary_t dictionary[] PROGMEM = LEADER_DICT
    ([LEAD_UNICODE_UCIS]   = {LEADER_SEQ(LEAD(0), Key_U), startUCIS},

     [LEAD_CSILLA]          = {LEADER_SEQ(LEAD(0), Key_C), Csilla},
     [LEAD_KIDS]            = {LEADER_SEQ(LEAD(0), Key_K), Kids},
     [LEAD_GERGO]           = {LEADER_SEQ(LEAD(0), Key_G), Gergo},
     [LEAD_SHRUGGY]         = {LEADER_SEQ(LEAD(0), Key_S), Shruggy},

     [LEAD_BUTTERFLY]       = {LEADER_SEQ(LEAD(0), OSM(LeftAlt)), Butterfly},
     [LEAD_GUI_HELPER]      = {LEADER_SEQ(LEAD(0), Key_Enter, Key_LeftGui), GUIHelper},
     [LEAD_GUI]             = {LEADER_SEQ(LEAD(0), Key_LeftGui), GUI},
     [LEAD_COMPOSE]         = {LEADER_SEQ(LEAD(0), Key_R), Compose});

/** MAIN **/

void bootStartAnimation() {
  ErgoDox.setStatusLED(1, false);
  ErgoDox.setStatusLED(2, false);
  ErgoDox.setStatusLED(3, false);
  ErgoDox.setStatusLEDBrightness(1, 15);
  ErgoDox.setStatusLEDBrightness(2, 15);
  ErgoDox.setStatusLEDBrightness(3, 15);

  ErgoDox.setStatusLED(1, true);
  delay(50);
  ErgoDox.setStatusLED(2, true);
  delay(50);
  ErgoDox.setStatusLED(3, true);
  delay(50);

  ErgoDox.setStatusLED(1, false);
  delay(50);
  ErgoDox.setStatusLED(2, false);
  delay(50);
  ErgoDox.setStatusLED(3, false);
  delay(50);
}

void bootFinishedAnimation() {
  for (byte i = 1; i < 4; i++) {
    ErgoDox.setStatusLED(i, true);
  }

  for (int i = 255; i > 15; i -= 2) {
    for (byte l = 1; l < 4; l++) {
      ErgoDox.setStatusLEDBrightness(l, i);
    }
    delay(5);
  }

  delay(500);

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

void setup() {
  bootStartAnimation();

  Serial.begin(9600);

  QUKEYS(
    kaleidoscope::Qukey(0, 0, 2, ShiftToLayer(FN))      // MO(FN)/Tab
  );

  Kaleidoscope.setup();

  MouseKeys.speed = 2;
  MouseWrapper.speedLimit = 64;
  MouseKeys.wheelDelay = 35;
  MouseKeys.accelDelay = 15;

  ErgoDox.debounce = 3;

  TapDance.time_out = 100;

  Qukeys.setTimeout(200);
  Qukeys.setReleaseDelay(20);

  Leader.dictionary = dictionary;

  OneShot.time_out = 500;

  bootFinishedAnimation();
}

void loop() {
  Kaleidoscope.loop();
}
