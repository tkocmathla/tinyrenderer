#ifndef TINYRENDERER_DRAW_H
#define TINYRENDERER_DRAW_H

#include <math.h>
#include <stdlib.h>

#include "obj/obj.h"
#include "tgac/tgac.h"
#include "util.h"
#include "vec.h"

void line(struct tgac_state_t* tga, int x0, int y0, int x1, int y1, tgac_pixel_t color) {
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
      tgac_set(tga, y, x, color);
    } else {
      tgac_set(tga, x, y, color);
    }
    err += derr;
    if (err > dx) {
      y += (y1 > y0 ? 1 : -1);
      err -= dx * 2;
    }
  }
}

void sort_triangle_vertices(vec3f* v0, vec3f* v1, vec3f* v2) {
  if (v0->y > v1->y) SWAP(*v0, *v1, typeof(*v0));
  if (v0->y > v2->y) SWAP(*v0, *v2, typeof(*v0));
  if (v1->y > v2->y) SWAP(*v1, *v2, typeof(*v0));
}

vec2f* triangle_bounding_box(struct tgac_state_t* tga, vec3f v0, vec3f v1, vec3f v2) {
  tgac_metadata_t meta = tgac_metadata(tga);
  vec2f clamp = {.x = meta.width - 1.0f, .y = meta.height - 1.0f};

  static vec2f box[2];
  box[0].x = MAX(0, MIN(MIN(v0.x, v1.x), v2.x));
  box[0].y = MAX(0, MIN(MIN(v0.y, v1.y), v2.y));
  box[1].x = MIN(clamp.x, MAX(MAX(v0.x, v1.x), v2.x));
  box[1].y = MIN(clamp.y, MAX(MAX(v0.y, v1.y), v2.y));

  return box;
}

vec3f barycentric(vec3f v0, vec3f v1, vec3f v2, int x, int y) {
  vec3f _u = {.x = (float)v2.x - v0.x, .y = (float)v1.x - v0.x, .z = (float)v0.x - x};
  vec3f _v = {.x = (float)v2.y - v0.y, .y = (float)v1.y - v0.y, .z = (float)v0.y - y};
  vec3f uv = vec_cross(_u, _v);

  if (fabs(uv.z) < 1) {
    vec3f bc = {.x = -1, .y = -1, .z = -1};
    return bc;
  }

  vec3f bc = {.x = 1.0f - (uv.x + uv.y) / uv.z, .y = uv.y / uv.z, .z = uv.x / uv.z};
  return bc;
}

void triangle(struct tgac_state_t* tga, float* zbuffer, vec3f v0, vec3f v1, vec3f v2,
              tgac_pixel_t color) {
  tgac_metadata_t meta = tgac_metadata(tga);
  sort_triangle_vertices(&v0, &v1, &v2);
  vec2f* box = triangle_bounding_box(tga, v0, v1, v2);

  for (int x = box[0].x; x < box[1].x; ++x) {
    for (int y = box[0].y; y < box[1].y; ++y) {
      vec3f bc = barycentric(v0, v1, v2, x, y);
      if (bc.x < 0 || bc.y < 0 || bc.z < 0) {
        continue;
      }
      float z = 0;
      z += bc.z * v0.z;
      z += bc.z * v1.z;
      z += bc.z * v2.z;
      int i = x + y * meta.width;
      if (zbuffer[i] < z) {
        zbuffer[i] = z;
        tgac_set(tga, x, y, color);
      }
    }
  }
}

#endif  // TINYRENDERER_DRAW_H
