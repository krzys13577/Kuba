#pragma once
#include <stdlib.h>
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

typedef struct Vec_1x2 {
	float x;
	float y;
} Vec_1x2;

typedef struct Vec_1x3 {
	float x;
	float y;
	float z;
} Vec_1x3;

typedef struct Vec_1x4 {
	float x;
	float y;
	float z;
	float w;
} Vec_1x4;

typedef struct Mat4x4{
	Vec_1x4 row1;
	Vec_1x4 row2;
	Vec_1x4 row3;
	Vec_1x4 row4;
}Mat4x4;

void Cross_product_1x3(Vec_1x3* a, Vec_1x3* b, Vec_1x3* out);
															
void Cross_product_1x2(Vec_1x2* a, Vec_1x2* b, Vec_1x3* out);

void Add_in_place_1x3(Vec_1x3* a, Vec_1x3* b);

void sub_in_place_1x3(Vec_1x3* a, Vec_1x3* b);

void add_1x2(Vec_1x2* a, Vec_1x2* b, Vec_1x2* out);

void sub_1x2(Vec_1x2* a, Vec_1x2* b, Vec_1x2* out);

void sub_in_place_1x2(Vec_1x2* a, Vec_1x2* b);

void Add_in_place_1x2(Vec_1x2* a, Vec_1x2* b);

// nie dzia³a napraw
int get_len(Vec_1x3* line);

int is_in_tri(Vec_1x2* side_a, Vec_1x2* side_b, Vec_1x2* side_c, Vec_1x2* point_vec_1, Vec_1x2* point_vec_2, Vec_1x2* point_vec_3);

Vec_1x4 multily_by_matrix_advanced(Vec_1x3* point, Mat4x4 mat, Vec_1x3* out);

