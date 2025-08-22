/**
 * Copyright 2021 Charly Delay <charly@codesink.dev> (@0xcharly)
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

#ifdef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE
#    include "timer.h"
#endif // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE

enum charybdis_keymap_layers {
    LAYER_BASE = 0,
    LAYER_NUMERIC_KEYS,
    LAYER_NAVIGATION,
    LAYER_POINTER,
};

/** \brief Automatically enable sniping-mode on the pointer layer. */
#define CHARYBDIS_AUTO_SNIPING_ON_LAYER LAYER_POINTER

#ifdef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE
static uint16_t auto_pointer_layer_timer = 0;
#    ifndef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS
#        define CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS 1000
#    endif // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS

#    ifndef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD
#        define CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD 8
#    endif // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD
#endif     // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE

// Pick any HSV you like for each layer:
#define HSV_BASE      HSV_WHITE     // Base
#define HSV_NUM       HSV_YELLOW    // Numeric
#define HSV_NAV       HSV_MAGENTA   // Navigation
#define HSV_POINTER   HSV_CYAN      // Pointer


/** these are aliases for german keycodes */
#define DE_SHARP_S KC_MINS
#define DE_UMLAUT_U KC_LBRC
#define DE_UMLAUT_O KC_SCLN
#define DE_UMLAUT_A KC_QUOT
#define DE_MINUS KC_SLSH
#define DE_Y KC_Z
#define DE_Z KC_Y
/** end of alias keycodes */

#define TO_BASE TO(LAYER_BASE)
#define TO_NUMERIC_KEYS TO(LAYER_NUMERIC_KEYS)
#define TO_NAVIGATION TO(LAYER_NAVIGATION)
#define TO_POINTER TO(LAYER_POINTER)
#define PT_Y LT(LAYER_POINTER, DE_Y)
#define ENTER_RALT_ON_HOLD MT(MOD_RALT, KC_ENT)
#define DEL_LALT_ON_HOLD MT(MOD_LALT, KC_DEL)

#ifndef POINTING_DEVICE_ENABLE
#    define DRGSCRL KC_NO
#    define DPI_MOD KC_NO
#    define S_D_MOD KC_NO
#    define SNIPING KC_NO
#endif // !POINTING_DEVICE_ENABLE


// ---------- Per-layer colors (RGB Matrix or RGB Light) ----------

#define HSV_BASE      HSV_WHITE
#define HSV_NUM       HSV_YELLOW
#define HSV_NAV       HSV_MAGENTA
#define HSV_POINTER   HSV_CYAN

static void apply_layer_color(uint8_t layer) {
#if defined(RGB_MATRIX_ENABLE)
    rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR);
    switch (layer) {
        case LAYER_POINTER:    rgb_matrix_sethsv_noeeprom(HSV_POINTER); break;
        case LAYER_NAVIGATION: rgb_matrix_sethsv_noeeprom(HSV_NAV);     break;
        case LAYER_NUMERIC_KEYS: rgb_matrix_sethsv_noeeprom(HSV_NUM);   break;
        case LAYER_BASE:
        default:               rgb_matrix_sethsv_noeeprom(HSV_BASE);    break;
    }
#elif defined(RGBLIGHT_ENABLE)
    rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
    switch (layer) {
        case LAYER_POINTER:    rgblight_sethsv_noeeprom(HSV_POINTER); break;
        case LAYER_NAVIGATION: rgblight_sethsv_noeeprom(HSV_NAV);     break;
        case LAYER_NUMERIC_KEYS: rgblight_sethsv_noeeprom(HSV_NUM);   break;
        case LAYER_BASE:
        default:               rgblight_sethsv_noeeprom(HSV_BASE);    break;
    }
#else
    (void)layer;
#endif
}

// Set base color at boot
void keyboard_post_init_user(void) {
    apply_layer_color(LAYER_BASE);
}

