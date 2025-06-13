#include "rednering.h"
#include "painting.h"
#include "triangle.h"
#include <stdio.h>
#include <Math.h>

Sceane init_sceane(int count, Camera cam) {
	Sceane out;
	out.camera = cam;
	out.Obj_count = count;
	out.objects = (Mesh*)malloc(sizeof(Mesh) * count);
	return out;
	
}

Vec_1x3 Vector_IntersctPlane(Vec_1x3 plane_p, Vec_1x3 plane_n, Vec_1x3 line_start, Vec_1x3 line_end, float* t) {
	float l = Gel_Vec_lenght(plane_n);
	plane_n = (Vec_1x3)NormalizeVec(plane_n, l);

	float plane_d = -dot(plane_n, plane_p);
	float ad = dot(line_start, plane_n);
	float bd = dot(line_end, plane_n);
	*t = (-plane_d - ad) / (bd - ad);

	Vec_1x3 lineStartToEnd = sub_vec_direct(line_end, line_start);
	Vec_1x3 lineToIntersect = mul_vec_direct(lineStartToEnd, *t);

	return (Vec_1x3)add_vec_direct(line_start, lineToIntersect);

}
float util(Vec_1x3 p, Vec_1x3 plane_n, Vec_1x3 plane_p) {
	return (plane_n.x * p.x + plane_n.y * p.y + plane_n.z * p.z - dot(plane_n, plane_p));

}


