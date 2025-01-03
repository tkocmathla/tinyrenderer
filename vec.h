#ifndef TINYRENDERER_VEC_H
#define TINYRENDERER_VEC_H

#include <math.h>

typedef struct {
  int x;
  int y;
} vec2i;

typedef struct {
  float x;
  float y;
} vec2f;

typedef struct {
  int x;
  int y;
  int z;
} vec3i;

typedef struct {
  float x;
  float y;
  float z;
} vec3f;

/// Convenience macros to create a vertex from coordinates.
#define V2I(x, y) ((vec2i){(x), (y)})
#define V3I(x, y, z) ((vec3i){(x), (y), (z)})
#define V2F(x, y) ((vec2f){(x), (y)})
#define V3F(x, y, z) ((vec3f){(x), (y), (z)})

vec3f vec_sub(vec3f v0, vec3f v1) {
  vec3f v = {.x = v0.x - v1.x, .y = v0.y - v1.y, .z = v0.z - v1.z};
  return v;
}

float vec_prod(vec3f v0, vec3f v1) { return v0.x * v1.x + v0.y * v1.y + v0.z * v1.z; }

vec3f vec_prod_scalar(vec3f v, float f) {
  vec3f _v = {.x = v.x * f, .y = v.y * f, .z = v.z * f};
  return _v;
}

/// Returns a new vector that is the cross product of the arguments.
vec3f vec_cross(vec3f v0, vec3f v1) {
  vec3f v = {.x = v0.y * v1.z - v0.z * v1.y,
             .y = v0.z * v1.x - v0.x * v1.z,
             .z = v0.x * v1.y - v0.y * v1.x};
  return v;
}

float vec_normal(vec3f v) { return sqrt(v.x * v.x + v.y * v.y + v.z * v.z); }

vec3f vec_normalize(vec3f v) { return vec_prod_scalar(v, (1.0f / vec_normal(v))); }

#endif  // TINYRENDERER_VEC_H
