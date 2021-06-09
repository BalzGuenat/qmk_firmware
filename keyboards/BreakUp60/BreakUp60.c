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

#include "BreakUp60.h"

void onSplit(void) {
	// insert code to run when keyboard splits
	tap_code(KC_S);
}

void onMerge(void) {
	// insert code to run when keyboard merges
	tap_code(KC_M);
}

bool is_keyboard_left(void);

bool last_split_state = false;

bool together(void) {
	return !last_split_state;
}

bool apart(void) {
	return !together();
}

void keyboard_post_init_kb(void) {
    // initialize anything that requires ChibiOS
	if (is_keyboard_left()) {
		setPinOutput(LED_PIN);
		writePinHigh(LED_PIN);
		// detect split state
		setPinInputHigh(SPLIT_SENSE_PIN);
		last_split_state = readPin(SPLIT_SENSE_PIN);
	} else {
		setPinOutput(LED_PIN);
		writePinLow(LED_PIN);
		// pull split sense pin low
		setPinOutput(SPLIT_SENSE_PIN);
		writePinLow(SPLIT_SENSE_PIN);
	}
}

void matrix_scan_kb(void) {
	bool split_state = readPin(SPLIT_SENSE_PIN);
	// writePin(LED_PIN, split_state);
	if (split_state != last_split_state) {
		// TODO: needs debouncing
		last_split_state = split_state;
		apart() ? onSplit() : onMerge();
	}
}
