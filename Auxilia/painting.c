#include "triangle.h"
#include "window_handle_mine.h"
#include "Vec_math.h"
#include "painting.h"
#include <math.h>
#include <stdio.h>

#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define TOP_LEFT_OFFSET(a) (a.y < 0 || (a.y == 0 && a.x > 0) ? 0 : 0)
#define EdgeCROSSZ(ab, ap) (ab.x* ap.y - ab.y * ap.x)
#define GET_SAFE(buffer, x, y, index, light) (x < 0 || y < 0 || x >= buffer->w || y >= buffer->h ? (struct triInt){128, 0, 128} : (struct triInt){buffer->memory[index] * light, buffer->memory[index+3]*light, buffer->memory[index+2]*light})
#define GET_SAFE_FLOAT(buffer_z, x, y, index) (x < 0 || y < 0 || x >= buffer_z->w || y >= buffer_z->h ? -2147483647 : ((float*)(buffer_z->memory))[index])

void drawAtSafe(struct GameWindowBuffer* buffer, int x, int y, struct triInt color) {
	if (x < 0 || y < 0 || x >= buffer->w || y >= buffer->h) return;
	int index = 4 * (x + y * buffer->w);
	buffer->memory[index + 0] = color.B;
	buffer->memory[index + 1] = color.G;
	buffer->memory[index + 2] = color.R;
}

void clear(GameWindowBuffer* buffer, GameWindowBuffer* deepth) {
	int index;
	short w = buffer->w;
	short h = buffer->h;

	unsigned char* screan = buffer->memory;
	unsigned char* deepth_map = deepth->memory;

	for (int x = 0; x < w; x++) {
		for (int y = 0; y < h; y++) {
			index = (x + y * w);
			((int*)screan)[index] = 0;
			// to musi byæ float to jest zbyt g³emboko aby by³o inaczej 
			//printf("hej\n");
			((float *)deepth_map)[index] = 123456792.000000;
			//printf("hej22\n");
			//deepth_map[index + 1] = 0;
			//deepth_map[index + 2] = 0;
			//deepth_map[index + 3] = 0;

			//drawAtSafe(buffer, x, y, (struct triInt) { 0, 0, 0 });
		}
	}
}

struct triInt get_Safe(struct GameWindowBuffer* buffer, int x, int y) {
	if (x < 0 || y < 0 || x >= buffer->w || y >= buffer->h) return;

	int index = 4 * (x + y * buffer->w);
	return (struct triInt) {
		buffer->memory[index + 2],
		buffer->memory[index + 1],
		buffer->memory[index ]
	};
}

