#include "Vec_math.h"
#include "triangle.h"
#include <math.h>


void Cross_product_1x3(Vec_1x3* a, Vec_1x3* b, Vec_1x3* out){
	out->x = a->y * b->z - a->z * b->y;
	out->y = a->z * b->x - a->x * b->z;
	out->z = a->x * b->y - a->y * b->x;
};

void Cross_product_1x2(Vec_1x2* a, Vec_1x2* b, Vec_1x3* out) {
	
	out->x = 0;
	out->y = 0;
	out->z = a->x * b->y - a->y * b->x;
	
};

void Add_in_place_1x3(Vec_1x3* a, Vec_1x3* b) {
	a->x += b->x;
	a->y += b->y;
	a->z += b->z;
};

void sub_in_place_1x3(Vec_1x3* a, Vec_1x3* b) {
	a->x -= b->x;
	a->y -= b->y;
	a->z -= b->z;
};


void add_1x2(Vec_1x2* a, Vec_1x2* b, Vec_1x2* out) {
	out->x = a->x + b->x;
	out->y = a->y + b->y;
};

void sub_1x2(Vec_1x2* a, Vec_1x2* b, Vec_1x2* out ) {
	out->x = a->x - b->x;
	out->y = a->y - b->y;
};

void sub_in_place_1x2(Vec_1x2* a, Vec_1x2* b) {
	a->x -= b->x;
	a->y -= b->y;
};


void Add_in_place_1x2(Vec_1x2* a, Vec_1x2* b) {
	a->x += b->x;
	a->y += b->y;
};

int get_len(Vec_1x3* line) {
	return sqrt((line->x * line->x + line->y * line->y) + line->z * line->z);
}

int is_in_tri(Vec_1x2* side_a, Vec_1x2* side_b, Vec_1x2* side_c, Vec_1x2* point_vec_1, Vec_1x2* point_vec_2, Vec_1x2* point_vec_3) {
	Vec_1x3 cors_1;
	Vec_1x3 cors_2;
	Vec_1x3 cors_3;
	Cross_product_1x2(side_a, point_vec_1, &cors_1);
	Cross_product_1x2(side_b, point_vec_2, &cors_2);
	Cross_product_1x2(side_c, point_vec_3, &cors_3);
	if (cors_1.z < 0) return 0;
	if (cors_2.z < 0) return 0;
	if (cors_3.z < 0) return 0;
	return 1;
}