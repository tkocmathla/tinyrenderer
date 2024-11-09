#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

#include "draw.h"
#include "obj/obj.h"
#include "tgac/tgac.h"

void draw_obj(const char *file) {
  int width = 800;
  int height = 800;
  struct tgac_state_t *tga = tgac_init(TGAC_TRUE_COLOR, width, height, &TGAC_WHITE);

  FILE *obj_file = fopen(file, "r");
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

    // Convert world coords to screen coords
    obj_vertex_t v0s = *v0;
    v0s.x = (v0->x + 1.0) * halfw;
    v0s.y = (v0->y + 1.0) * halfh;
    obj_vertex_t v1s = *v1;
    v1s.x = (v1->x + 1.0) * halfw;
    v1s.y = (v1->y + 1.0) * halfh;
    obj_vertex_t v2s = *v2;
    v2s.x = (v2->x + 1.0) * halfw;
    v2s.y = (v2->y + 1.0) * halfh;

    tgac_pixel_t color = {.r = rand() % 255, .g = rand() % 255, .b = rand() % 255, .a = 255};
    triangle(tga, v0s, v1s, v2s, color);
  }

  FILE *tga_file = fopen("obj_test.tga", "wb");
  tgac_write(tga_file, tga);
  fclose(tga_file);
}

int main(int argc, char **argv) {
  draw_obj("data/african_head.obj");

  struct tgac_state_t *tga = tgac_init(TGAC_TRUE_COLOR, 250, 250, &TGAC_BLACK);

  triangle(tga, v(10, 70, 0), v(50, 160, 0), v(70, 80, 0), TGAC_RED);
  triangle(tga, v(180, 50, 0), v(150, 1, 0), v(70, 180, 0), TGAC_WHITE);
  triangle(tga, v(180, 150, 0), v(120, 160, 0), v(130, 180, 0), TGAC_GREEN);

  FILE *tga_file = fopen("triangle_test.tga", "wb");
  tgac_write(tga_file, tga);
  fclose(tga_file);
}
