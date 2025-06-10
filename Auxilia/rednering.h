#include "Vec_math.h"
#include "triangle.h"
#include "window_handle_mine.h"

typedef struct Camera {
	Vec_1x3 pos;
	Vec_1x3 rotation;
	Vec_1x3 up;
	Vec_1x3 forword;
	Vec_1x3 right;

} Camera;

typedef struct Sceane {
	int Obj_count;
	Mesh** objects;
	Camera camera;
} Sceane;

Sceane init_sceane(int count, Camera cam);

void redner_sceane(Sceane* sceane, GameWindowBuffer* screane, GameWindowBuffer* depth, GameWindowBuffer* texture);
