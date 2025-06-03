#include <stdio.h>
#include "readingFromOBJ.h"
#include "triangle.h"
#include <stdlib.h>

Mesh load_mesh_from_file(char path[200]) {
    FILE* file;

    fopen_s(&file, path, "rb");

    Mesh out;
    char temp_store[6];


    fread(temp_store, 1, 6, file);
    int file_lenght = atoi(temp_store);
    char* data = (char*)malloc(file_lenght+1);

    //printf("Data read: %d\n", file_lenght+1);

    fread(data, 1, file_lenght+1, file);
    data[file_lenght] = '\0';
    int list_counter = 0;
    int verts_counter = 0;
    int UV_counter = 0;
    int tris_counter = 0;

    Vec_1x2* UV_list = NULL;
    Vec_1x3* vert_list = NULL;
    Trianle* tri_list = NULL;

    for (int i = 0; i < file_lenght; i++){

        if (data[i] == 'n') {
            char num_char[7] = { data[i + 2], data[i + 3], data[i + 4], data[i + 5] , data[i + 6] , data[i + 7], '\0' };
            if(list_counter == 2) {
                tri_list = (Trianle*)malloc(sizeof(Trianle) * atoi(num_char));
                out.tri_count = atoi(num_char);
                out.tris = tri_list;
            }
            else if (list_counter) {
                UV_list = (Vec_1x2*)malloc(sizeof(Vec_1x2) * atoi(num_char));
            }
            else {
   
                vert_list = (Vec_1x3*)malloc(sizeof(Vec_1x3) * atoi(num_char));
            }
            
            list_counter += 1;
            
        }

        if (data[i] == 'v') {
            float xyz[3];
  
            char* str = (char *)malloc(500);

            int j = i + 2;
            int counter = 0;
            int pos_counter = 0;
            while (1) {
                if (data[j]=='\n') {
                    str[counter] = '\0';
                    xyz[pos_counter] = atof(str);
                    pos_counter += 1;
                    counter = 0;
                    break;
                }
                if (data[j] == ' ') {
                    str[counter] = '\0';
                    xyz[pos_counter] = atof(str);
                    pos_counter += 1;
                    counter = 0;
                }
                else
                {
                    str[counter] = data[j];
                    counter += 1;
                }
   
                j++;
            }
            free(str);
            //printf("\nx: %f, y: %f, z: %f\n", xyz[0], xyz[1], xyz[2]);
            vert_list[verts_counter] = (Vec_1x3){ xyz[0], xyz[1], xyz[2] };
            verts_counter += 1;
        }

        if (data[i] == 't') {
            float xy[2];

            char* str = (char*)malloc(500);

            int j = i + 2;
            int counter = 0;
            int pos_counter = 0;
            while (1) {
                if (data[j] == '\n') {
                    str[counter] = '\0';
                    xy[pos_counter] = atof(str);
                    pos_counter += 1;
                    counter = 0;
                    break;
                }
                if (data[j] == ' ') {
                    str[counter] = '\0';
                    xy[pos_counter] = atof(str);
                    pos_counter += 1;
                    counter = 0;
                }
                else
                {
                    str[counter] = data[j];
                    counter += 1;
                }

                j++;
            }
            free(str);
            //printf("\nx: %f, y: %f\n", xy[0], xy[1]);
            UV_list[UV_counter] = (Vec_1x2){ xy[0], xy[1] };
            UV_counter += 1;
        }
        
        if (data[i] == 'f') {

            //char* str = (char*)malloc(500);
            char* vert_str = (char*)malloc(500);
            char* UV_str = (char*)malloc(500);


            int j = i + 2;
            Trianle tri;

            int is_vert = 1;
            int verts_counter_local = 0;
            int UV_counter_local = 0;
            int sides_counter = 0;
   
            Trianle_UV temp_UVs;
            Trianle_vert temp_verts;

            while (1) {
                if (data[j] == '\n') {
                    UV_str[UV_counter_local] = '\0';
                    UV_counter_local = 0;
                    verts_counter_local = 0;
                    switch (sides_counter) {
                        case 0:
                            tri.UVs.a = UV_list[atoi(UV_str)-1];
                            tri.verts.a = vert_list[atoi(vert_str) - 1];
                            break;
                        case 1:
                            tri.UVs.b = UV_list[atoi(UV_str) - 1];
                            tri.verts.b = vert_list[atoi(vert_str) - 1];
                            break;
                        case 2:
                            tri.UVs.c = UV_list[atoi(UV_str) - 1];
                            tri.verts.c = vert_list[atoi(vert_str) - 1];
                            break;
                    }
                    break;
                }
                if (data[j] == ' ') {
                    UV_str[UV_counter_local] = '\0';
                    UV_counter_local = 0;
                    verts_counter_local = 0;
                    switch (sides_counter) {
                        case 0:
                            tri.UVs.a = UV_list[atoi(UV_str) - 1];
                            tri.verts.a = vert_list[atoi(vert_str)-1];
                            break;
                        case 1:
                            tri.UVs.b = UV_list[atoi(UV_str) - 1];
                            tri.verts.b = vert_list[atoi(vert_str) - 1];
                            break;
                        case 2:
                            tri.UVs.c = UV_list[atoi(UV_str) - 1];
                            tri.verts.c = vert_list[atoi(vert_str) - 1];
                            break;
                    }
                    sides_counter += 1;
                }
                if (data[j] == '/') {
                    vert_str[verts_counter_local] = '\0';
                    UV_counter_local = 0;
                    verts_counter_local += 1;
                }
                else {
                    UV_str[UV_counter_local] = data[j];
                    vert_str[verts_counter_local] = data[j];
                    verts_counter_local += 1;
                    UV_counter_local += 1;
                }
                j++;
            }
            //printf("triangle a: %f %f %f b: %f %f %f b: %f %f %f ||| t1: %f %f t2: %f %f t1: %f %f\n",
            //    tri.verts.a.x,
            //    tri.verts.a.y,
            //    tri.verts.a.z,
            //    tri.verts.b.x,
            //    tri.verts.b.y,
            //    tri.verts.b.z,
            //    tri.verts.c.x,
            //    tri.verts.c.y,
            //    tri.verts.c.z,
            //
            //    tri.UVs.a.x,
            //    tri.UVs.a.y,
            //    tri.UVs.b.x,
            //    tri.UVs.b.y,
            //    tri.UVs.c.x,
            //    tri.UVs.c.y);
            free(vert_str);
            free(UV_str);
            tri_list[tris_counter] = tri;
            tris_counter++;
        }

    }

    free(data);
    free(vert_list);
    free(UV_list);
    fclose(file);
    return out;

};
























