#ifndef TINYRENDERER_OBJ_OBJ_H
#define TINYRENDERER_OBJ_OBJ_H

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vec.h"

typedef struct {
  int v0;
  int v1;
  int v2;
  int tex0;
  int tex1;
  int tex2;
} obj_face_t;

typedef struct {
  vec3f *vertices;
  vec3f *textures;
  obj_face_t *faces;
  int nvertices;
  int nfaces;
} obj_model_t;

obj_model_t *obj_load_model(FILE *file) {
  // Read through the file once to get the count of items.
  obj_model_t *obj = malloc(sizeof(obj_model_t));
  assert(obj);
  obj->nfaces = 0;
  obj->nvertices = 0;

  char *line = NULL;
  size_t len = 0;
  while (getline(&line, &len, file) != -1) {
    if (line[0] == 'v' && line[1] == ' ') {
      ++obj->nvertices;
    } else if (line[0] == 'f') {
      ++obj->nfaces;
    }
  }
  rewind(file);

  // Now parse the actual file content to build the obj model.
  obj->vertices = malloc(obj->nvertices * sizeof(vec3f));
  assert(obj->vertices);
  obj->textures = malloc(obj->nvertices * sizeof(vec3f));
  assert(obj->textures);
  obj->faces = malloc(obj->nfaces * sizeof(obj_face_t));
  assert(obj->faces);

  int vi = 0;
  int ti = 0;
  int fi = 0;
  while (getline(&line, &len, file) != -1) {
    if (line[0] == 'v' && line[1] == ' ') {
      vec3f *v = &obj->vertices[vi];
      assert(sscanf(line, "v %f %f %f", &v->x, &v->y, &v->z) == 3);
      ++vi;
    } else if (line[0] == 'v' && line[1] == 't') {
      vec3f *t = &obj->textures[ti];
      assert(sscanf(line, "vt  %f %f %f", &t->x, &t->y, &t->z) == 3);
      ++ti;
    } else if (line[0] == 'f') {
      obj_face_t *f = &obj->faces[fi];
      assert(sscanf(line, "f %d/%d/%*d %d/%d/%*d %d/%d/%*d", &f->v0, &f->tex0, &f->v1, &f->tex1,
                    &f->v2, &f->tex2) == 6);
      ++fi;

      // Convert 1-indexed values to 0-indexed.
      --f->v0;
      --f->v1;
      --f->v2;
      --f->tex0;
      --f->tex1;
      --f->tex2;
    }
  }

  return obj;
}

#endif  // TINYRENDERER_OBJ_OBJ_H
