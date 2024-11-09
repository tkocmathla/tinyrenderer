#ifndef TINYRENDERER_DRAW_H
#define TINYRENDERER_DRAW_H

#include <math.h>
#include <stdlib.h>

#include "obj/obj.h"
#include "tgac/tgac.h"

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define SWAP(a, b, type) \
  do {                   \
    type tmp = a;        \
    a = b;               \
    b = tmp;             \
  } while (0)

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

// TODO: (inline) bounding box func

void sort_triangle_vertices(obj_vertex_t* v0, obj_vertex_t* v1, obj_vertex_t* v2) {
  if (v0->y > v1->y) SWAP(*v0, *v1, typeof(*v0));
  if (v0->y > v2->y) SWAP(*v0, *v2, typeof(*v0));
  if (v1->y > v2->y) SWAP(*v1, *v2, typeof(*v0));
}

obj_vertex_t* triangle_bounding_box(obj_vertex_t v0, obj_vertex_t v1, obj_vertex_t v2) {
  static obj_vertex_t box[2];
  // FIXME: get dims from tga state
  obj_vertex_t clamp = {.x = 800 - 1, .y = 800 - 1, .z = 0};

  // Lower-left
  box[0].x = MAX(0, MIN(MIN(v0.x, v1.x), v2.x));
  box[0].y = MAX(0, MIN(MIN(v0.y, v1.y), v2.y));

  // Upper-right
  box[1].x = MIN(clamp.x, MAX(MAX(v0.x, v1.x), v2.x));
  box[1].y = MIN(clamp.y, MAX(MAX(v0.y, v1.y), v2.y));

  return box;
}

obj_vertex_t cross(obj_vertex_t v0, obj_vertex_t v1) {
  obj_vertex_t v = {.x = v0.y * v1.z - v0.z * v1.y,
                    .y = v0.z * v1.x - v0.x * v1.z,
                    .z = v0.x * v1.y - v0.y * v1.x};
  return v;
}

// TODO: we need a higher-level 2d-vector type over obj_vertex_t
obj_vertex_t barycentric(obj_vertex_t v0, obj_vertex_t v1, obj_vertex_t v2, int x, int y) {
  obj_vertex_t _u = {.x = v2.x - v0.x, .y = v1.x - v0.x, .z = v0.x - x};
  obj_vertex_t _v = {.x = v2.y - v0.y, .y = v1.y - v0.y, .z = v0.y - y};
  obj_vertex_t uv = cross(_u, _v);

  // FIXME: sus
  if (fabs(uv.z) < 1) {
    obj_vertex_t bc = {.x = -1, .y = 1, .z = 1};
    return bc;
  }

  obj_vertex_t bc = {.x = 1.0f - (uv.x + uv.y) / uv.z, .y = uv.y / uv.z, .z = uv.x / uv.z};
  return bc;
}

void triangle(struct tgac_state_t* tga, obj_vertex_t v0, obj_vertex_t v1, obj_vertex_t v2,
              tgac_pixel_t color) {
  sort_triangle_vertices(&v0, &v1, &v2);
  obj_vertex_t* box = triangle_bounding_box(v0, v1, v2);

  for (int x = box[0].x; x < box[1].x; ++x) {
    for (int y = box[0].y; y < box[1].y; ++y) {
      obj_vertex_t bc = barycentric(v0, v1, v2, x, y);
      if (bc.x < 0 || bc.y < 0 || bc.z < 0) {
        continue;
      }
      tgac_set(tga, x, y, color);
    }
  }
}

#endif  // TINYRENDERER_DRAW_H
