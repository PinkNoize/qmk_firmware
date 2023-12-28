#include QMK_KEYBOARD_H
#include "split_util.h"
#include "print.h"

// Layers
#define TRNS_LAYER 4
#define FN_LAYER 3
#define SYMBOLS_LAYER 2
#define GAMING_LAYER 1
#define BASE_LAYER 0

// Color Definitions
//#define BASE_COLOR 60, 20, 80
#define BASE_COLOR 11, 176, 255
#define NUMPAD_COLOR HSV_RED
#define ARROW_COLOR HSV_MAGENTA
#define CAPS_COLOR HSV_GOLD
#define TOGGLE_GAMING_COLOR HSV_CHARTREUSE

// Left side mappings
#define W_INDEX 12
#define E_INDEX 11
#define A_INDEX 16
#define S_INDEX 17
#define D_INDEX 18
#define F_INDEX 19
#define CAPS_INDEX 28

// Right side mappings
#define NUMPAD_START 44
#define NUMPAD_END 58
#define BKSP_INDEX 34

#define Y_INDEX 43
#define CAPS2_INDEX 62
#define END_INDEX 67

#define ARROW_UP 45
#define ARROW_RIGHT 51
#define ARROW_DOWN 52
#define ARROW_LEFT 53


void keyboard_post_init_user(void) {
    // debug_enable=true;
    // debug_matrix=true;
    rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR);
    rgb_matrix_sethsv_noeeprom(HSV_OFF);
}

void set_rgb_matrix_hsv_color(int index, uint8_t led_min, uint8_t led_max, uint16_t hue, uint8_t sat, uint8_t val) {
    HSV hsv = {
        hue,
        sat,
        (val > RGB_MATRIX_MAXIMUM_BRIGHTNESS) ? RGB_MATRIX_MAXIMUM_BRIGHTNESS : val
    };
    RGB rgb = hsv_to_rgb(hsv);
    RGB_MATRIX_INDICATOR_SET_COLOR(index, rgb.r, rgb.g, rgb.b);
    //rgb_matrix_set_color(index, rgb.r, rgb.g, rgb.b);
}

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    static uint8_t prev_layer = 0xff;
    //dprintf("isLeft: %d, min: %d, max: %d\n", isLeftHand, led_min, led_max);
    uint8_t layer = get_highest_layer(layer_state|default_layer_state);
    switch(layer) {
        case TRNS_LAYER:
            if (keymap_config.swap_lctl_lgui) {
                if (prev_layer != layer) {
                    rgb_matrix_sethsv_noeeprom(HSV_WHITE);
                }
            }
            break;
        case GAMING_LAYER:
            if (prev_layer != layer) {
                rgb_matrix_sethsv_noeeprom(HSV_RED);
            }
            if (isLeftHand) {
                set_rgb_matrix_hsv_color(E_INDEX, led_min, led_max, ARROW_COLOR);
                set_rgb_matrix_hsv_color(S_INDEX, led_min, led_max, ARROW_COLOR);
                set_rgb_matrix_hsv_color(D_INDEX, led_min, led_max, ARROW_COLOR);
                set_rgb_matrix_hsv_color(F_INDEX, led_min, led_max, ARROW_COLOR);
            } else {
                set_rgb_matrix_hsv_color(END_INDEX, led_min, led_max, TOGGLE_GAMING_COLOR);
            }
            break;
        case FN_LAYER:
            if (!isLeftHand) {
                set_rgb_matrix_hsv_color(ARROW_UP, led_min, led_max, ARROW_COLOR);
                set_rgb_matrix_hsv_color(ARROW_RIGHT, led_min, led_max, ARROW_COLOR);
                set_rgb_matrix_hsv_color(ARROW_DOWN, led_min, led_max, ARROW_COLOR);
                set_rgb_matrix_hsv_color(ARROW_LEFT, led_min, led_max, ARROW_COLOR);
            }
            break;
        case SYMBOLS_LAYER:
            // if (prev_layer != layer) {
            //     rgb_matrix_sethsv_noeeprom(HSV_BLUE);
            // }
            set_rgb_matrix_hsv_color(NUMPAD_START, led_min, led_max, NUMPAD_COLOR);
            set_rgb_matrix_hsv_color(NUMPAD_START+1, led_min, led_max, NUMPAD_COLOR);
            set_rgb_matrix_hsv_color(NUMPAD_START+2, led_min, led_max, NUMPAD_COLOR);
            set_rgb_matrix_hsv_color(NUMPAD_START+7, led_min, led_max, NUMPAD_COLOR);
            set_rgb_matrix_hsv_color(NUMPAD_START+8, led_min, led_max, NUMPAD_COLOR);
            set_rgb_matrix_hsv_color(NUMPAD_START+9, led_min, led_max, NUMPAD_COLOR);
            set_rgb_matrix_hsv_color(NUMPAD_START+12, led_min, led_max, NUMPAD_COLOR);
            set_rgb_matrix_hsv_color(NUMPAD_START+13, led_min, led_max, NUMPAD_COLOR);
            set_rgb_matrix_hsv_color(NUMPAD_START+15, led_min, led_max, NUMPAD_COLOR);
            set_rgb_matrix_hsv_color(NUMPAD_START+19, led_min, led_max, NUMPAD_COLOR);
            if (host_keyboard_led_state().num_lock && !isLeftHand) {
                set_rgb_matrix_hsv_color(Y_INDEX, led_min, led_max, CAPS_COLOR);
            }
            break;
        case BASE_LAYER:
            if (prev_layer != layer) {
                rgb_matrix_sethsv_noeeprom(BASE_COLOR);
            }
            break;
        default:
            break;
    }
    if (host_keyboard_led_state().caps_lock) {
        if (isLeftHand && layer!=GAMING_LAYER) {
            set_rgb_matrix_hsv_color(CAPS_INDEX, led_min, led_max, CAPS_COLOR);
        } else {
            set_rgb_matrix_hsv_color(CAPS2_INDEX, led_min, led_max, CAPS_COLOR);
        }
    }
    prev_layer = layer;
    // for (uint8_t row = 0; row < MATRIX_ROWS; ++row) {
    //     for (uint8_t col = 0; col < MATRIX_COLS; ++col) {
    //         uint8_t index = g_led_config.matrix_co[row][col];

    //         if (index >= led_min && index < led_max && index != NO_LED &&
    //         keymap_key_to_keycode(layer, (keypos_t){col,row}) == KC_I) {
    //             set_rgb_matrix_hsv_color(index, led_min, led_max, HSV_GREEN);
    //             dprintf("Found I\n");
    //             dprintf("i @ %d: left hand?: %d\n", index, isLeftHand);
    //         }
    //     }
    // }
    return false;
}

