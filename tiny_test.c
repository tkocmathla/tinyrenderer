#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

#include "draw.h"
#include "obj/obj.h"
#include "tgac/tgac.h"

void draw_obj(const char *file) {
  int width = 800;
  int height = 800;
  struct tgac_state_t *tga = tgac_init(TGAC_TRUE_COLOR, width, height, &TGAC_BLACK);

  FILE *obj_file = fopen(file, "rb");
  assert(obj_file);
  obj_model_t *obj = obj_load_model(obj_file);
  fclose(obj_file);

  float zbuffer[width * height];
  for (int i = 0; i < width * height; ++i) {
    zbuffer[i] = -INFINITY;
  }

  float halfw = width / 2.0;
  float halfh = height / 2.0;
  for (int i = 0; i < obj->nfaces; i++) {
    obj_face_t *face = &obj->faces[i];
    vec3f *v0w = &obj->vertices[face->v0];
    vec3f *v1w = &obj->vertices[face->v1];
    vec3f *v2w = &obj->vertices[face->v2];

    // Convert world coords to screen coords
    vec3f v0s = {.x = (v0w->x + 1.0) * halfw, .y = (v0w->y + 1.0) * halfh, .z = v0w->z};
    vec3f v1s = {.x = (v1w->x + 1.0) * halfw, .y = (v1w->y + 1.0) * halfh, .z = v1w->z};
    vec3f v2s = {.x = (v2w->x + 1.0) * halfw, .y = (v2w->y + 1.0) * halfh, .z = v2w->z};

    vec3f norm = vec_normalize(vec_cross(vec_sub(*v2w, *v0w), vec_sub(*v1w, *v0w)));
    vec3f light = {.x = 0, .y = 0, .z = -1};
    float intensity = vec_prod(norm, light);

    if (intensity > 0) {
      tgac_pixel_t color = {
          .r = intensity * 255, .g = intensity * 255, .b = intensity * 255, .a = 255};
      triangle(tga, zbuffer, v0s, v1s, v2s, color);
    }
  }

  FILE *tga_file = fopen("obj_test.tga", "wb");
  assert(tga_file);
  tgac_write(tga_file, tga);
  fclose(tga_file);
}

int main(int argc, char **argv) { draw_obj("data/african_head.obj"); }
