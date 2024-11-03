#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

#include "obj/obj.h"
#include "tgac/tgac.h"

#define SWAP(a, b, type)                                                       \
  do {                                                                         \
    type tmp = a;                                                              \
    a = b;                                                                     \
    b = tmp;                                                                   \
  } while (0)

void line(int x0, int y0, int x1, int y1, struct tgac_state_t *tga,
          tgac_pixel_t pixel) {
  bool steep = false;
  if (abs(x0 - x1) < abs(y0 - y1)) {
    SWAP(x0, y0, typeof(x0));
    SWAP(x1, y1, typeof(x1));
    steep = true;
  }
  if (x0 > x1) {
    SWAP(x0, x1, typeof(x0));
    SWAP(y0, y1, typeof(y0));
  }
  int dx = x1 - x0;
  int dy = y1 - y0;
  int derr = abs(dy) * 2;
  int err = 0;
  int y = y0;
  for (float x = x0; x < x1; x++) {
    if (steep) {
      tgac_set(tga, y, x, pixel);
    } else {
      tgac_set(tga, x, y, pixel);
    }
    err += derr;
    if (err > dx) {
      y += (y1 > y0 ? 1 : -1);
      err -= dx * 2;
    }
  }
}

int main(int argc, char **argv) {
  int width = 800;
  int height = 800;
  struct tgac_state_t *tga = tgac_init(TGAC_TRUE_COLOR, width, height, &TGAC_WHITE);

  FILE *obj_file = fopen("data/african_head.obj", "r");
  assert(obj_file);
  obj_model_t *obj = obj_load_model(obj_file);
  fclose(obj_file);

  float halfw = width / 2.0;
  float halfh = height / 2.0;
  for (int i = 0; i < obj->nfaces; i++) {
    obj_face_t *face = &obj->faces[i];
    obj_vertex_t *v0 = &obj->vertices[face->v0];
    obj_vertex_t *v1 = &obj->vertices[face->v1];
    obj_vertex_t *v2 = &obj->vertices[face->v2];

    line((v0->x + 1.0) * halfw, (v0->y + 1.0) * halfh, (v1->x + 1.0) * halfw,
         (v1->y + 1.0) * halfh, tga, TGAC_BLACK);
    line((v1->x + 1.0) * halfw, (v1->y + 1.0) * halfh, (v2->x + 1.0) * halfw,
         (v2->y + 1.0) * halfh, tga, TGAC_BLACK);
    line((v2->x + 1.0) * halfw, (v2->y + 1.0) * halfh, (v0->x + 1.0) * halfw,
         (v0->y + 1.0) * halfh, tga, TGAC_BLACK);
  }

  FILE *tga_file = fopen("test.tga", "wb");
  tgac_write(tga_file, tga);
  fclose(tga_file);
}
