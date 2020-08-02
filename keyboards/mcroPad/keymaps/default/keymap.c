/* Copyright 2020 Balz Guenat
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
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
#include QMK_KEYBOARD_H
#include "analog.h"

// Defines names for use in layer keycodes and the keymap
enum layer_names {
    _BASE
};

// Defines the keycodes used by our macros in process_record_user
enum custom_keycodes {
    QMKBEST = SAFE_RANGE,
    QMKURL,
    SLIDER
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* Base */
    [_BASE] = LAYOUT(
        RESET,
        KC_B,   KC_C,   KC_D,   KC_E,  KC_F,   KC_G,
        KC_H,   KC_I,   KC_J,   KC_K,  KC_L,   SLIDER
    )
};

uint16_t slider_last_val = 0;
uint8_t last_volume = 0;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case QMKBEST:
            if (record->event.pressed) {
                // when keycode QMKBEST is pressed
                SEND_STRING("QMK is the best thing ever!");
            } else {
                // when keycode QMKBEST is released
            }
            break;
        case QMKURL:
            if (record->event.pressed) {
                // when keycode QMKURL is pressed
                SEND_STRING("https://qmk.fm/\n");
            } else {
                // when keycode QMKURL is released
            }
            break;
        case SLIDER:
            if (record->event.pressed) {
                // char *str = "A\n";
                uint8_t val = (slider_last_val >> 3) % 26;
                // sprintf(&str, "%u\n", slider_last_val);
                tap_code(KC_A + val);
            }
            break;
    }
    return true;
}

void adjust_volume(int8_t steps) {
    if (steps == 0) {
        return;
    }
    uint8_t kc = steps >= 0 ? KC_VOLU : KC_VOLD;
    steps = steps >= 0 ? steps : -steps;
    for (int i = 0; i < steps; i++)
    {
        tap_code(kc);
    }
}

uint8_t to_vol(uint16_t val) {
    return (0x3FF - val) * 50 / 0x3FF;
    // return 255 - (val >> 2);
}

uint8_t digit_to_kc(uint8_t d) {
    return KC_1 + ((d + 9) % 10);
}

void slider_update(uint16_t val) {
    uint8_t vol = to_vol(val);
    int8_t diff = vol - last_volume;
    adjust_volume(diff);
    last_volume = vol;
}

uint8_t divisor = 0;

void slider(void) {
    if (divisor++) { // only run the slider function 1/256 times it's called
        return;
    }
    uint16_t cur = analogReadPin(SLIDER_PIN);
    int16_t diff = cur - slider_last_val;
    if (diff >= SLIDER_SENS || diff <= -SLIDER_SENS) {
        // tap_code(digit_to_kc(cur / 100));
        // tap_code(digit_to_kc((cur % 100) / 10));
        // tap_code(digit_to_kc(cur % 10));
        // tap_code(KC_SPACE);
        slider_update(cur);
        slider_last_val = cur;
    }
}

void matrix_init_user(void) {
    last_volume = to_vol(analogReadPin(SLIDER_PIN));
}

void matrix_scan_user(void) {
    slider();
}

void encoder_update_user(uint8_t index, bool clockwise) {
    if (index == 0) { /* First encoder */
        if (clockwise) {
            tap_code(KC_DOWN);
        } else {
            tap_code(KC_UP);
        }
    }
}