#include "rednering.h"
#include "painting.h"
#include "triangle.h"
#include <stdio.h>
#include <Math.h>
#define EdgeCrossAsVec(a, b) {a.y*b.z-a.z*b.y, a.x*b.z-a.z*b.x, a.y*b.x-a.x*b.y}
#define Gel_Vec_lenght(a) (sqrt(a.x*a.x+a.y*a.y+a.z*a.z))
#define NormalizeVec(a, l) {a.x/l, a.y/l, a.z/l}
#define dot(a,b) (a.x*b.x+a.y*b.y+a.z*b.z)
#define sub_vec(a, b) {a->x-b->x, a->y-b->y, a->z-b->z}


Sceane init_sceane(int count, Camera cam) {
	Sceane out;
	out.camera = cam;
	out.Obj_count = count;
	out.objects = (Mesh*)malloc(sizeof(Mesh) * count);
	return out;
	
}

void redner_mesh(Mesh* mesh, Camera cam, GameWindowBuffer* screane, GameWindowBuffer* textture, GameWindowBuffer* depth) {
	Vec_1x3 pos   = mesh->pos;
	Vec_1x3 rot   = mesh->rot;
	Vec_1x3 scale = mesh->scale;

	float h = (float)screane->h;
	float w = (float)screane->w;
	float Zf = 1000;
	float Zn = 0.1;
	float FOV = 1.0 / tanf(90*0.5 / 180.0 * 3.1415);
	
	// rotacja skalowanie przesuniêcie kamera projekcja 
	// x->x
	// y->y
	// z->z
	// rotx->a
	// roty->b
	// rotz->c
	// scale.x->q
	// scale.y->i
	// scale.z->e
	//h->h
	//w->w
	//znear->n
	//zfare->f
	//FOW->u
	//
	//
	//
	//
	//
	//
	//{ { R.x, U.x, F.x, 0 }, { R.y,    U.y,    F.y,    0 }, { R.z,    U.z,    F.z,    0 }, { -Dot[R, P], -Dot[U, P], -Dot[F, P], 1 } }
	//{{((h R.x cot(u / 2)) / w | (h cot(u / 2) U.x) / w | (h F.x cot(u / 2)) / w | 0}
	//	R.y cot(u / 2) | cot(u / 2) U.y | F.y cot(u / 2) | 0
	//	(f R.z) / (f - n) - R.P | (f U.z) / (f - n) - U.P | (f F.z) / (f - n) - F.P | 1
	//	- (f n R.z) / (f - n) | -(f n U.z) / (f - n) | -(f n F.z) / (f - n) | 0)
	// {
	// {1,0,0,x},
	// {0,1,0,y},
	// {0,0,1,z},
	// {0,0,0,1}
	// }
	//

	Mat4x4 mat_pro = 
	{
		{(h / w) * FOV,0,0,0},
		{ 0,FOV,0,0 },
		{0,0,Zf / (Zf - Zn),1},
		{0,0,(-Zf * Zn) / (Zf - Zn),0 }
	};

	//b = y
	//
	//y = z
	//a = x
	Mat4x4 mat_rot =
	{
		{
			cos(rot.y) * cos(rot.z),
			cos(rot.z) * sin(rot.x) * sin(rot.y) - cos(rot.x) * sin(rot.z),
			cos(rot.x) * cos(rot.z)* sin(rot.y) + sin(rot.x) * sin(rot.z),
			0
		},
		{
			cos(rot.y) * sin(rot.z),
			sin(rot.x)* sin(rot.y)* sin(rot.z) + cos(rot.x) * cos(rot.z),
			cos(rot.x)* sin(rot.y)* sin(rot.z) - cos(rot.z) * sin(rot.x),
			0
		},
		{
			-sin(rot.y),
			sin(rot.x) * cos(rot.y),
			cos(rot.x)* cos(rot.y),
			0
		},
		{0, 0, 0, 1}
	};


	Mat4x4 mat_scale_ofste = {
	{scale.x,0,0, pos.x},
	{0,scale.y,0,pos.y},
	{0,0,scale.z,pos.z},
	{0 ,0 ,0 ,1}
	};
	Vec_1x3 light = { 0,0,-1 };


	for (int i = 0; i < mesh->vert_count; ) {
		multily_by_matrix_advanced(mesh->verst_reference + i, mat_rot, mesh->verts + i);
		multily_by_matrix_advanced(mesh->verts + i, mat_scale_ofste, mesh->verts +i);
		multily_by_matrix_advanced(mesh->verts + i, mat_pro, mesh->verts+i);
		//printf("x: %lf y: %lf z: %lf", (mesh->verts + i)->x, (mesh->verts + i)->y, (mesh->verts + i)->z);
		i++;
	}
	Vec_1x3 edge1;
	Vec_1x3 edge2;
	Vec_1x3 Normal;
	struct triInt white = { 255,255,255 };
	for (int i = 0; i < mesh->tri_count; ) {
		edge1 = (Vec_1x3)sub_vec((mesh->tris + i)->verts.b, (mesh->tris + i)->verts.a);
		edge2 = (Vec_1x3)sub_vec((mesh->tris + i)->verts.c, (mesh->tris + i)->verts.a);
		//{ edge1.y* edge2.z - edge1.z * edge2.y, edge1.x* edge2.z - edge1.z * edge2.x, edge1.y* edge2.x - edge1.x * edge2.y };
		Normal = (Vec_1x3)EdgeCrossAsVec(edge2, edge1);

		float l = Gel_Vec_lenght(Normal);
		Normal = (Vec_1x3)NormalizeVec(Normal, l);
		//printf("x: {%lf * %lf - %lf * %lf} = %lf, y: {%lf * %lf - %lf * %lf} = %lf, z: {%lf * %lf - %lf * %lf} = %lf\n", edge1.y, edge2.z, edge1.z, edge2.y, Normal.x, edge1.x, edge2.z, edge1.z, edge2.x, Normal.y, edge1.y, edge2.x, edge1.x, edge2.y, Normal.z);


	
		if (Normal.z>0) {
			printf("hej: %lf\n", (mesh->tris + i)->verts.a->z);

			
			Draw_triangle(mesh->tris + i, screane, textture, depth, Normal.z * 8 > 1.0 ? 1 : Normal.z * 8);
			

			//Trianle temp_tri;
			//
			//Vec_1x3 a = {0,0,0};
			//Vec_1x3 b = {0,0,0};
			//Vec_1x3 c = { 0,0,0 };
			//
			//temp_tri.verts.a = &a;
			//temp_tri.verts.b = &b;
			//temp_tri.verts.c = &c;
			//
			//
			//multily_by_matrix_advanced((mesh->tris + i)->verts.a, mat_pro, &a);
			//multily_by_matrix_advanced((mesh->tris + i)->verts.b, mat_pro, &b);
			//multily_by_matrix_advanced((mesh->tris + i)->verts.c, mat_pro, &c);


			//Draw_triangle_outline(mesh->tris + i, screane, white);
		}
		i++;
	}
}

void redner_sceane(Sceane* sceane, GameWindowBuffer* screane, GameWindowBuffer* texture, GameWindowBuffer* depth) {


	// oblicz wielk¹ macie¿
	for (int i = 0; i < sceane->Obj_count;) {

		redner_mesh(sceane->objects[i], sceane->camera, screane, texture, depth);
		i++;
	}
};

