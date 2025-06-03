#pragma once
#include "Vec_math.h"

typedef struct Trianle_vert {
	Vec_1x3 a;
	Vec_1x3 b;
	Vec_1x3 c;
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
} Mesh;