void Triangle_clip(Trianle* in_tri, int deepth, GameWindowBuffer* screane, GameWindowBuffer* textture, GameWindowBuffer* depth, float light) {
	Vec_1x3 clipped_points[6];
	Trianle clipped_tris[2];
	Vec_1x3 plane_p = { 0,0,0 };
	Vec_1x3 plane_n = { 0,0,0 };
	float t;
	if (deepth == 0) {
		plane_p = (Vec_1x3){ 0,1.0,0 };
		plane_n = (Vec_1x3){ 0,-1,0 };
	}
	if (deepth == 1) {
		plane_p = (Vec_1x3){ 0,-1.0,0 };
		plane_n = (Vec_1x3){ 0,1,0 };
	}
	if (deepth == 2) {
		plane_p = (Vec_1x3){ -1.0,0,0 };
		plane_n = (Vec_1x3){ 1,0,0 };
	}
	if (deepth == 3) {
		plane_p = (Vec_1x3){ 1.0,0,0 };
		plane_n = (Vec_1x3){ -1,0,0 };
	}
	if (deepth == 4) {
		plane_p = (Vec_1x3){ 0,0,10 };
		plane_n = (Vec_1x3){ 0,0,1 };
	}
	if (deepth == 5) {
		Draw_triangle(in_tri, screane, textture, depth, light);
		//Draw_triangle_outline(in_tri, screane, (struct triInt){255,255,255});
		return;
	}

	float l = Gel_Vec_lenght(plane_n);

	plane_n = (Vec_1x3)NormalizeVec(plane_n, l);

	Vec_1x3 inside_points[3]; int inside_points_count = 0;
	Vec_1x3 outside_points[3]; int outside_points_count = 0;
	Vec_1x2 outside_text[3];
	Vec_1x2 inside_text[3];

	float d0 = util(*(in_tri->verts.a), plane_n, plane_p);
	float d1 = util(*(in_tri->verts.b), plane_n, plane_p);
	float d2 = util(*(in_tri->verts.c), plane_n, plane_p);
	//printf("%f %f %f hej\n", in_tri->verts.a->x, in_tri->verts.a->y, in_tri->verts.a->z);

	if (d0 >= 0) { 
		inside_points[inside_points_count] = *(in_tri->verts.a);
		inside_text[inside_points_count] = in_tri->UVs.a;
		inside_points_count++;
	}
	else {
		outside_points[outside_points_count] = *(in_tri->verts.a);
		outside_text[outside_points_count] = in_tri->UVs.a;
		outside_points_count++;
	};


	if (d1 >= 0) {
		inside_points[inside_points_count] = *(in_tri->verts.b);
		inside_text[inside_points_count] = in_tri->UVs.b;
		inside_points_count++;
	}
	else {
		outside_points[outside_points_count] = *(in_tri->verts.b);
		outside_text[outside_points_count] = in_tri->UVs.b;
		outside_points_count++;
	};

	if (d2 >= 0) {
		inside_points[inside_points_count] = *(in_tri->verts.c);
		inside_text[inside_points_count] = in_tri->UVs.c;
		inside_points_count++;
	}
	else {
		outside_points[outside_points_count] = *(in_tri->verts.c);
		outside_text[outside_points_count] = in_tri->UVs.c;
		outside_points_count++;
	};

	if (inside_points_count == 3)
	{
		(clipped_tris + 0)->verts.a = clipped_points + 0;
		(clipped_tris + 0)->verts.b = clipped_points + 1;
		(clipped_tris + 0)->verts.c = clipped_points + 2;
		*(clipped_points + 0) = *(in_tri->verts.a);     
		*(clipped_points + 1) = *(in_tri->verts.b);     
		*(clipped_points + 2) = *(in_tri->verts.c);  
		(clipped_tris + 0)->UVs = in_tri->UVs;
		Triangle_clip(clipped_tris + 0, deepth + 1, screane, textture, depth, light);
		return;
	}
	if (inside_points_count == 1 && outside_points_count == 2) {
		(clipped_tris + 0)->verts.a = clipped_points + 0;
		(clipped_tris + 0)->verts.b = clipped_points + 1;
		(clipped_tris + 0)->verts.c = clipped_points + 2;
		(clipped_tris + 0)->UVs.a = inside_text[0];
		*(clipped_points + 0) = inside_points[0];
		
		*(clipped_points + 1) = Vector_IntersctPlane(plane_p, plane_n, inside_points[0], outside_points[0], &t);
		(clipped_tris + 0)->UVs.b.x = t * (outside_text[0].x - inside_text[0].x) + inside_text[0].x;
		(clipped_tris + 0)->UVs.b.y = t * (outside_text[0].y - inside_text[0].y) + inside_text[0].y;

		*(clipped_points + 2) = Vector_IntersctPlane(plane_p, plane_n, inside_points[0], outside_points[1], &t);
		(clipped_tris + 0)->UVs.c.x = t * (outside_text[1].x - inside_text[0].x) + inside_text[0].x;
		(clipped_tris + 0)->UVs.c.y = t * (outside_text[1].y - inside_text[0].y) + inside_text[0].y;

		Vec_1x3 a = sub_vec_direct(clipped_points[1], clipped_points[0]);
		Vec_1x3 a1 = sub_vec_direct(clipped_points[2], clipped_points[0]);

		if (a.x * a1.y - a.y * a1.x > 0) {
			Vec_1x3 temp = clipped_points[1];
			clipped_points[1] = clipped_points[2];
			clipped_points[2] = temp;

			Vec_1x2 temp_text = clipped_tris[0].UVs.c;
			clipped_tris[0].UVs.c = clipped_tris[0].UVs.b;
			clipped_tris[0].UVs.b = temp_text;
		}


		Triangle_clip(clipped_tris + 0, deepth + 1, screane, textture, depth, light);
		return;
	}

	if (inside_points_count == 2 && outside_points_count == 1) {
		// coœ jest kurde na odwrót
		//printf("%f \n", clipped_points[3].x, clipped_points[3].x);
		(clipped_tris + 0)->verts.a = clipped_points + 0;
		(clipped_tris + 0)->verts.b = clipped_points + 1;
		(clipped_tris + 0)->verts.c = clipped_points + 2;
		(clipped_tris + 1)->verts.a = clipped_points + 3;
		(clipped_tris + 1)->verts.b = clipped_points + 4;
		(clipped_tris + 1)->verts.c = clipped_points + 5;

		(clipped_tris + 1)->UVs = in_tri->UVs;

		*(clipped_points + 0) = inside_points[0];
		(clipped_tris + 0)->UVs.a = inside_text[0];
		*(clipped_points + 1) = inside_points[1];
		(clipped_tris + 0)->UVs.b = inside_text[1];
		*(clipped_points + 2) = Vector_IntersctPlane(plane_p, plane_n, inside_points[0], outside_points[0], &t);
		(clipped_tris + 0)->UVs.c.x = t * (outside_text[0].x - inside_text[0].x) + inside_text[0].x;
		(clipped_tris + 0)->UVs.c.y = t * (outside_text[0].y - inside_text[0].y) + inside_text[0].y;


		*(clipped_points + 3) = inside_points[1];
		(clipped_tris + 1)->UVs.a = inside_text[1];
		*(clipped_points + 4) = *(clipped_points + 2);
		(clipped_tris + 1)->UVs.b = (clipped_tris + 0)->UVs.c;
		*(clipped_points + 5) = Vector_IntersctPlane(plane_p, plane_n, inside_points[1], outside_points[0], &t);
		(clipped_tris + 1)->UVs.c.x = t * (outside_text[0].x - inside_text[1].x) + inside_text[1].x;
		(clipped_tris + 1)->UVs.c.y = t * (outside_text[0].y - inside_text[1].y) + inside_text[1].y;
		
		Vec_1x3 a = sub_vec_direct(clipped_points[1], clipped_points[0]);
		Vec_1x3 a1 = sub_vec_direct(clipped_points[2], clipped_points[0]);

		Vec_1x3 b = sub_vec_direct(clipped_points[4], clipped_points[3]);
		Vec_1x3 b1 = sub_vec_direct(clipped_points[5], clipped_points[3]);

		if (a.x* a1.y - a.y * a1.x > 0) {
			Vec_1x3 temp = clipped_points[1];
			clipped_points[1] = clipped_points[2];
			clipped_points[2] = temp;

			Vec_1x2 temp_text = clipped_tris[0].UVs.c;
			clipped_tris[0].UVs.c = clipped_tris[0].UVs.b;
			clipped_tris[0].UVs.b = temp_text;
		}


		if (b.x * b1.y - b.y * b1.x > 0) {

			Vec_1x3 temp = clipped_points[4];
			clipped_points[4] = clipped_points[5];
			clipped_points[5] = temp;

			Vec_1x2 temp_text = clipped_tris[1].UVs.c;
			clipped_tris[1].UVs.c = clipped_tris[1].UVs.b;
			clipped_tris[1].UVs.b = temp_text;
		}

		Triangle_clip(clipped_tris + 0, deepth + 1, screane, textture, depth, light);
		Triangle_clip(clipped_tris + 1, deepth + 1, screane, textture, depth, light);

		return;
	}

}


