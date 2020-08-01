/*
 * battcheck-mode.c: Battery check mode for Anduril.
 *
 * Copyright (C) 2017 Selene ToyKeeper
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

#ifndef BATTCHECK_MODE_C
#define BATTCHECK_MODE_C

#include "battcheck-mode.h"

uint8_t battcheck_state(Event event, uint16_t arg) {
    ////////// Every action below here is blocked in the simple UI //////////
    #ifdef USE_SIMPLE_UI
    if (simple_ui_active) {
        return EVENT_NOT_HANDLED;
    }
    #endif

    // 1 click: off
    if (event == EV_1click) {
        set_state(off_state, 0);
        return MISCHIEF_MANAGED;
    }

    #if defined(USE_BEACON_MODE)
    // 2 clicks: beacon mode
    else if (event == EV_2clicks) {
        set_state(beacon_state, 0);
        return MISCHIEF_MANAGED;
    }
    #elif defined(USE_THERMAL_REGULATION)
    // 2 clicks: tempcheck mode
    else if (event == EV_2clicks) {
        set_state(tempcheck_state, 0);
        return MISCHIEF_MANAGED;
    }
    #endif

    #ifdef USE_VOLTAGE_CORRECTION
    // 7 clicks: voltage config mode
    else if (event == EV_7clicks) {
        push_state(voltage_config_state, 0);
        return MISCHIEF_MANAGED;
    }
    #endif

    return EVENT_NOT_HANDLED;
}

#ifdef USE_VOLTAGE_CORRECTION
// the user can adjust the battery measurements... on a scale of 1 to 13
// 1 = subtract 0.30V
// 2 = subtract 0.25V
// ...
// 7 = no effect (add 0V)
// 8 = add 0.05V
// ...
// 13 = add 0.30V
void voltage_config_save() {
    uint8_t foo = config_state_values[0];
    if (foo) voltage_correction = config_state_values[0];
}

uint8_t voltage_config_state(Event event, uint16_t arg) {
    return config_state_base(event, arg, 1, voltage_config_save);
}
#endif  // #ifdef USE_VOLTAGE_CORRECTION


#endif

