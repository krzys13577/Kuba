#pragma once
#include "Vec_math.h"
#include "window_handle_mine.h"


typedef struct Trianle_vert {
	Vec_1x3* a;
	Vec_1x3* b;
	Vec_1x3* c;
} Trianle_vert;


typedef struct Trianle_UV {
	Vec_1x2 a;
	Vec_1x2 b;
	Vec_1x2 c;
} Trianle_UV;

typedef struct Trianle {
	Trianle_vert verts;
	Trianle_UV UVs;

} Trianle;


typedef struct Mesh {
	int tri_count;
	Trianle* tris;
	int vert_count;
	Vec_1x3* verts;
	Vec_1x3* verst_reference;
	Vec_1x3 pos;
	Vec_1x3 rot;
	Vec_1x3 scale;
	GameWindowBuffer textture;
} Mesh;