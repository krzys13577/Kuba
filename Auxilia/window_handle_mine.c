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

void resetWindowBuffer(GameWindowBuffer* gameWindowBuffer, BITMAPINFO* bitmapInfo, HWND wind)
{
    RECT rect = { 0 };
    GetClientRect(wind, &rect);
    gameWindowBuffer->h = rect.bottom;
    gameWindowBuffer->w = rect.right;

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