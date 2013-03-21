#ifndef GLWT_win32_keymap_h
#define GLWT_win32_keymap_h

#include <stdlib.h>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

struct keymap_entry
{
    int from, to;
};

static const struct keymap_entry unsorted_keymap[] =
{
    { VK_BACK, GLWT_KEY_BACKSPACE },
    { VK_TAB, GLWT_KEY_TAB },
    { VK_RETURN, GLWT_KEY_RETURN },
    { VK_ESCAPE, GLWT_KEY_ESCAPE },
    { VK_SPACE, GLWT_KEY_SPACE },

    { VK_OEM_PLUS, GLWT_KEY_PLUS },
    { VK_OEM_COMMA, GLWT_KEY_COMMA },
    { VK_OEM_MINUS, GLWT_KEY_MINUS },
    { VK_OEM_PERIOD, GLWT_KEY_PERIOD },
    { VK_DIVIDE, GLWT_KEY_SLASH },

    { (int)('0'), GLWT_KEY_0 },
    { (int)('1'), GLWT_KEY_1 },
    { (int)('2'), GLWT_KEY_2 },
    { (int)('3'), GLWT_KEY_3 },
    { (int)('4'), GLWT_KEY_4 },
    { (int)('5'), GLWT_KEY_5 },
    { (int)('6'), GLWT_KEY_6 },
    { (int)('7'), GLWT_KEY_7 },
    { (int)('8'), GLWT_KEY_8 },
    { (int)('9'), GLWT_KEY_9 },

    { (int)('A'), GLWT_KEY_A },
    { (int)('B'), GLWT_KEY_B },
    { (int)('C'), GLWT_KEY_C },
    { (int)('D'), GLWT_KEY_D },
    { (int)('E'), GLWT_KEY_E },
    { (int)('F'), GLWT_KEY_F },
    { (int)('G'), GLWT_KEY_G },
    { (int)('H'), GLWT_KEY_H },
    { (int)('I'), GLWT_KEY_I },
    { (int)('J'), GLWT_KEY_J },
    { (int)('K'), GLWT_KEY_K },
    { (int)('L'), GLWT_KEY_L },
    { (int)('M'), GLWT_KEY_M },
    { (int)('N'), GLWT_KEY_N },
    { (int)('O'), GLWT_KEY_O },
    { (int)('P'), GLWT_KEY_P },
    { (int)('Q'), GLWT_KEY_Q },
    { (int)('R'), GLWT_KEY_R },
    { (int)('S'), GLWT_KEY_S },
    { (int)('T'), GLWT_KEY_T },
    { (int)('U'), GLWT_KEY_U },
    { (int)('V'), GLWT_KEY_V },
    { (int)('W'), GLWT_KEY_W },
    { (int)('X'), GLWT_KEY_X },
    { (int)('Y'), GLWT_KEY_Y },
    { (int)('Z'), GLWT_KEY_Z },

    // TODO: LEFTBRACKET, BACKSLASH, RIGHTBRACKET, CARET, UNDERSCORE. BACKQUOTE??!
    { VK_DELETE, GLWT_KEY_DELETE },

    { VK_NUMPAD0, GLWT_KEY_KEYPAD_0 },
    { VK_NUMPAD1, GLWT_KEY_KEYPAD_1 },
    { VK_NUMPAD2, GLWT_KEY_KEYPAD_2 },
    { VK_NUMPAD3, GLWT_KEY_KEYPAD_3 },
    { VK_NUMPAD4, GLWT_KEY_KEYPAD_4 },
    { VK_NUMPAD5, GLWT_KEY_KEYPAD_5 },
    { VK_NUMPAD6, GLWT_KEY_KEYPAD_6 },
    { VK_NUMPAD7, GLWT_KEY_KEYPAD_7 },
    { VK_NUMPAD8, GLWT_KEY_KEYPAD_8 },
    { VK_NUMPAD9, GLWT_KEY_KEYPAD_9 },
    { VK_DECIMAL, GLWT_KEY_KEYPAD_SEPARATOR },
    { VK_DIVIDE, GLWT_KEY_KEYPAD_DIVIDE },
    { VK_MULTIPLY, GLWT_KEY_KEYPAD_MULTIPLY },
    { VK_ADD, GLWT_KEY_KEYPAD_PLUS },
    { VK_SUBTRACT, GLWT_KEY_KEYPAD_MINUS },
    { VK_RETURN, GLWT_KEY_KEYPAD_ENTER },

    { VK_UP, GLWT_KEY_UP },
    { VK_DOWN, GLWT_KEY_DOWN },
    { VK_LEFT, GLWT_KEY_LEFT },
    { VK_RIGHT, GLWT_KEY_RIGHT },
    { VK_PRIOR, GLWT_KEY_PAGE_UP },
    { VK_NEXT, GLWT_KEY_PAGE_DOWN },
    { VK_HOME, GLWT_KEY_HOME },
    { VK_END, GLWT_KEY_END },
    { VK_INSERT, GLWT_KEY_INSERT },

    { VK_F1, GLWT_KEY_F1 },
    { VK_F2, GLWT_KEY_F2 },
    { VK_F3, GLWT_KEY_F3 },
    { VK_F4, GLWT_KEY_F4 },
    { VK_F5, GLWT_KEY_F5 },
    { VK_F6, GLWT_KEY_F6 },
    { VK_F7, GLWT_KEY_F7 },
    { VK_F8, GLWT_KEY_F8 },
    { VK_F9, GLWT_KEY_F9 },
    { VK_F10, GLWT_KEY_F10 },
    { VK_F11, GLWT_KEY_F11 },
    { VK_F12, GLWT_KEY_F12 },

    { VK_LSHIFT, GLWT_KEY_LSHIFT },
    { VK_RSHIFT, GLWT_KEY_RSHIFT },
    { VK_LCONTROL, GLWT_KEY_LCTRL },
    { VK_RCONTROL, GLWT_KEY_RCTRL },
    { VK_LMENU, GLWT_KEY_LALT },
    { VK_RMENU, GLWT_KEY_RALT },
    { VK_LWIN, GLWT_KEY_LSUPER },
    { VK_RWIN, GLWT_KEY_RSUPER },
    { VK_MENU, GLWT_KEY_MENU },
    { VK_RMENU, GLWT_KEY_ALTGR },
    { VK_NUMLOCK, GLWT_KEY_NUM_LOCK },
    { VK_CAPITAL, GLWT_KEY_CAPS_LOCK },
    { VK_SCROLL, GLWT_KEY_SCROLL_LOCK },
};

