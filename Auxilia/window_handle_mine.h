#pragma once
#include <windows.h>

typedef struct GameWindowBuffer {
    int w;
    int h;
    unsigned char* memory;
} GameWindowBuffer;

void Create_window(LRESULT(*proc)(HWND wind, UINT msg, WPARAM wp, LPARAM lp), HWND* wind_out);

void InvalidateRect_mine(HWND wind);

void resetWindowBuffer(GameWindowBuffer* gameWindowBuffer, BITMAPINFO* bitmapInfo, HWND wind);

void handle_entires(HWND wind);
