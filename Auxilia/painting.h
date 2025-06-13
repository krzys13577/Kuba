#pragma once
#include "window_handle_mine.h"
#include "Vec_math.h"
#include "triangle.h"


struct triInt {
	char R;
	char G;
	char B;
} ;

void drawAtSafe(struct GameWindowBuffer* buffer, int x, int y, struct triInt color);

void Draw_triangle(Trianle* tri, GameWindowBuffer* screan, GameWindowBuffer* texture, GameWindowBuffer* depth_test, float light);

void clear(struct GameWindowBuffer* buffer, GameWindowBuffer* deepth);

void Draw_triangle_outline(Trianle* tri, GameWindowBuffer* screen, struct triInt color);
