#include <windows.h>
#include "window_handle_mine.h"


void Create_window(LRESULT(*proc)(HWND wind, UINT msg, WPARAM wp, LPARAM lp), HWND* wind_out) {

    WNDCLASS wc = { sizeof(WNDCLASS) };
    BITMAPINFO bitmapInfo;

    wc.hCursor = LoadCursor(0, IDC_ARROW);
    wc.hInstance = GetModuleHandle(0);
    wc.lpszClassName = L"EpicWindowClass";
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = proc;

    if (!RegisterClass(&wc)) { return; };

    HWND wind = CreateWindowW(
        wc.lpszClassName,
        L"We have a Window!",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        0, 0, GetModuleHandle(0), 0
    );

    *wind_out = wind;
};

void InvalidateRect_mine(HWND wind) {
    InvalidateRect(wind, NULL, FALSE);
}

void resetWindowBuffer(GameWindowBuffer* gameWindowBuffer, BITMAPINFO* bitmapInfo, HWND wind, GameWindowBuffer* depth_buffer)
{
    RECT rect = { 0 };
    GetClientRect(wind, &rect);
    gameWindowBuffer->h = rect.bottom;
    gameWindowBuffer->w = rect.right;

    depth_buffer->h = rect.bottom;
    depth_buffer->w = rect.right;

    depth_buffer->memory = (unsigned char*)malloc(rect.bottom * rect.right *4);

    for (int i = 0; i < rect.bottom * rect.right; i++) {
        ((int *)(depth_buffer->memory))[i] = 0;
    }

    if (gameWindowBuffer->memory)
    {
        VirtualFree(gameWindowBuffer->memory, 0, MEM_RELEASE);
    }

    gameWindowBuffer->memory =
        (unsigned char*)VirtualAlloc(0, 4 * gameWindowBuffer->h * gameWindowBuffer->w,
            MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

    bitmapInfo->bmiHeader.biSize = sizeof(BITMAPINFO);
    bitmapInfo->bmiHeader.biWidth = *(int*)gameWindowBuffer;
    bitmapInfo->bmiHeader.biHeight = -*((int*)(gameWindowBuffer)+1);
    bitmapInfo->bmiHeader.biPlanes = 1;
    bitmapInfo->bmiHeader.biBitCount = 32;
    bitmapInfo->bmiHeader.biCompression = BI_RGB;
}

void handle_entires(HWND wind) {
    MSG msg = { 0 };
    while (PeekMessage(&msg, wind, 0, 0, PM_REMOVE) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

GameWindowBuffer init_GameWindowBuffer(int w, int h) {
    GameWindowBuffer out;
    out.h = h;
    out.w = w;
    int *to_clear = (int*)malloc(sizeof(int)*h*w);
    for (int i = 0; i < w * h; i++) {
        to_clear[i] = 0;
    }
    out.memory = (unsigned char*)to_clear;
    return out;
};