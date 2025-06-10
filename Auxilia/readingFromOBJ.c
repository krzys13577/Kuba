#include <stdio.h>
#include "readingFromOBJ.h"
#include "triangle.h"
#include "dinamick_list.h"
#include <stdlib.h>

Mesh load_mesh_from_file(char path[200], int w, int h) {
    FILE* file = fopen(path, "r");
    Mesh out;
    char line[300];
    
    Vector list_verts_reference;
    vec_init_my(&list_verts_reference, sizeof(Vec_1x3));

    Vector list_U;
    vec_init_my(&list_U, sizeof(Vec_1x2));

    Vector list_tris;
    vec_init_my(&list_tris, sizeof(Trianle));

    while (fgets(line, sizeof(line), file)) {
        switch (line[0])
        {
        float a;
        float b;
        float c;

        int x;
        int y;

        int x1;
        int y1;
 
        int x2;
        int y2;

        case 'v':
            sscanf_s(line+2, "%f %f %f", &a, &b, &c);
            //printf("hej: %f %f %f\n", a, b, c);
            push_my(&list_verts_reference, (void*)(&(Vec_1x3){a, b, c}));
            break;
        case 't':
            out.verts = (Vec_1x3*)malloc(list_verts_reference.size * sizeof(Vec_1x3));
            sscanf_s(line + 2, "%f %f", &a, &b);
            //printf("kk: %f %f\n", a, b);
            push_my(&list_U, (void*)(&(Vec_1x2) {a*w, b*h}));
            break;
        case 'f':
            sscanf_s(line + 2, "%d/%d %d/%d %d/%d", &x, &y, &x1, &y1, &x2, &y2);
            Trianle temp;

            //printf("%d/%d %d/%d %d/%d\n", x, y, x1, y1, x2, y2);
            temp.verts = (Trianle_vert){ out.verts + x - 1, out.verts + x1 - 1, out.verts + x2 - 1 };
            temp.UVs = (Trianle_UV){ *(Vec_1x2*)get_my(&list_U, y-1),*(Vec_1x2*)get_my(&list_U, y1-1),*(Vec_1x2*)get_my(&list_U, y2-1) };
            push_my(&list_tris, &temp);
            break;
        default:
            break;
        }
    }
    memcpy(out.verts, list_verts_reference.data, sizeof(Vec_1x3) * list_verts_reference.size);
    printf("coun: %d\n", list_tris.size);

    out.verst_reference = list_verts_reference.data;
    out.vert_count = list_verts_reference.size;
    out.tri_count = list_tris.size;
    out.tris = list_tris.data;

    fclose(file);
    return out;


};

