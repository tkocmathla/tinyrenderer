#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    float x;
    float y;
    float z;
} obj_vertex_t;

typedef struct {
    int v0;
    int v1;
    int v2;
} obj_face_t;

typedef struct {
    obj_vertex_t* vertices;
    obj_face_t* faces;
    int nvertices;
    int nfaces;
} obj_model_t;

obj_model_t* obj_load_model(FILE* file) {
    // Read through the file once to get the count of items.
    obj_model_t* obj = malloc(sizeof(obj_model_t));
    assert(obj);
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
    obj->vertices = malloc(obj->nvertices * sizeof(obj_vertex_t));
    obj->faces = malloc(obj->nfaces * sizeof(obj_face_t));

    int vi = 0;
    int fi = 0;
    while (getline(&line, &len, file) != -1) {
        if (line[0] == 'v' && line[1] == ' ') {
            obj_vertex_t* v = &obj->vertices[vi];
            assert(sscanf(line, "v %f %f %f", &v->x, &v->y, &v->z) == 3);
            ++vi;
        } else if (line[0] == 'f') {
            obj_face_t* f = &obj->faces[fi];
            assert(sscanf(line, "f %d/%*d/%*d %d/%*d/%*d %d/%*d/%*d", &f->v0, &f->v1, &f->v2) == 3);
            // Convert 1-indexed values to 0-indexed.
            --f->v0;
            --f->v1;
            --f->v2;
            ++fi;
        }
    }

    return obj;
}