enum {
    TD_LSFT_CAPS,
    TD_RSFT_CAPS,
};

// Tap Dance definitions
tap_dance_action_t tap_dance_actions[] = {
    // Tap once for shift, twice for Caps Lock
    [TD_LSFT_CAPS] = ACTION_TAP_DANCE_DOUBLE(KC_LSFT, KC_CAPS),
    // Tap once for shift, twice for Caps Lock
    [TD_RSFT_CAPS] = ACTION_TAP_DANCE_DOUBLE(KC_RSFT, KC_CAPS),
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	[BASE_LAYER] = LAYOUT(
        KC_ESC, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KC_BSPC,
        KC_TAB, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_BSLS,
        KC_LCTL, KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_QUOT,
        TD(TD_LSFT_CAPS), KC_Z, KC_X, KC_C, KC_V, KC_B, KC_F23, KC_END, KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, TD(TD_RSFT_CAPS),
        KC_LALT, MO(SYMBOLS_LAYER), KC_SPC, KC_ENT, MO(FN_LAYER), KC_LGUI
    ),
    [GAMING_LAYER] = LAYOUT(
        KC_ESC, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KC_BSPC,
        KC_TAB, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_BSLS,
        KC_LCTL, KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_QUOT,
        KC_LSFT, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_HOME, TO(BASE_LAYER), KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, TD(TD_RSFT_CAPS),
        KC_LALT, KC_SPC, KC_F23, KC_ENT, MO(FN_LAYER), KC_LGUI
    ),
	[SYMBOLS_LAYER] = LAYOUT(
        KC_NO, KC_EXLM, KC_AT, KC_HASH, KC_DLR, KC_PERC, KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_DEL,
        LCTL(KC_V), KC_GRV, KC_LCBR, KC_ASTR, KC_RCBR, KC_PLUS, KC_NUM, KC_P7, KC_P8, KC_P9, KC_NO, KC_TRNS,
        LCTL(KC_C), KC_UNDS, KC_LPRN, KC_EQL, KC_RPRN, KC_MINS, KC_NO, KC_P4, KC_P5, KC_P6, KC_NO, QK_BOOT,
        KC_TRNS, KC_NO, KC_LBRC, KC_AMPR, KC_RBRC, KC_HASH, KC_HOME, KC_NO, KC_NO, KC_P1, KC_P2, KC_P3, KC_NO, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, MO(TRNS_LAYER), KC_P0),
	[FN_LAYER] = LAYOUT(
        KC_F12, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11,
        RGB_TOG, KC_EXLM, KC_AT, KC_HASH, KC_DLR, KC_PERC, KC_NO, KC_NO, KC_UP, KC_NO, TO(GAMING_LAYER), KC_TRNS,
        RGB_MOD, KC_MPRV, KC_MNXT, KC_VOLU, KC_PGUP, KC_UNDS, KC_NO, KC_LEFT, KC_DOWN, KC_RGHT, KC_NO, KC_BSLS,
        TD(TD_LSFT_CAPS), KC_MSTP, KC_MPLY, KC_VOLD, KC_PGDN, KC_MINS, KC_LPRN, KC_TRNS, KC_NO, KC_NO, KC_NO, KC_NO, EE_CLR, KC_TRNS,
        KC_TRNS, MO(TRNS_LAYER), KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS),
    [TRNS_LAYER] = LAYOUT(
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, CG_TOGG,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS)
};

#if defined(ENCODER_ENABLE) && defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {

};
#endif // defined(ENCODER_ENABLE) && defined(ENCODER_MAP_ENABLE)



