#include <stdio.h>
#include "window_handle_mine.h"
#include "painting.h"



GameWindowBuffer read_file(char a[200]) {
    FILE* fitr;
    //"C:\\Users\\KrzysztofPolowczyk\\Desktop\\W.bmp"
    // Open the file in binary read mode
    fopen_s(&fitr, "C:\\Users\\KrzysztofPolowczyk\\Desktop\\Tunk.bmp", "r");


    // wspieram tylko RGBA ewentualnie ARGB

    char* buf = (char*)malloc(18289152);

    int size = fread(buf, 1, 18289152, fitr);

    printf("%d x \n", *(int*)(buf + 18));
    printf("%d y \n", *(int*)(buf + 22));
    printf("%d head \n", *(int*)(buf + 10));

    GameWindowBuffer out = { 0 };

    int x = *(int*)(buf + 18);
    int y = *(int*)(buf + 22);
    int head = *(int*)(buf + 10);
    int pic_size = x * y;
    int c = 0;
    int break_point = 30;

    out.h = y;
    out.w = x;

    int bytes_per_col = 4;

    unsigned char* textture;

    textture = buf + head;

    //free(buf);
    fclose(fitr);

    out.memory = textture;

    return out;
}