void redner_mesh(Mesh* mesh, Camera* cam, GameWindowBuffer* screane, GameWindowBuffer* textture, GameWindowBuffer* depth) {
	//printf("hej\n");
	Vec_1x3 pos = mesh->pos;
	Vec_1x3 rot = mesh->rot;
	Vec_1x3 scale = mesh->scale;

	float h = (float)screane->h;
	float w = (float)screane->w;
	float Zf = 1000;
	float Zn = 0.1;
	float FOV = 1.0 / tanf(90 * 0.5 / 180.0 * 3.1415);

	Mat4x4 mat_pro =
	{
		{(h / w) * FOV,0,0,0},
		{ 0,FOV,0,0 },
		{0,0,Zf / (Zf - Zn),1},
		{0,0,(-Zf * Zn) / (Zf - Zn),0 }
	};

	Mat4x4 mat_rot =
	{
		{
			cos(rot.y) * cos(rot.z),
			cos(rot.z) * sin(rot.x) * sin(rot.y) - cos(rot.x) * sin(rot.z),
			cos(rot.x) * cos(rot.z) * sin(rot.y) + sin(rot.x) * sin(rot.z),
			0
		},
		{
			cos(rot.y) * sin(rot.z),
			sin(rot.x) * sin(rot.y) * sin(rot.z) + cos(rot.x) * cos(rot.z),
			cos(rot.x) * sin(rot.y) * sin(rot.z) - cos(rot.z) * sin(rot.x),
			0
		},
		{
			-sin(rot.y),
			sin(rot.x) * cos(rot.y),
			cos(rot.x) * cos(rot.y),
			0
		},
		{0, 0, 0, 1}
	};

	Mat4x4 mat_rot_cam =
	{
		{
			cos(cam->rot.y) * cos(cam->rot.z),
			cos(cam->rot.z) * sin(cam->rot.x) * sin(cam->rot.y) - cos(cam->rot.x) * sin(cam->rot.z),
			cos(cam->rot.x) * cos(cam->rot.z) * sin(cam->rot.y) + sin(cam->rot.x) * sin(cam->rot.z),
			0
		},
		{
			cos(cam->rot.y) * sin(cam->rot.z),
			sin(cam->rot.x) * sin(cam->rot.y) * sin(cam->rot.z) + cos(cam->rot.x) * cos(cam->rot.z),
			cos(cam->rot.x) * sin(cam->rot.y) * sin(cam->rot.z) - cos(cam->rot.z) * sin(cam->rot.x),
			0
		},
		{
			-sin(cam->rot.y),
			sin(cam->rot.x) * cos(cam->rot.y),
			cos(cam->rot.x) * cos(cam->rot.y),
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


	float l;
	Vec_1x3 up = { 0,1,0 };
	Vec_1x3 target = { 0,0,1 };
	Vec_1x3 lookDir = { 0,0,1 };
	//printf("hej: %f\n", lookDir.x);
	target = (Vec_1x3)add_vec_direct(cam->pos, lookDir);

	Vec_1x3 new_forword = sub_vec_direct(target, cam->pos);
	l = Gel_Vec_lenght(new_forword);
	new_forword = (Vec_1x3)NormalizeVec(new_forword, l);

	l = dot(up, new_forword);
	Vec_1x3 a = mul_vec_direct(new_forword, l);
	Vec_1x3 newUp = sub_vec_direct(up, a);
	l = Gel_Vec_lenght(newUp);
	newUp = (Vec_1x3)NormalizeVec(newUp, l);

	Vec_1x3 newRight = EdgeCrossAsVec(newUp, new_forword);

	Mat4x4 view = {
		{ newRight.x,     newRight.y,     newRight.z,     -dot(newRight, cam->pos) },
		{ newUp.x,        newUp.y,        newUp.z,        -dot(newUp, cam->pos) },
		{ new_forword.x,  new_forword.y,  new_forword.z,  -dot(new_forword, cam->pos) },
		{ 0,              0,              0,              1 }
	};

	//Mat4x4 view = {
	//	{ newRight.x,  newUp.x ,   new_forword.x  ,  0    },
	//	{ newRight.y , newUp.y,      new_forword.y,   0      },
	//	{ newRight.z,    newUp.z  ,  new_forword.z,   0},
	//	{ -dot(newRight, cam.pos),              -dot(newUp, cam.pos),              -dot(new_forword, cam.pos),              1 }
	//};
	Vec_1x4 temp_look_dir = { lookDir.x, lookDir.y, lookDir.z, 1 };
	temp_look_dir = multily_by_matrix_advanced(temp_look_dir, mat_rot_cam);
	cam->lookDir = (Vec_1x3){ temp_look_dir.x * -1, temp_look_dir.y * -1, temp_look_dir.z };

	// fix this it should be done vie one matrix in one function call # with no extra variables
	for (int i = 0; i < mesh->vert_count; ) {
		Vec_1x4 temp = { (mesh->verst_reference + i)->x,(mesh->verst_reference + i)->y,(mesh->verst_reference + i)->z,1 };
		temp = multily_by_matrix_advanced(temp, mat_rot);
		temp = multily_by_matrix_advanced(temp, mat_scale_ofste);
		temp = multily_by_matrix_advanced(temp, view);
		// t¹ toracjê to najpierw yaw
		// a potem pitch x
		temp = multily_by_matrix_advanced(temp, mat_rot_cam);
		temp = multily_by_matrix_advanced(temp, mat_pro);
	
		mesh->verts[i] = (Vec_1x3){ temp.x/temp.w, temp.y / temp.w, temp.z};
		i++;
	}
	Vec_1x3 edge1;
	Vec_1x3 edge2;
	Vec_1x3 Normal;
	
	struct triInt white = { 255,255,255 };

	for (int i = 0; i < mesh->tri_count; ) {
		edge1 = (Vec_1x3)sub_vec((mesh->tris + i)->verts.b, (mesh->tris + i)->verts.a);
		edge2 = (Vec_1x3)sub_vec((mesh->tris + i)->verts.c, (mesh->tris + i)->verts.a);
		Normal = (Vec_1x3)EdgeCrossAsVec(edge1, edge2);

		l = (float)Gel_Vec_lenght(Normal);
		Normal = (Vec_1x3)NormalizeVec(Normal, l);

		
	
		if (Normal.z > 0) {

			// to jest jak to mówi kuba "¿eŸba"
			float light = Normal.z * 8 > 1 ? 1 : Normal.z * 8;
			Triangle_clip(mesh->tris + i, 0, screane, textture, depth, light);
			
		}
		i++;
	}
}

void redner_sceane(Sceane* sceane, GameWindowBuffer* screane, GameWindowBuffer* texture, GameWindowBuffer* depth) {


	// oblicz wielk¹ macie¿
	for (int i = 0; i < sceane->Obj_count;) {

		redner_mesh(sceane->objects[i], &sceane->camera, screane, texture, depth);
		i++;
	}
};

