#include "window_handle_mine.h"
#include <windows.h>
#include "triangle.h"
#include "painting.h"
#include "Vec_math.h"
#include "BMP.h"
#include <stdio.h>

int Is_running = 1;
GameWindowBuffer gameWindowBuffer = { 0 };
BITMAPINFO bitmapInfo = { 0 };
HWND wind;
int fps = 0;

// handle inputs heare #ToDO apstract this mess
LRESULT CALLBACK windProc(HWND wind, UINT msg, WPARAM wp, LPARAM lp)
{
    LRESULT rez = 0;

    switch (msg)
    {
    case WM_CLOSE:
        Is_running = 0;
        break;
    case WM_SIZE:
    {
        resetWindowBuffer(&gameWindowBuffer, &bitmapInfo, wind);
    } break;

    case WM_PAINT:
    {
        PAINTSTRUCT Paint;
        HDC DeviceContext = BeginPaint(wind, &Paint);

        HDC hdc = GetDC(wind);

        StretchDIBits(hdc,
            0, 0, gameWindowBuffer.w, gameWindowBuffer.h,
            0, 0, gameWindowBuffer.w, gameWindowBuffer.h,
            gameWindowBuffer.memory,
            &bitmapInfo,
            DIB_RGB_COLORS,
            SRCCOPY
        );

        ReleaseDC(wind, hdc);

        EndPaint(wind, &Paint);
    } break;

    default:
        rez = DefWindowProc(wind, msg, wp, lp);
        break;
    }


    return rez;
}

DWORD WINAPI FPS(LPVOID idn) {
    while (1) {
        printf("FPS: %d\n", fps);
        fps = 0;
        Sleep(1000);
    }
}

int main() {
    
    Create_window(&windProc, &wind);
    Trianle tri;
    tri.verts.a = (Vec_1x3){ 0,0,0 } ;
    tri.verts.c = (Vec_1x3){ 800,0,0 };
    tri.verts.b = (Vec_1x3){0,800,0};

    tri.UVs.a = (Vec_1x2){ 0,0 };
    tri.UVs.c = (Vec_1x2){ 400,0 };
    tri.UVs.b = (Vec_1x2){ 0,400 };

    Trianle tri2;
    tri2.verts.a = (Vec_1x3){ 801,0,0 };
    tri2.verts.b = (Vec_1x3){ 801,801,0 };
    tri2.verts.c = (Vec_1x3){ 0,801,0 };
       
    tri2.UVs.a = (Vec_1x2){ 400,0 };
    tri2.UVs.b = (Vec_1x2){ 400,400 };
    tri2.UVs.c = (Vec_1x2){ 0,400 };
    DWORD ThreadId_send;

    CreateThread(NULL, 0, FPS, NULL, 0, NULL);
    GameWindowBuffer texture = read_file("C:\\Users\\KrzysztofPolowczyk\\Desktop\\tunk.bmp");


    while (Is_running) {

        handle_entires(wind);
        Draw_triangle(&tri, &gameWindowBuffer, &texture);
        Draw_triangle(&tri2, &gameWindowBuffer, &texture);

        InvalidateRect(wind, NULL, FALSE);
        fps += 1;
    }
}