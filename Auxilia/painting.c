#include "triangle.h"
#include "window_handle_mine.h"
#include "Vec_math.h"
#include "painting.h"

#include <stdio.h>

#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define TOP_LEFT_OFFSET(a) (a.y < 0 || (a.y == 0 && a.x > 0) ? 0 : -1)
#define EdgeCROSSZ(ab, ap) (ab.x* ap.y - ab.y * ap.x)
#define GET_SAFE(buffer, x, y, index) (x < 0 || y < 0 || x >= buffer->w || y >= buffer->h ? (struct triInt){128, 0, 128} : (struct triInt){buffer->memory[index], buffer->memory[index+3], buffer->memory[index+2]})
#define GET_SAFE_INT(buffer_z, x, y, index) (x < 0 || y < 0 || x >= buffer_z->w || y >= buffer_z->h ? 9999999 : ((int*)(buffer_z->memory))[index])

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
			index = 4*(x + y * w);
			screan[index] =   0;
			screan[index+1] = 0;
			screan[index+2] = 0;


			deepth_map[index] = 0;
			deepth_map[index + 1] = 0;
			deepth_map[index + 2] = 0;
			deepth_map[index + 3] = 0;
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

void Draw_triangle(Trianle* tri, GameWindowBuffer* screan, GameWindowBuffer* texture, GameWindowBuffer* depth_test) {
	
	Vec_1x3 v0 = tri->verts.a;
	Vec_1x3 v1 = tri->verts.b;
	Vec_1x3 v2 = tri->verts.c;

	int TEXTURE_widgt = texture->w;

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

	double alpha;
	double beta;
	double gamma;

	int x_text;
	int y_text;
	char is_in;

	//int deepth;
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

				x_text = tri->UVs.a.x * alpha + tri->UVs.b.x * beta + tri->UVs.c.x * gamma;
				y_text = tri->UVs.a.y * alpha + tri->UVs.b.y * beta + tri->UVs.c.y * gamma;

				int deepth = tri->verts.a.z * alpha + tri->verts.b.z * beta + tri->verts.c.z * gamma +1;
				index = 4 * (x_text + y_text * TEXTURE_widgt);
				int index_idn = (x + y * depth_test->w);
				//printf("deepth %d, %d\n", GET_SAFE_INT(depth_test, x, y, index / 4));
				//printf("deepth :%d kk %d color:%u\n", deepth, GET_SAFE_INT(depth_test, x, y, index_idn), GET_SAFE(texture, x_text, y_text, index).R);

				if (GET_SAFE_INT(depth_test, x, y, index_idn) < deepth) {
					drawAtSafe(screan, x, y, GET_SAFE(texture, x_text, y_text, index));
					//drawAtSafe(screan, x, y, (struct triInt) {0,deepth,0});
					((int*)(depth_test->memory))[index_idn] = deepth;
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
