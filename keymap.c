/* Copyright 2015-2017 Jack Humbert
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
#include "muse.h"

#ifdef PIMORONI_TRACKBALL_ENABLE
#include "pimoroni_trackball.h"
#include "pointing_device.h"
#include "i2c_master.h"
#include "timer.h"
#endif

enum planck_layers { _QWERTY, _LOWER, _RAISE, _EXTRA, _ADJUST, _MOUS };

enum planck_keycodes { QWERTY = SAFE_RANGE, BALL_NCL, BALL_RCL, BALL_SCR, BALL_VAL, BALL_SAT, BALL_HUE};

#define LOWER MO(_LOWER)
#define RAISE MO(_RAISE)
#define EXTRA TG(_EXTRA)
#define FN_1 LT(0, KC_NO)
#define FN_2 LT(0, KC_TRNS)

const uint16_t PROGMEM test_combo[] = {KC_LGUI, KC_LSFT, COMBO_END};
combo_t key_combos[COMBO_COUNT] = {COMBO(test_combo, KC_CAPS)};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

	/* Qwerty
	* ,-----------------------------------------------------------------------------------.
	* | Tab  |   Q  |   W  |   E  |   R  |   T  |   Y  |   U  |   I  |   O  |   P  | Bksp |
	* |------+------+------+------+------+------+------+------+------+------+------+------|
	* | Esc  |   A  |   S  |   D  |   F  |   G  |   H  |   J  |   K  |   L  |   ;  |  "   |
	* |------+------+------+------+------+------+------+------+------+------+------+------|
	* | Shift|   Z  |   X  |   C  |   V  |   B  |   N  |   M  |   ,  |   .  |   /  |Enter |
	* |------+------+------+------+------+------+------+------+------+------+------+------|
	* | Brite| Ctrl | Alt  | FN1  | Space | TRACKBALL  | FN2  | Left | Down |  Up  |Right |
	* `-----------------------------------------------------------------------------------'
	*/
	[_QWERTY] = LAYOUT_planck_grid(
		KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,  KC_T,   KC_Y,   KC_U,  KC_I,    KC_O,    KC_P,    KC_BSPC,
		KC_ESC,  KC_A,    KC_S,    KC_D,    KC_F,  KC_G,   KC_H,   KC_J,  KC_K,    KC_L,    KC_SCLN, KC_QUOT,
		KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,  KC_B,   KC_N,   KC_M,  KC_COMM, KC_DOT,  KC_SLSH, KC_SFTENT,
		KC_LCTRL,KC_LGUI, KC_LALT, FN_1, KC_SPC, _______, _______, FN_2, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT
	),

	/* Lower
	* ,-----------------------------------------------------------------------------------.
	* |   `  |      |      |  End |      |      |      |      |      |   [  |   ]  | Bksp |
	* |------+------+------+------+------+------+------+------+------+------+------+------|
	* |   ~  |      |      |   _  |   -  |      | Home |   +  |   =  |      |      |  |   |
	* |------+------+------+------+------+------+------+------+------+------+------+------|
	* |      |      |      |      |      |      |      |      |      |      |   \  |      |
	* |------+------+------+------+------+------+------+------+------+------+------+------|
	* |      |      |      |      |      |             |      | Left | Down |  Up  | Right|
	* `-----------------------------------------------------------------------------------'
	*/
	[_LOWER] = LAYOUT_planck_grid(
		KC_GRV, _______, _______, KC_END , _______, _______, _______, _______, _______, KC_LBRC, KC_RBRC, KC_DEL,
		KC_TILD, _______, _______, KC_UNDS, KC_MINS, _______, KC_HOME, KC_PLUS, KC_EQL, _______, _______, KC_PIPE,
		_______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_BSLS, _______,
		_______, _______, _______, _______, _______, _______, _______, _______, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT
	),

	/* Raise
	* ,-----------------------------------------------------------------------------------.
	* |   `  |      |      |  End |      |      |      |      |      |   {  |   }  | Bksp |
	* |------+------+------+------+------+------+------+------+------+------+------+------|
	* |   ~  |      |      |   _  |   -  |      | Home |   +  |   =  |      |      |  |   |
	* |------+------+------+------+------+------+------+------+------+------+------+------|
	* |      |      |      |      |      |      |      |      |      |      |   \  |      |
	* |------+------+------+------+------+------+------+------+------+------+------+------|
	* |      |      |      |      |      |             |      | Left | Down |  Up  | Right|
	* `-----------------------------------------------------------------------------------'
	*/
	[_RAISE] = LAYOUT_ortho_4x12 (
        KC_GRV, _______, _______, KC_END , _______, _______, _______, _______, _______,  KC_LCBR, KC_RCBR, KC_DEL,
        KC_TILD, _______, _______, KC_UNDS, KC_MINS, _______, KC_HOME, KC_PLUS, KC_EQL,  _______, _______, KC_PIPE,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_BSLS, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
    ),

    /* Extra
    * ,-----------------------------------------------------------------------------------.
	* |      |   1  |   2  |   3  |   4  |   5  |   6  |   7  |   8  |   9  |   0  | Bksp |
	* |------+------+------+------+------+------+------+------+------+------+------+------|
	* |   F1 |  F2  |  F3  |  F4  |  F5  |  F6  |  F7  |  F8  |  F9  | F10  |  F11 |  F12 |
	* |------+------+------+------+------+------+------+------+------+------+------+------|
	* |      |      |      |      |      |      |      |      |      |      |   \  |      |
	* |------+------+------+------+------+------+------+------+------+------+------+------|
	* |      |      |      |      |      |             |      | Prev | Vol- | Vol+ | Next |
	* `-----------------------------------------------------------------------------------'
    */
	[_EXTRA] = LAYOUT_ortho_4x12 (
		_______,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_BSPC,
		  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,  KC_F10,  KC_F11,  KC_F12,
		_______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_MPLY,
		_______, _______, _______, _______, _______, _______, _______, _______, KC_MPRV, KC_VOLD, KC_VOLU, KC_MNXT
	),

    /* Mouse
	* ,-----------------------------------------------------------------------------------.
	* |      |      |      |      |      |      |      |      |      |      |      | Bksp |
	* |------+------+------+------+------+------+------+------+------+------+------+------|
	* |      |      |      |      |      |      |      |      |      |      |      |      |
	* |------+------+------+------+------+------+------+------+------+------+------+------|
	* |      |      |      |      |      |      |      |      |      |      |      |      |
	* |------+------+------+------+------+------+------+------+------+------+------+------|
	* |      |      |      |      |      |             |LClick|RClick|Scroll|      |      |
	* `-----------------------------------------------------------------------------------'
	*/
    [_MOUS] = LAYOUT_ortho_4x12 (
		KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_BSPC,
		KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,
		KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,
		KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   BALL_NCL,  BALL_RCL,  BALL_SCR,  KC_NO,     KC_NO
    ),

	/* Adjust (Lower + Raise)
	* ,-----------------------------------------------------------------------------------.
	* |CKOnOf| Reset|EEP_R |      |      |      |      |      |      | BSAT | BHUE | BVal |
	* |------+------+------+------+------+------+------+------+------+------+------+------|
	* | CK Up|      |MUSmod|Aud on|Audoff|      |      |Qwerty|      |      |      | Swirl|
	* |------+------+------+------+------+------+------+------+------+------+------+------|
	* |CK Dwn|      |      |Mus on|Musoff|      |      |      |      |      |SupCar| Cycle|
	* |------+------+------+------+------+------+------+------+------+------+------+------|
	* |CK Rst|      |      |      |      |             |      |      |Light-|Light+|Toggle|
	* `-----------------------------------------------------------------------------------'
	*/
	[_ADJUST] = LAYOUT_planck_grid(
		CK_TOGG, RESET,   EEP_RST,   _______, _______, _______, _______, _______, _______, BALL_SAT,  BALL_HUE,  BALL_VAL,
		CK_UP, _______, MU_MOD,  AU_ON,   AU_OFF,  _______, _______, QWERTY,  _______, _______, _______,   RGB_M_SW,
		CK_DOWN, _______, _______,  MU_ON,   MU_OFF,  _______,   _______, _______, _______, _______, RGB_M_K, RGB_M_R,
		CK_RST, _______, _______, _______, _______, _______, _______, _______, _______, RGB_VAD, RGB_VAI, RGB_TOG
	)

};