void Draw_triangle(Trianle* tri, GameWindowBuffer* screan, GameWindowBuffer* texture, GameWindowBuffer* depth_test, float light) {


	Vec_1x3 v0 = *tri->verts.a;
	Vec_1x3 v1 = *tri->verts.c;
	Vec_1x3 v2 = *tri->verts.b;

	v0.y = ceil((v0.y + 1) * 0.5 * (float)screan->h);
	v1.y = ceil((v1.y + 1) * 0.5 * (float)screan->h);
	v2.y = ceil((v2.y + 1) * 0.5 * (float)screan->h);
	
	v0.x = ceil((v0.x + 1) * 0.5 * (float)screan->w);
	v1.x = ceil((v1.x + 1) * 0.5 * (float)screan->w);
	v2.x = ceil((v2.x + 1) * 0.5 * (float)screan->w);

	int x_min = (int)MIN(MIN(v0.x, v1.x), v2.x);
    int y_min = (int)MIN(MIN(v0.y, v1.y), v2.y);
    int x_max = (int)MAX(MAX(v0.x, v1.x), v2.x);
    int y_max = (int)MAX(MAX(v0.y, v1.y), v2.y);

	int delta_w0_col = v1.y - v2.y;
	int delta_w1_col = v2.y - v0.y;
	int delta_w2_col = v0.y - v1.y;
	Vec_1x2 p = { x_min, y_min };

	Vec_1x2 ab = {v2.x - v1.x, v2.y - v1.y};
	Vec_1x2 bc = {v0.x - v2.x, v0.y - v2.y};
	Vec_1x2 ca = { v1.x - v0.x, v1.y - v0.y };

	int field = (int)EdgeCROSSZ(ab, bc);

	Vec_1x2 ap = { p.x - v1.x, p.y - v1.y };
	Vec_1x2 bp = { p.x - v2.x, p.y - v2.y };
	Vec_1x2 cp = { p.x - v0.x, p.y - v0.y };

	int w0_baise = (int)EdgeCROSSZ(ab, ap) + TOP_LEFT_OFFSET(bc);
	int w1_baise = (int)EdgeCROSSZ(bc, bp) + TOP_LEFT_OFFSET(ca);
	int w2_baise = (int)EdgeCROSSZ(ca, cp) + TOP_LEFT_OFFSET(ab);

	int w0;
	int w1;
	int w2;

	float alpha;
	float beta;
	float gamma;

	int x_text;
	int y_text;
	char is_in;

	int index;

	for (int y = y_min; y <= y_max; y++) {

		w0 = w0_baise;
		w1 = w1_baise;
		w2 = w2_baise;
		is_in = 0;

		for (int x = x_min; x <= x_max; x++) {
			if (w0 >= 0 && w1 >= 0 && w2 >= 0) {
				alpha = (float)w0 / field;
				beta = (float)w1 / field;
				gamma = (float)w2 / field;

				x_text = tri->UVs.a.x * alpha + tri->UVs.c.x * beta + tri->UVs.b.x * gamma;
				y_text = tri->UVs.a.y * alpha + tri->UVs.c.y * beta + tri->UVs.b.y * gamma;

				float deepth = tri->verts.a->z * alpha + tri->verts.b->z * beta + tri->verts.c->z * gamma +1;
				index = 4 * (x_text + y_text * texture->w);
				int index_idn = (x + y * depth_test->w);
				//printf("deepth %d, %d\n", GET_SAFE_INT(depth_test, x, y, index / 4));
				//printf("deepth :%d kk %d color:%u\n", deepth, GET_SAFE_INT(depth_test, x, y, index_idn), GET_SAFE(texture, x_text, y_text, index).R);
				//GET_SAFE_INT(depth_test, x, y, index_idn) < deepth
				if (GET_SAFE_FLOAT(depth_test, x, y, index_idn) > deepth) {
					drawAtSafe(screan, x, y, GET_SAFE(texture, x_text, y_text, index,light));
					//drawAtSafe(screan, x, y, (struct triInt){ deepth , deepth, deepth});
					//drawAtSafe(screan, x, y, (struct triInt) {0,deepth,0});
					((float*)(depth_test->memory))[index_idn] = deepth;
				}
				is_in = 1;
			} else if (is_in) {
				break;
			}

			w0 += delta_w0_col;
			w1 += delta_w1_col;
			w2 += delta_w2_col;

		}
		w0_baise += ab.x;
		w1_baise += bc.x;
		w2_baise += ca.x;
	}

}

void Draw_line(GameWindowBuffer* screen, int x0, int y0, int x1, int y1, struct triInt color) {

	int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
	int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
	int err = dx + dy, e2;

	while (1) {
		drawAtSafe(screen, x0, y0, color);
		if (x0 == x1 && y0 == y1) break;
		e2 = 2 * err;
		if (e2 >= dy) { err += dy; x0 += sx; }
		if (e2 <= dx) { err += dx; y0 += sy; }
	}
}

void Draw_triangle_outline(Trianle* tri, GameWindowBuffer* screen, struct triInt color) {
	Vec_1x3 v0 = (*tri->verts.a);
	Vec_1x3 v1 = (*tri->verts.b);
	Vec_1x3 v2 = (*tri->verts.c);

	v0.y = (v0.y + 1) * 0.5 * (float)screen->h;
	v1.y = (v1.y + 1) * 0.5 * (float)screen->h;
	v2.y = (v2.y + 1) * 0.5 * (float)screen->h;

	v0.x = (v0.x + 1) * 0.5 * (float)screen->w;
	v1.x = (v1.x + 1) * 0.5 * (float)screen->w;
	v2.x = (v2.x + 1) * 0.5 * (float)screen->w;

	// Draw lines between the triangle vertices
	Draw_line(screen, (int)v0.x, (int)v0.y, (int)v1.x, (int)v1.y, color);
	Draw_line(screen, (int)v1.x, (int)v1.y, (int)v2.x, (int)v2.y, color);
	Draw_line(screen, (int)v2.x, (int)v2.y, (int)v0.x, (int)v0.y, color);
}
