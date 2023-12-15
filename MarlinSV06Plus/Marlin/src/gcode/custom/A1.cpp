/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

#include "../../inc/MarlinConfig.h"
#include "../gcode.h"

#if ENABLED(RTS_AVAILABLE)
  #include "../../lcd/sv06p/LCD_RTS.h"
#endif
/**
 * A1: Control Sovol SV06 Plus LCD state
 *
 *  M[mode] - Set mode flag
 *
 *  Default values are used for omitted arguments.
 */

const char* mode_names[] = {"Light", "Dark"};


void GcodeSuite::A1() {
    SERIAL_ECHOLNPGM("SV06 Plus LCD control");
    if (parser.seenval('M')) {
        const int new_mode_flag = parser.intval('M', -1);
        if (new_mode_flag == 0 || new_mode_flag == 1) {
            SERIAL_ECHOLNPGM("Setting LCD mode: ", mode_names[new_mode_flag]);
            Mode_flag = new_mode_flag;
        } else {
            SERIAL_ECHOLNPGM("Invalid LCD mode requested: ", new_mode_flag);
        }
    }
    if (parser.seenval('P')) {
        const int new_page_num = parser.intval('P', -1);
        SERIAL_ECHOLNPGM("Setting LCD page: ", new_page_num);
        rtscheck.RTS_SndData(ExchangePageBase + new_page_num, ExchangepageAddr);
    }
    if (parser.seenval('A')) {
        const int new_page_num = parser.intval('A', -1) + (Mode_flag ? 0 : 55);
        SERIAL_ECHOLNPGM(
            "Setting LCD page (adjusted for ",
            mode_names[Mode_flag] ,
            " mode): ",
            new_page_num
        );
        rtscheck.RTS_SndData(ExchangePageBase + new_page_num, ExchangepageAddr);
    }
    SERIAL_ECHOLNPGM("LCD mode: ", mode_names[Mode_flag]);
}