// Update color on layer change
layer_state_t layer_state_set_user(layer_state_t state) {
    apply_layer_color(get_highest_layer(state));
#ifdef CHARYBDIS_AUTO_SNIPING_ON_LAYER
    charybdis_set_pointer_sniping_enabled(layer_state_cmp(state, CHARYBDIS_AUTO_SNIPING_ON_LAYER));
#endif
    return state;
}

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [LAYER_BASE] = LAYOUT(
  // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
        KC_ESC,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,       KC_6,    KC_7,    KC_8,    KC_9,    KC_0, DE_SHARP_S,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
        KC_TAB,    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,       DE_Z,    KC_U,    KC_I,    KC_O,    KC_P, DE_UMLAUT_U,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       KC_LSFT,    KC_A,    KC_S,    KC_D,    KC_F,    KC_G,       KC_H,    KC_J,    KC_K,    KC_L, DE_UMLAUT_O, DE_UMLAUT_A,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       KC_LCTL,    PT_Y,    KC_X,    KC_C,    KC_V,    KC_B,       KC_N,    KC_M, KC_COMM,  KC_DOT, DE_MINUS, KC_RSFT,
  // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                                  KC_BSPC, KC_SPC,  KC_LGUI,     TO_POINTER,  ENTER_RALT_ON_HOLD,
                             TO_NUMERIC_KEYS, TO_NAVIGATION,     DEL_LALT_ON_HOLD
  //                            ╰───────────────────────────╯ ╰──────────────────╯
  ),

  [LAYER_NUMERIC_KEYS] = LAYOUT(
  // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
       _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
         KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,    KC_F7,     KC_F8,   KC_F9,  KC_F10,  KC_F11,  KC_F12,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       _______,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,     KC_6,      KC_7,    KC_8,    KC_9,    KC_0, DE_SHARP_S,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       _______,  _______, XXXXXXX, KC_GRV, KC_NUBS, XXXXXXX,    KC_RBRC, KC_NUHS, KC_COMM,  KC_DOT, KC_RALT, _______,
  // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                                   _______, _______, _______,    TO_POINTER, _______,
                                      TO_BASE, TO_NAVIGATION,    _______
  //                            ╰───────────────────────────╯ ╰──────────────────╯
  ),

  [LAYER_NAVIGATION] = LAYOUT(
  // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
       _______, XXXXXXX, XXXXXXX, XXXXXXX, KC_MUTE, KC_VOLU,    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
        KC_TAB, XXXXXXX, XXXXXXX, KC_AGIN, KC_UNDO, KC_VOLD,    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       _______, XXXXXXX,  KC_CUT, KC_COPY, KC_PASTE, XXXXXXX,   KC_LEFT, KC_DOWN,   KC_UP, KC_RGHT, KC_HOME,  KC_END,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,    XXXXXXX, XXXXXXX, MS_WHLD, MS_WHLU, XXXXXXX, XXXXXXX,
  // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                                  _______, _______, _______,   TO_POINTER, _______,
                                           TO_BASE, XXXXXXX,   _______
  //                            ╰───────────────────────────╯ ╰──────────────────╯
  ),

  [LAYER_POINTER] = LAYOUT(
  // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
       QK_BOOT,  EE_CLR, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, QK_BOOT,  EE_CLR,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, DPI_MOD, S_D_MOD,    S_D_MOD, DPI_MOD, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       XXXXXXX, KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, XXXXXXX,    XXXXXXX, KC_RSFT, KC_RCTL, KC_RALT, KC_RGUI, XXXXXXX,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       XXXXXXX, _______, DRGSCRL, SNIPING, XXXXXXX, XXXXXXX,    XXXXXXX, XXXXXXX, SNIPING, DRGSCRL, _______, XXXXXXX,
  // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                                  _______, XXXXXXX, _______,    XXXXXXX, XXXXXXX,
                                           XXXXXXX, XXXXXXX,    XXXXXXX
  //                            ╰───────────────────────────╯ ╰──────────────────╯
  ),
};
// clang-format on

#ifdef POINTING_DEVICE_ENABLE
#    ifdef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE
report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
    if (abs(mouse_report.x) > CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD ||
        abs(mouse_report.y) > CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD) {
        if (auto_pointer_layer_timer == 0) {
            layer_on(LAYER_POINTER);
            apply_layer_color(LAYER_POINTER);
        }
        auto_pointer_layer_timer = timer_read();
    }
    return mouse_report;
}

void matrix_scan_user(void) {
    if (auto_pointer_layer_timer != 0 &&
        TIMER_DIFF_16(timer_read(), auto_pointer_layer_timer) >= CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS) {
        auto_pointer_layer_timer = 0;
        layer_off(LAYER_POINTER);
        apply_layer_color(get_highest_layer(layer_state));
    }
}
#    endif // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE
#endif     // POINTING_DEVICE_ENABLE
