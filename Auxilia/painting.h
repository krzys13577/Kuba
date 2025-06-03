#pragma once
#include "triangle.h"
#include "window_handle_mine.h"
#include "Vec_math.h"

struct triInt {
	char R;
	char G;
	char B;
} triInt;

void drawAtSafe(struct GameWindowBuffer* buffer, int x, int y, struct triInt color);

void Draw_triangle(Trianle* tri, GameWindowBuffer* screan, GameWindowBuffer* texture, GameWindowBuffer* depth_test);

void clear(struct GameWindowBuffer* buffer, GameWindowBuffer* deepth);
