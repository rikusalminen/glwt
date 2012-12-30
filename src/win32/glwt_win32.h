#ifndef GLWT_glwt_win32_h
#define GLWT_glwt_win32_h

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <win32/keymap.h>

struct glwt_win32
{
    HINSTANCE hinstance;
    ATOM classatom;
    HWND dummy_hwnd;
    HDC dummy_hdc;
    int pixel_format;
    struct keymap keymap;
};

struct glwt_window_win32
{
    HWND hwnd;
    HDC hdc;
    int hover;
};

void glwtWin32Error(const char *msg);
LRESULT CALLBACK glwtWin32WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);

#endif
