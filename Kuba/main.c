#include "window_handle_mine.h"
#include <windows.h>
#include "triangle.h"
#include "painting.h"
#include "Vec_math.h"
#include "BMP.h"
#include "readingFromOBJ.h"
#include "rednering.h"
#include <math.h>
#include <stdio.h>

int Is_running = 1;
GameWindowBuffer gameWindowBuffer = { 0 };
GameWindowBuffer Depth = { 0 };
BITMAPINFO bitmapInfo = { 0 };
HWND wind;
int fps = 0;


void print_mesh_info(const Mesh* mesh) {
    for (int i = 0; i < mesh->vert_count; ) {
        printf("x: %f, y: %f, z: %f\n", (mesh->verst_reference + i)->x, (mesh->verst_reference + i)->y, (mesh->verst_reference + i)->z);

        printf("\n");
        i += 1;
    };

    for (int i = 0; i < mesh->tri_count; ++i) {
        printf("Triangle %d:\n", i);

        // Vertices
        printf("  Vertices:\n");
        printf("    A: (%.2f, %.2f, %.2f)\n", mesh->tris[i].verts.a->x, mesh->tris[i].verts.a->y, mesh->tris[i].verts.a->z);
        printf("    B: (%.2f, %.2f, %.2f)\n", mesh->tris[i].verts.b->x, mesh->tris[i].verts.b->y, mesh->tris[i].verts.b->z);
        printf("    C: (%.2f, %.2f, %.2f)\n", mesh->tris[i].verts.c->x, mesh->tris[i].verts.c->y, mesh->tris[i].verts.c->z);

        // UVs
        printf("  UVs:\n");
        printf("    A: (%.2f, %.2f)\n", mesh->tris[i].UVs.a.x, mesh->tris[i].UVs.a.y);
        printf("    B: (%.2f, %.2f)\n", mesh->tris[i].UVs.b.x, mesh->tris[i].UVs.b.y);
        printf("    C: (%.2f, %.2f)\n", mesh->tris[i].UVs.c.x, mesh->tris[i].UVs.c.y);

        printf("\n");
    }
}


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
        resetWindowBuffer(&gameWindowBuffer, &bitmapInfo, wind, &Depth);
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
    RECT rect = { 0 };
    GetClientRect(wind, &rect);

    Depth.memory = (unsigned char*)malloc(rect.bottom * rect.right * 4);

    for (int i = 0; i < rect.bottom * rect.right; i++) {
        ((int*)(Depth.memory))[i] = 0;
    }

    //Trianle tri;
    //tri.verts.a = (Vec_1x3){ 500,000,500 };
    //tri.verts.b = (Vec_1x3){ 500,500,500 };
    //tri.verts.c = (Vec_1x3){ 000,500,500 };
    //
    //tri.UVs.a = (Vec_1x2){ 500,000};
    //tri.UVs.b = (Vec_1x2){ 500,500};
    //tri.UVs.c = (Vec_1x2){ 000,500};
    //
    //Trianle tri2;
    //tri2.verts.a = (Vec_1x3){ 000, 000, 500 };
    //tri2.verts.b = (Vec_1x3){ 500,000,500 };
    //tri2.verts.c = (Vec_1x3){ 000,500,500 };
    //   
    //tri2.UVs.a = (Vec_1x2){ 0,0 };
    //tri2.UVs.b = (Vec_1x2){ 500,0 };
    //tri2.UVs.c = (Vec_1x2){ 0,500 };

    GameWindowBuffer texture = read_file("C:\\Users\\KrzysztofPolowczyk\\Desktop\\T-34.bmp");
    GameWindowBuffer BLUE = read_file("C:\\Users\\KrzysztofPolowczyk\\Desktop\\BLUE.bmp");
    GameWindowBuffer RED = read_file("C:\\Users\\KrzysztofPolowczyk\\Desktop\\red.bmp");
    

    DWORD ThreadId_send;

    Mesh mesh1 = load_mesh_from_file("C:\\Users\\KrzysztofPolowczyk\\Desktop\\cube.txt", texture.w, texture.h);
    Mesh mesh2 = load_mesh_from_file("C:\\Users\\KrzysztofPolowczyk\\Desktop\\cube.txt", texture.w, texture.h);
    //print_mesh_info(&mesh1);

    mesh1.pos = (Vec_1x3){0,0,15.0};
    mesh1.rot = (Vec_1x3){0,0,0};
    mesh1.scale = (Vec_1x3){0.3,0.3,0.3};
    mesh2.pos = (Vec_1x3){0,0,40};
    mesh2.rot = (Vec_1x3){0,0,0};
    mesh2.scale = (Vec_1x3){0.3,0.3,0.3 };

    //Mesh mesh2 = load_mesh_from_file("C:\\Users\\KrzysztofPolowczyk\\Desktop\\cube2.txt");

    CreateThread(NULL, 0, FPS, NULL, 0, NULL);

    Camera cam = {
        {0,0,0},
        {0,0,0},
        {0,0,-1},
        {1,0,0},
        {1,0,0}
    };

    Sceane sceane = init_sceane(1, cam);

    sceane.objects[0] = &mesh1;
    //sceane.objects[1] = &mesh2;

    
    float counter = 0;
    while (Is_running) {
        mesh1.rot.z = counter/10;
        mesh1.rot.x = counter/10;
        //mesh2.rot.z = counter / 10;
        //mesh2.rot.x = counter / 10;
        //mesh1.rot.y = counter/20;

        //mesh1.pos.y = sin(counter)*0.2;
        //mesh1.rot.y = counter*2;
        //mesh1.rot.y = counter/8;
        //mesh1.scale.y = 4* sin(counter)* sin(counter);
        //mesh1.scale.x = 4* sin(counter)* sin(counter);
        //mesh1.scale.y = 4* sin(counter)* sin(counter);
        
        handle_entires(wind);

        clear(&gameWindowBuffer, &Depth);
        redner_sceane(&sceane, &gameWindowBuffer, &texture, &Depth);

        InvalidateRect(wind, NULL, FALSE);

        fps += 1;
        //Sleep(500);
        counter += 0.01;
    }
}

