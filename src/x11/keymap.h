#ifndef GLWT_x11_keymap_h
#define GLWT_x11_keymap_h

#include <stdlib.h>

struct keymap_entry
{
    int from, to;
};

static const struct keymap_entry unsorted_keymap[] =
{
    { XK_BackSpace, GLWT_KEY_BACKSPACE },
    { XK_Tab, GLWT_KEY_TAB },
    { XK_Return, GLWT_KEY_RETURN },
    { XK_Escape, GLWT_KEY_ESCAPE },
    { XK_space, GLWT_KEY_SPACE },

    { XK_plus, GLWT_KEY_PLUS },
    { XK_comma, GLWT_KEY_COMMA },
    { XK_minus, GLWT_KEY_MINUS },
    { XK_period, GLWT_KEY_PERIOD },
    { XK_slash, GLWT_KEY_SLASH }, // TODO: what?

    { XK_0, GLWT_KEY_0 },
    { XK_1, GLWT_KEY_1 },
    { XK_2, GLWT_KEY_2 },
    { XK_3, GLWT_KEY_3 },
    { XK_4, GLWT_KEY_4 },
    { XK_5, GLWT_KEY_5 },
    { XK_6, GLWT_KEY_6 },
    { XK_7, GLWT_KEY_7 },
    { XK_8, GLWT_KEY_8 },
    { XK_9, GLWT_KEY_9 },

    { XK_a, GLWT_KEY_A },
    { XK_b, GLWT_KEY_B },
    { XK_c, GLWT_KEY_C },
    { XK_d, GLWT_KEY_D },
    { XK_e, GLWT_KEY_E },
    { XK_f, GLWT_KEY_F },
    { XK_g, GLWT_KEY_G },
    { XK_h, GLWT_KEY_H },
    { XK_i, GLWT_KEY_I },
    { XK_j, GLWT_KEY_J },
    { XK_k, GLWT_KEY_K },
    { XK_l, GLWT_KEY_L },
    { XK_m, GLWT_KEY_M },
    { XK_n, GLWT_KEY_N },
    { XK_o, GLWT_KEY_O },
    { XK_p, GLWT_KEY_P },
    { XK_q, GLWT_KEY_Q },
    { XK_r, GLWT_KEY_R },
    { XK_s, GLWT_KEY_S },
    { XK_t, GLWT_KEY_T },
    { XK_u, GLWT_KEY_U },
    { XK_v, GLWT_KEY_V },
    { XK_w, GLWT_KEY_W },
    { XK_x, GLWT_KEY_X },
    { XK_y, GLWT_KEY_Y },
    { XK_z, GLWT_KEY_Z },

    // TODO: LEFTBRACKET, BACKSLASH, RIGHTBRACKET, CARET, UNDERSCORE. BACKQUOTE??!
    { XK_Delete, GLWT_KEY_DELETE },

    { XK_KP_Insert, GLWT_KEY_KEYPAD_0 },
    { XK_KP_End, GLWT_KEY_KEYPAD_1 },
    { XK_KP_Down, GLWT_KEY_KEYPAD_2 },
    { XK_KP_Page_Down, GLWT_KEY_KEYPAD_3 },
    { XK_KP_Left, GLWT_KEY_KEYPAD_4 },
    { XK_KP_Begin, GLWT_KEY_KEYPAD_5 },
    { XK_KP_Right, GLWT_KEY_KEYPAD_6 },
    { XK_KP_Home, GLWT_KEY_KEYPAD_7 },
    { XK_KP_Up, GLWT_KEY_KEYPAD_8 },
    { XK_KP_Page_Up, GLWT_KEY_KEYPAD_9 },
    { XK_KP_Delete, GLWT_KEY_KEYPAD_SEPARATOR },
    { XK_KP_Divide, GLWT_KEY_KEYPAD_DIVIDE },
    { XK_KP_Multiply, GLWT_KEY_KEYPAD_MULTIPLY },
    { XK_KP_Add, GLWT_KEY_KEYPAD_PLUS },
    { XK_KP_Subtract, GLWT_KEY_KEYPAD_MINUS },
    { XK_KP_Enter, GLWT_KEY_KEYPAD_ENTER },

    { XK_Up, GLWT_KEY_UP },
    { XK_Down, GLWT_KEY_DOWN },
    { XK_Left, GLWT_KEY_LEFT },
    { XK_Right, GLWT_KEY_RIGHT },
    { XK_Page_Up, GLWT_KEY_PAGE_UP },
    { XK_Page_Down, GLWT_KEY_PAGE_DOWN },
    { XK_Home, GLWT_KEY_HOME },
    { XK_End, GLWT_KEY_END },
    { XK_Insert, GLWT_KEY_INSERT },

    { XK_F1, GLWT_KEY_F1 },
    { XK_F2, GLWT_KEY_F2 },
    { XK_F3, GLWT_KEY_F3 },
    { XK_F4, GLWT_KEY_F4 },
    { XK_F5, GLWT_KEY_F5 },
    { XK_F6, GLWT_KEY_F6 },
    { XK_F7, GLWT_KEY_F7 },
    { XK_F8, GLWT_KEY_F8 },
    { XK_F9, GLWT_KEY_F9 },
    { XK_F10, GLWT_KEY_F10 },
    { XK_F11, GLWT_KEY_F11 },
    { XK_F12, GLWT_KEY_F12 },

    { XK_Shift_L, GLWT_KEY_LSHIFT },
    { XK_Shift_R, GLWT_KEY_RSHIFT },
    { XK_Control_L, GLWT_KEY_LCTRL },
    { XK_Control_R, GLWT_KEY_RCTRL },
    { XK_Alt_L, GLWT_KEY_LALT },
    { XK_Alt_R, GLWT_KEY_RALT },
    { XK_Super_L, GLWT_KEY_LSUPER },
    { XK_Super_R, GLWT_KEY_RSUPER },
    { XK_Menu, GLWT_KEY_MENU },
    { XK_ISO_Level3_Shift, GLWT_KEY_ALTGR },
    { XK_Num_Lock, GLWT_KEY_NUM_LOCK },
    { XK_Caps_Lock, GLWT_KEY_CAPS_LOCK },
    { XK_Scroll_Lock, GLWT_KEY_SCROLL_LOCK },
};

struct keymap
{
    struct keymap_entry keys[sizeof(unsorted_keymap)/sizeof(*unsorted_keymap)];
};

static inline int keymap_comparator(const void *a, const void *b)
{
    const struct keymap_entry *l = (const struct keymap_entry*)a;
    const struct keymap_entry *r = (const struct keymap_entry*)b;
    return l->from < r->from ? -1 : (l->from > r->from ? 1 : 0);
}

static inline void keymap_init(struct keymap *keymap)
{
    const int keymap_size = sizeof(unsorted_keymap)/sizeof(*unsorted_keymap);
    for(int i = 0; i < keymap_size; ++i)
        keymap->keys[i] = unsorted_keymap[i];
    qsort(keymap->keys, keymap_size, sizeof(struct keymap_entry), keymap_comparator);
}

static inline int keymap_lookup(const struct keymap *keymap, int xkeysym)
{
    const int keymap_size = sizeof(unsorted_keymap)/sizeof(*unsorted_keymap);
    const struct keymap_entry key = { xkeysym, 0 };
    const struct keymap_entry *entry =
        bsearch(&key, keymap->keys, keymap_size, sizeof(struct keymap_entry), keymap_comparator);
    return entry ? entry->to : GLWT_KEY_UNKNOWN;
}

#endif