struct keymap
{
    struct keymap_entry keys[sizeof(unsorted_keymap)/sizeof(*unsorted_keymap)];
};

static int keymap_comparator(const void *a, const void *b)
{
    const struct keymap_entry *l = (const struct keymap_entry*)a;
    const struct keymap_entry *r = (const struct keymap_entry*)b;
    return l->from < r->from ? -1 : (l->from > r->from ? 1 : 0);
}

static void keymap_init(struct keymap *keymap)
{
    const int keymap_size = sizeof(unsorted_keymap)/sizeof(*unsorted_keymap);
    int i = 0;
    for(i = 0; i < keymap_size; ++i)
        keymap->keys[i] = unsorted_keymap[i];
    qsort(keymap->keys, keymap_size, sizeof(struct keymap_entry), keymap_comparator);
}

static int keymap_lookup(const struct keymap *keymap, int xkeysym)
{
    const int keymap_size = sizeof(unsorted_keymap)/sizeof(*unsorted_keymap);
    const struct keymap_entry key = { xkeysym, 0 };
    const struct keymap_entry *entry =
        bsearch(&key, keymap->keys, keymap_size, sizeof(struct keymap_entry), keymap_comparator);
    return entry ? entry->to : GLWT_KEY_UNKNOWN;
}

#endif
