#include <stdio.h>
#include "window_handle_mine.h"
#include "painting.h"


// to nie dzia³a
GameWindowBuffer read_file(char path[200]) {
    FILE* fitr;
    //"C:\\Users\\KrzysztofPolowczyk\\Desktop\\W.bmp"
    // Open the file in binary read mode
    fopen_s(&fitr, path, "r");

    
    // wspieram tylko RGBA ewentualnie ABGR

    char* buf = (char*)malloc(6);

    fread(buf, 1, 6, fitr);

    int size = *(int*)(buf + 2);
    free(buf);

    char *img = (char*)malloc(size-6);

    fread(img, 1, size-6, fitr);

    GameWindowBuffer out = { 0 };

    int x = *(int*)(img + 12);
    int y = *(int*)(img + 16);
    int head = *(int*)(img + 4);
    int pic_size = x * y;

    out.h = y;
    out.w = x;

    int bytes_per_col = 4;

    unsigned char* textture;


    textture = img + head;

    fclose(fitr);

    out.memory = textture;
    //printf("file %s\n", path);
    //printf("size: %d\n", size);
    //printf("W: %d H:%d\n", out.w,out.h);
    //printf("R: %d G:%d, B:%d A:%d\n", out.memory[0], out.memory[1], out.memory[2], out.memory[3]);

    return out;
}