#ifdef PIMORONI_TRACKBALL_ENABLE
#define MOUSE_TIMEOUT 1000
#define TRACKBALL_TIMEOUT 5
#define SIGN(x) ((x > 0) - (x < 0))

static int16_t mouse_auto_layer_timer = 0;
static bool hue_mode_enabled = 0;
static bool saturation_mode_enabled = 0;
static bool value_mode_enabled = 0;
static bool scroll_mode_enabled = 0;

// user config EEPROM stuff {{{
typedef union {
    uint32_t raw;
    struct {
        uint8_t tb_hue : 8;
        uint8_t tb_value : 8;
        uint8_t tb_saturation : 8;
    };
} user_config_t;

user_config_t user_config;

static uint8_t tb_hue        = 204;
static uint8_t tb_saturation = 220;
static uint8_t tb_value      = 194;
static int16_t tb_timer      = 0;

static int16_t x_offset = 0;
static int16_t y_offset = 0;
static int16_t v_offset = 0;
static int16_t h_offset = 0;

static uint8_t last_layer = _QWERTY;

void eeconfig_init_user(void) {
    user_config.raw           = 0;
    user_config.tb_hue        = 204;
    user_config.tb_saturation = 220;
    user_config.tb_value      = 194;
    eeconfig_update_user(user_config.raw);
}

