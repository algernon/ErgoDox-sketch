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

#include "StatusLEDDance.h"
#include "layers.h"

namespace kaleidoscope {

bool StatusLEDDance::disabled;

void StatusLEDDance::setStatusLEDByModifier(uint8_t led, Key modifier) {
  if (hid::isModifierKeyActive(modifier))
    ErgoDox.setStatusLED(led, true);
  else
    ErgoDox.setStatusLED(led, false);
}

EventHandlerResult StatusLEDDance::beforeReportingState() {
  if (disabled)
    return EventHandlerResult::OK;

  bool layerColor = false;

  if (Layer.isOn(HUN)) {
    ErgoDox.setStatusLED(2, true);
    ErgoDox.setStatusLED(3, true);
    layerColor = true;
  }

  if (Layer.isOn(FN)) {
    ErgoDox.setStatusLED(1, true);
    ErgoDox.setStatusLED(3, true);
    layerColor = true;
  }

  if (Layer.isOn(NUMPAD)) {
    ErgoDox.setStatusLED(1, true);
    ErgoDox.setStatusLED(2, true);
    layerColor = true;
  }

  if (Layer.isOn(APPSEL)) {
    ErgoDox.setStatusLED(1, true);
    ErgoDox.setStatusLED(2, true);
    ErgoDox.setStatusLED(3, true);
    layerColor = true;
  }

  if (layerColor)
    return EventHandlerResult::OK;

  setStatusLEDByModifier(1, Key_LeftShift);
  setStatusLEDByModifier(2, Key_LeftAlt);
  setStatusLEDByModifier(3, Key_LeftControl);

  return EventHandlerResult::OK;
}
}

kaleidoscope::StatusLEDDance StatusLEDDance;
