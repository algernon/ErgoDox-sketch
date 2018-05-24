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

#pragma once

#include "layers.h"

enum {
  LPB,
  RPB,
  COLON,
  RESET,
  TMUX,
  TMUXP,
  ARRW,
  MNP,
};

enum {
  FX,
  PLOVER,
};

#define Key_AT    LSHIFT(Key_2)
#define Key_STAR  LSHIFT(Key_8)
#define Key_DOLLR LSHIFT(Key_4)
#define Key_CARET LSHIFT(Key_6)
#define Key_PRCNT LSHIFT(Key_5)
#define Key_EXCLM LSHIFT(Key_1)
#define Key_HASH  LSHIFT(Key_3)
#define Key_AND   LSHIFT(Key_7)
#define Key_LCB   LSHIFT(Key_LeftBracket)
#define Key_RCB   LSHIFT(Key_RightBracket)

#define Key_Underscore LSHIFT(Key_Minus)

#define Key_PlayPause Consumer_PlaySlashPause

#define R(n) (Key){.raw = kaleidoscope::language::n}
#define MW(d) Key_mouseWarp ## d
#define MM(d) Key_mouse ## d
#define MS(d) MM(Scroll ## d)

/* *INDENT-OFF* */
KEYMAPS(
  [DVORAK] = KEYMAP_STACKED
  (
      // left hand
       TD(MNP)         ,Key_LCB       ,Key_AT    ,Key_STAR      ,Key_DOLLR ,Key_CARET ,Key_F11
      ,Key_Backtick    ,Key_Quote     ,Key_Comma ,Key_Period    ,Key_P     ,Key_Y     ,TD(LPB)
      ,Key_Tab         ,Key_A         ,Key_O     ,Key_E         ,Key_U     ,Key_I
      ,Key_PlayPause   ,Key_Slash     ,Key_Q     ,Key_J         ,Key_K     ,Key_X     ,TD(TMUX)
      ,XXX             ,XXX           ,XXX       ,XXX           ,TD(COLON)

                        ,OSM(LeftAlt)   ,Key_LeftGui
                                        ,OSM(LeftControl)
      ,Key_Backspace    ,OSM(LeftShift) ,Key_Esc

      // right hand
      ,XXX             ,Key_PRCNT     ,Key_EXCLM ,Key_HASH ,Key_AND    ,Key_RCB       ,XXX
      ,TD(RPB)         ,Key_F         ,Key_G     ,Key_C    ,Key_R      ,Key_L         ,Key_Backslash
                       ,Key_D         ,Key_H     ,Key_T    ,Key_N      ,Key_S         ,Key_Equals
      ,TD(TMUXP)       ,Key_B         ,Key_M     ,Key_W    ,Key_V      ,Key_Z         ,TD(RESET)
      ,Key_Minus       ,XXX           ,XXX       ,XXX      ,XXX

      ,OSL(FN)  ,OSL(NUMPAD)
      ,OSL(HUN)
      ,LEAD(0)  ,Key_Enter         ,Key_Space
  ),

  [HUN] = KEYMAP_STACKED
  (
      // left hand
       XXX             ,XXX           ,XXX        ,XXX           ,XXX        ,XXX       ,XXX
      ,XXX             ,XXX           ,R(HUN_ODA) ,XXX           ,R(HUN_UDA) ,XXX       ,XXX
      ,XXX             ,R(HUN_AA)     ,R(HUN_OA)  ,R(HUN_EA)     ,R(HUN_UA)  ,R(HUN_IA)
      ,XXX             ,XXX           ,R(HUN_OU)  ,XXX           ,R(HUN_UU)  ,XXX       ,XXX
      ,XXX             ,XXX           ,XXX        ,XXX           ,XXX

                       ,XXX            ,XXX
                                       ,XXX
      ,XXX             ,___            ,___

      // right hand
      ,XXX             ,XXX           ,XXX       ,XXX      ,XXX        ,XXX           ,XXX
      ,XXX             ,XXX           ,XXX       ,XXX      ,XXX        ,XXX           ,XXX
                       ,XXX           ,XXX       ,XXX      ,XXX        ,XXX           ,XXX
      ,XXX             ,XXX           ,XXX       ,XXX      ,XXX        ,XXX           ,XXX
      ,XXX             ,XXX           ,XXX       ,XXX      ,XXX

      ,XXX      ,XXX
      ,___
      ,XXX      ,XXX        ,XXX
  ),

  [NUMPAD] = KEYMAP_STACKED
  (
      // left hand
       XXX             ,XXX           ,XXX       ,XXX           ,XXX       ,XXX       ,XXX
      ,XXX             ,XXX           ,XXX       ,XXX           ,XXX       ,XXX       ,XXX
      ,XXX             ,XXX           ,XXX       ,XXX           ,XXX       ,XXX
      ,XXX             ,XXX           ,XXX       ,XXX           ,XXX       ,XXX       ,XXX
      ,XXX             ,XXX           ,XXX       ,XXX           ,XXX

                       ,___           ,XXX
                                      ,XXX
      ,___             ,___           ,___

      // right hand
      ,XXX             ,XXX           ,XXX       ,XXX      ,XXX        ,XXX           ,XXX
      ,XXX             ,XXX           ,Key_1     ,Key_4    ,Key_7      ,XXX           ,XXX
                       ,Key_0         ,Key_2     ,Key_5    ,Key_8      ,Key_Minus     ,XXX
      ,Key_Underscore  ,Key_Period    ,Key_3     ,Key_6    ,Key_9      ,XXX           ,XXX
      ,XXX             ,XXX           ,XXX       ,XXX      ,XXX

      ,XXX      ,___
      ,XXX
      ,XXX      ,XXX        ,XXX
  ),

  [FN] = KEYMAP_STACKED
  (
      // left hand
       XXX             ,Key_F9        ,Key_F7    ,Key_F5        ,Key_F3    ,Key_F1    ,XXX
      ,XXX             ,MS(L)         ,MW(NW)    ,MM(Up)        ,MW(NE)    ,MS(Up)    ,MW(End)
      ,___             ,MS(R)         ,MM(L)     ,MM(Dn)        ,MM(R)     ,MS(Dn)
      ,XXX             ,XXX           ,MW(SW)    ,XXX           ,MW(SE)    ,XXX       ,MM(BtnL)
      ,XXX             ,XXX           ,XXX       ,XXX           ,XXX

      ,XXX              ,Consumer_VolumeIncrement
                        ,Consumer_VolumeDecrement
      ,XXX              ,___            ,___

      // right hand
      ,LGUI(Key_L)     ,Key_F10       ,Key_F2        ,Key_F4        ,Key_F6         ,Key_F8        ,XXX
      ,XXX             ,XXX           ,Key_Home      ,Key_UpArrow   ,Key_End        ,XXX           ,XXX
                       ,XXX           ,Key_LeftArrow ,Key_DownArrow ,Key_RightArrow ,XXX           ,XXX
      ,XXX             ,XXX           ,XXX           ,XXX           ,XXX            ,XXX           ,XXX
      ,MM(BtnR)        ,XXX           ,XXX           ,XXX           ,XXX

      ,XXX      ,XXX
      ,___
      ,XXX      ,Key_PageUp ,Key_PageDown
  ),

  [APPSEL] = KEYMAP_STACKED
  (
      // left hand
       XXX             ,XXX           ,XXX       ,XXX           ,XXX       ,XXX       ,XXX
      ,XXX             ,XXX           ,XXX       ,XXX           ,XXX       ,XXX       ,XXX
      ,XXX             ,XXX           ,XXX       ,XXX           ,XXX       ,XXX
      ,XXX             ,XXX           ,XXX       ,XXX           ,XXX       ,XXX       ,XXX
      ,XXX             ,XXX           ,XXX       ,XXX           ,XXX

                        ,XXX            ,XXX
                                        ,XXX
      ,XXX              ,___            ,___

      // right hand
      ,XXX             ,XXX           ,XXX       ,XXX      ,XXX        ,XXX           ,XXX
      ,XXX             ,XXX           ,XXX       ,XXX      ,XXX        ,XXX           ,XXX
                       ,XXX           ,XXX       ,XXX      ,XXX        ,XXX           ,XXX
      ,XXX             ,XXX           ,XXX       ,XXX      ,XXX        ,XXX           ,XXX
      ,XXX             ,XXX           ,XXX       ,XXX      ,XXX

      ,XXX      ,XXX
      ,___
      ,XXX      ,XXX        ,XXX
  )
)
/* *INDENT-ON* */