void keyboard_post_init_user(void) {
    user_config.raw = eeconfig_read_user();
    tb_hue          = user_config.tb_hue;
    tb_saturation   = user_config.tb_saturation;
    tb_value        = user_config.tb_value;
    trackball_set_hsv(tb_hue, tb_saturation, tb_value);
}

void matrix_init_user() {
    trackball_init();
}

void suspend_power_down_user(void) {
    trackball_set_brightness(0);
    /* trackball_sleep(); */
}

__attribute__((weak)) void pointing_device_init(void) { trackball_set_rgbw(0,0,0,tb_value); }

void update_member(int8_t* member, int16_t* offset) {
    if (*offset > 127) {
        *member = 127;
        *offset -= 127;
    } else if (*offset < -127) {
        *member = -127;
        *offset += 127;
    } else {
        *member = *offset;
        *offset = 0;
    }
}

void pointing_device_task(void) {
    report_mouse_t mouse = pointing_device_get_report();

    if (trackball_get_interrupt() && (!tb_timer || timer_elapsed(tb_timer) > TRACKBALL_TIMEOUT)) {
        tb_timer = timer_read() | 1;

        trackball_state_t state = trackball_get_state();

        if (state.button_triggered) {
            if (state.button_down) {
                mouse.buttons |= MOUSE_BTN1;
            } else {
                mouse.buttons &= ~MOUSE_BTN1;
            }
            pointing_device_set_report(mouse);
            pointing_device_send();
        } else {
            // on the ADJUST layer with BALL_VAL held, roll ball downwards to change trackball value
            if (value_mode_enabled == 1) {
                tb_value += state.y * 2;
                trackball_set_hsv(tb_hue, tb_saturation, tb_value | 1);
                // on the ADJUST layer with BALL_SAT held, roll ball downwards to change trackball saturation
            } else if (saturation_mode_enabled == 1) {
                tb_saturation += state.y * 2;
                trackball_set_hsv(tb_hue, tb_saturation | 1, tb_value);
                // on the ADJUST layer, or with BALL_HUE held, roll ball downwards to change trackball hue
            } else if (hue_mode_enabled == 1) {
                tb_hue += state.y;
                trackball_set_hsv(tb_hue | 1, tb_saturation, tb_value);
            } else if (scroll_mode_enabled == 1) {
                h_offset += (state.x);
                v_offset -= (state.y);
            } else if ((state.x || state.y) && !state.button_down) {
                if (!mouse_auto_layer_timer && !layer_state_is(_EXTRA)) {
                    layer_on(_MOUS);
                }
                mouse_auto_layer_timer = timer_read() | 1;

                uint8_t scale = 4;
                x_offset += state.x * state.x * SIGN(state.x) * scale;
                y_offset += state.y * state.y * SIGN(state.y) * scale;
            }
        }
    }
    while (x_offset || y_offset || h_offset || v_offset) {
        update_member(&mouse.x, &x_offset);
        update_member(&mouse.y, &y_offset);

        update_member(&mouse.v, &v_offset);
        update_member(&mouse.h, &h_offset);

        pointing_device_set_report(mouse);
        pointing_device_send();
    }
}

