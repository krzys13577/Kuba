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
	// chyba nie diza³a
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

Vec_1x4 multily_by_matrix_advanced(Vec_1x4 point, Mat4x4 mat){
	Vec_1x4 out;

	out.x = mat.row1.x * point.x + mat.row1.y * point.y + mat.row1.z * point.z + mat.row1.w * point.w;
	out.y = mat.row2.x * point.x + mat.row2.y * point.y + mat.row2.z * point.z + mat.row2.w * point.w;
	out.z = mat.row3.x * point.x + mat.row3.y * point.y + mat.row3.z * point.z + mat.row3.w * point.w;
	out.w = mat.row4.x * point.x + mat.row4.y * point.y + mat.row4.z * point.z + mat.row4.w * point.w;
	return out;
}


Vec_1x3 rotVec(Vec_1x3 vec, Vec_1x3 rot) {
	Vec_1x3 out;
	out.x = vec.x * (cos(rot.y) * cos(rot.z)) + vec.y * (cos(rot.z) * sin(rot.x) * sin(rot.y) - cos(rot.x) * sin(rot.z)) + vec.z * (cos(rot.x) * cos(rot.z) * sin(rot.y) + sin(rot.x) * sin(rot.z));
	out.y = vec.x * cos(rot.y) * sin(rot.z) + vec.y * (sin(rot.x) * sin(rot.y) * sin(rot.z) + cos(rot.x) * cos(rot.z)) + vec.z * (cos(rot.x) * sin(rot.y) * sin(rot.z) - cos(rot.z) * sin(rot.x));
	out.z = vec.x * (-sin(rot.y)) + vec.y * (sin(rot.x) * cos(rot.y)) + vec.z * (cos(rot.x) * cos(rot.y));
	return out;
}

