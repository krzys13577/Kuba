#pragma once
#include "Vec_math.h"
#include "triangle.h"
#include "window_handle_mine.h"
#include "painting.h"
#define EdgeCrossAsVec(a, b) {a.y*b.z-a.z*b.y, a.x*b.z-a.z*b.x, a.y*b.x-a.x*b.y}
#define Gel_Vec_lenght(a) (sqrt(a.x*a.x+a.y*a.y+a.z*a.z))
#define NormalizeVec(a, l) {a.x/l, a.y/l, a.z/l}
#define dot(a,b) (a.x*b.x+a.y*b.y+a.z*b.z)
#define sub_vec(a, b) {a->x-b->x, a->y-b->y, a->z-b->z}
#define sub_vec_direct(a, b) {a.x-b.x, a.y-b.y, a.z-b.z}
#define add_vec_direct(a, b) {a.x+b.x, a.y+b.y, a.z+b.z}
#define mul_vec_direct(a, b) {a.x*b, a.y*b, a.z*b}


typedef struct Camera {
	Vec_1x3 pos;
	Vec_1x3 lookDir;
	Vec_1x3 up;
	Vec_1x3 rot;

} Camera;

typedef struct Sceane {
	int Obj_count;
	Mesh** objects;
	Camera camera;
} Sceane;

Sceane init_sceane(int count, Camera cam);

void redner_sceane(Sceane* sceane, GameWindowBuffer* screane, GameWindowBuffer* depth, GameWindowBuffer* texture);