void trackball_register_button(bool pressed, enum mouse_buttons button) {
    report_mouse_t currentReport = pointing_device_get_report();
    if (pressed) {
        currentReport.buttons |= button;
    } else {
        currentReport.buttons &= ~button;
    }
    pointing_device_set_report(currentReport);
}

layer_state_t layer_state_set_user(layer_state_t state) {
    uint8_t layer = get_highest_layer(state);

    switch (layer) {
        case _LOWER:
            trackball_set_hsv(128, 255, tb_value);  // Cyan
            break;
        case _RAISE:
            trackball_set_hsv(128, 255, tb_value);  // Cyan
            break;
        case _EXTRA:
            trackball_set_hsv(28, 255, tb_value);  // Orange
            break;
        case _ADJUST:
            trackball_set_hsv(170, 255, tb_value);  // Blue
            break;
        case _MOUS:
            trackball_set_hsv(tb_hue, tb_saturation, tb_value);
            break;
        default:
            trackball_set_hsv(tb_hue, tb_saturation, ((tb_value - 30) > 0 ? (tb_value - 30) : 0));
            break;
    }

    // if layer *was* ADJUST, save eeprom
    if (last_layer == _ADJUST && last_layer != layer && (tb_value != user_config.tb_value || tb_hue != user_config.tb_hue || tb_saturation != user_config.tb_saturation)) {
        user_config.tb_value      = tb_value;
        user_config.tb_hue        = tb_hue;
        user_config.tb_saturation = tb_saturation;
        eeconfig_update_user(user_config.raw);
    }

    last_layer = layer;
    return update_tri_layer_state(state, _LOWER, _RAISE, _ADJUST);
}
#endif

bool process_record_user(uint16_t keycode, keyrecord_t* record) {
    switch (keycode) {
        case QWERTY:
            if (record->event.pressed) {
                set_single_persistent_default_layer(_QWERTY);
            }
            break;

        case FN_1:
            if (record->tap.count && record->event.pressed) {
                if (layer_state_is(_EXTRA)) {
                    layer_off(_EXTRA);
                } else {
                    layer_on(_EXTRA);
                }
            } else if (record->event.pressed) {
                layer_on(_LOWER);
            } else {
                layer_off(_LOWER);
            }
            break;

        case FN_2:
            if (record->tap.count && record->event.pressed) {
                if (layer_state_is(_EXTRA)) {
                    layer_off(_EXTRA);
                } else {
                    layer_on(_EXTRA);
                }
            } else if (record->event.pressed) {
                layer_on(_RAISE);
            } else {
                layer_off(_RAISE);
            }
            break;

#ifdef PIMORONI_TRACKBALL_ENABLE
        case BALL_NCL:
            record->event.pressed ? register_code(KC_BTN1) : unregister_code(KC_BTN1);
            break;

        case BALL_RCL:
            record->event.pressed ? register_code(KC_BTN2) : unregister_code(KC_BTN2);
            break;

        case BALL_SCR:
            if (record->event.pressed) {
                scroll_mode_enabled = 1;
            } else {
                scroll_mode_enabled = 0;
            }
            break;

        case BALL_HUE:
            if (record->event.pressed) {
                hue_mode_enabled = 1;
            } else {
                hue_mode_enabled = 0;
            }
            break;

        case BALL_SAT:
            if (record->event.pressed) {
                saturation_mode_enabled = 1;
            } else {
                saturation_mode_enabled = 0;
            }
            break;

        case BALL_VAL:
            if (record->event.pressed) {
                value_mode_enabled = 1;
            } else {
                value_mode_enabled = 0;
            }
            break;

            if ((keycode < KC_BTN1 || ((keycode > KC_BTN5) && (keycode < SAFE_RANGE))) && layer_state_is(_MOUS) && record->event.pressed) {
                layer_off(_MOUS);
                mouse_auto_layer_timer = 0;
            }
#endif
    }
    return true;
}

