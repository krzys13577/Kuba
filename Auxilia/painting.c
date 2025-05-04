#include "triangle.h"
#include "window_handle_mine.h"
#include "Vec_math.h"
#include "painting.h"
#include <stdio.h>
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define EdgeCROSSZ(ab, ap) (ab.x* ap.y - ab.y * ap.x)

void drawAtSafe(struct GameWindowBuffer* buffer, int x, int y, struct triInt color) {
	if (x < 0 || y < 0 || x >= buffer->w || y >= buffer->h) return;

	int index = 4 * (x + y * buffer->w);
	buffer->memory[index + 0] = color.B;
	buffer->memory[index + 1] = color.G;
	buffer->memory[index + 2] = color.R;
	buffer->memory[index + 3] = 0;
}

struct triInt get_Safe(struct GameWindowBuffer* buffer, int x, int y) {
	if (x < 0 || y < 0 || x >= buffer->w || y >= buffer->h) return;

	int index = 4 * (x + y * buffer->w);
	return (struct triInt) {
		buffer->memory[index + 2],
		buffer->memory[index + 1],
		buffer->memory[index + 0]
	};

	
}

void Draw_triangle(Trianle* tri, GameWindowBuffer* screan, GameWindowBuffer* texture) {
	Vec_1x3 v0 = tri->verts.a;
	Vec_1x3 v1 = tri->verts.b;
	Vec_1x3 v2 = tri->verts.c;
	int x_min = (int)MIN(MIN(v0.x, v1.x), v2.x);
    int y_min = (int)MIN(MIN(v0.y, v1.y), v2.y);
    int x_max = (int)MAX(MAX(v0.x, v1.x), v2.x);
    int y_max = (int)MAX(MAX(v0.y, v1.y), v2.y);

	int delta_w0_col = v1.y - v2.y;
	int delta_w1_col = v2.y - v0.y;
	int delta_w2_col = v0.y - v1.y;
	int delta_w0_row = v2.x - v1.x;
	int delta_w1_row = v0.x - v2.x;
	int delta_w2_row = v1.x - v0.x;
	Vec_1x2 p = { x_min, y_min };


	Vec_1x2 ab = {v2.x - v1.x, v2.y - v1.y};
	Vec_1x2 bc = {v0.x - v2.x, v0.y - v2.y};
	Vec_1x2 ca = { v1.x - v0.x, v1.y - v0.y };

	int field = (int)EdgeCROSSZ(ab, bc);
	int turn = 1;
	if (field < 0) {
		turn = -1;
	};


	Vec_1x2 ap = { p.x - v1.x, p.y - v1.y };
	Vec_1x2 bp = { p.x - v2.x, p.y - v2.y };
	Vec_1x2 cp = { p.x - v0.x, p.y - v0.y };

	int w0_baise = (int)EdgeCROSSZ(ab, ap);
	int w1_baise = (int)EdgeCROSSZ(bc, bp);
	int w2_baise = (int)EdgeCROSSZ(ca, cp);
	int w0;
	int w1;
	int w2;
	double alpha;
	double beta;
	double gamma;


	for (int y = y_min; y <= y_max; y++) {
		w0 = w0_baise;
		w1 = w1_baise;
		w2 = w2_baise;
		for (int x = x_min; x <= x_max; x++) {
			if (w0*turn >= 0 && w1* turn >= 0 && w2* turn >= 0) {
				alpha = (double)w0 / field;
				beta = (double)w1 / field;
				gamma = (double)w2 / field;
				int x_text = tri->UVs.a.x * alpha + tri->UVs.b.x * beta + tri->UVs.c.x * gamma;
				int y_text = tri->UVs.a.y * alpha + tri->UVs.b.y * beta + tri->UVs.c.y * gamma;
				drawAtSafe(screan, x, y, get_Safe(texture, x_text, y_text));

			}
			w0 += delta_w0_col;
			w1 += delta_w1_col;
			w2 += delta_w2_col;
		}
		w0_baise += delta_w0_row;
		w1_baise += delta_w1_row;
		w2_baise += delta_w2_row;
	}

}