#ifdef AUDIO_ENABLE
bool     muse_mode      = false;
uint8_t  last_muse_note = 0;
uint16_t muse_counter   = 0;
uint8_t  muse_offset    = 70;
uint16_t muse_tempo     = 50;

float tone_caps_on[][2]  = SONG(MARIO_MUSHROOM);
float tone_caps_off[][2] = SONG(MARIO_GAMEOVER);

bool music_mask_user(uint16_t keycode) {
    switch (keycode) {
        case RAISE:
        case LOWER:
            return false;
        default:
            return true;
    }
}
#endif

void matrix_scan_user(void) {
#ifdef PIMORONI_TRACKBALL_ENABLE
    if (mouse_auto_layer_timer && timer_elapsed(mouse_auto_layer_timer) > MOUSE_TIMEOUT) {
        report_mouse_t rep = pointing_device_get_report();
        if (rep.buttons) {
            return;
        }
        layer_off(_MOUS);
        mouse_auto_layer_timer = 0;
    }
#endif
#ifdef AUDIO_ENABLE
    if (muse_mode) {
        if (muse_counter == 0) {
            uint8_t muse_note = muse_offset + SCALE[muse_clock_pulse()];
            if (muse_note != last_muse_note) {
                stop_note(compute_freq_for_midi_note(last_muse_note));
                play_note(compute_freq_for_midi_note(muse_note), 0xF);
                last_muse_note = muse_note;
            }
        }
        muse_counter = (muse_counter + 1) % muse_tempo;
    } else {
        if (muse_counter) {
            stop_all_notes();
            muse_counter = 0;
        }
    }
#endif
}

void led_set_user(uint8_t usb_led) {
    static uint8_t old_usb_led = 0;  // 0 = off, 2 = on

    wait_ms(10);
    //_delay_ms(10); // gets rid of tick

    if (!is_playing_notes()) {
        if ((usb_led & (1 << USB_LED_CAPS_LOCK)) && !(old_usb_led & (1 << USB_LED_CAPS_LOCK))) {
            // If CAPS LK LED is turning on...
#ifdef AUDIO_ENABLE
            PLAY_SONG(tone_caps_on);
#endif
#ifdef PIMORONI_TRACKBALL_ENABLE
            trackball_set_hsv(85, 255, tb_value);  // Green
#endif
        } else if (!(usb_led & (1 << USB_LED_CAPS_LOCK)) && (old_usb_led & (1 << USB_LED_CAPS_LOCK))) {
            // If CAPS LK LED is turning off...
#ifdef AUDIO_ENABLE
            PLAY_SONG(tone_caps_off);
#endif
#ifdef PIMORONI_TRACKBALL_ENABLE
            trackball_set_hsv(tb_hue, tb_saturation, tb_value);
#endif
        }
    }
    old_usb_led = usb_led;
}