#include "tgac.h"

#include <stdio.h>
#include <stdlib.h>

//////////////////////////////////
// Internal utility definitions //
//////////////////////////////////

typedef struct __attribute__((packed)) {
  tgac_byte_t id_length;
  tgac_byte_t color_map_type;
  tgac_byte_t image_type;

  // Color map spec.
  tgac_short_t color_map_origin;
  tgac_short_t color_map_length;
  tgac_byte_t color_map_depth;

  // Image spec.
  tgac_short_t x_origin;
  tgac_short_t y_origin;
  tgac_short_t width;
  tgac_short_t height;
  tgac_byte_t bits_per_pixel;
  tgac_byte_t image_descriptor;
} tgac_header_t;

static_assert(sizeof(tgac_header_t) == 18,
              "tgac header size must be exactly 18 bytes");

typedef struct tgac_state_t {
  tgac_header_t *header;
  tgac_pixel_t *image;
} tgac_state_t;

// Reads and returns a short integer from `file'.
tgac_short_t tgac_read_short(FILE *file) {
  tgac_short_t value = 0;
  value = fgetc(file);
  value |= fgetc(file) << 8;
  return value;
}

// Writes `value' into `file' using little-endian ordering.
void tgac_write_short(FILE *file, tgac_short_t value) {
  fputc(value & 0xFF, file);
  fputc((value >> 8) & 0xFF, file);
}

// Writes `header' in TGA format into `file'.
void tgac_write_header(FILE *file, tgac_header_t *header) {
  rewind(file);
  fputc(header->id_length, file);
  fputc(header->color_map_type, file);
  fputc(header->image_type & 0xFF, file);
  tgac_write_short(file, header->color_map_origin);
  tgac_write_short(file, header->color_map_length);
  fputc(header->color_map_depth, file);
  tgac_write_short(file, header->x_origin);
  tgac_write_short(file, header->y_origin);
  tgac_write_short(file, header->width);
  tgac_write_short(file, header->height);
  fputc(header->bits_per_pixel, file);
  fputc(header->image_descriptor, file);
}

// Writes the image pixels from `tga' into `file'.
void tgac_write_pixels(FILE *file, tgac_state_t *tga) {
  tgac_header_t *header = tga->header;
  tgac_pixel_t *image = tga->image;
  assert(header->image_type == TGAC_TRUE_COLOR && "unsupported image type");

  int offset = sizeof(tgac_header_t) + header->id_length +
               (header->color_map_type * header->color_map_length);
  fseek(file, offset, SEEK_SET);

  for (int i = 0; i < header->height * header->width; i++) {
    fputc(image[i].b, file);
    fputc(image[i].g, file);
    fputc(image[i].r, file);
    fputc(image[i].a, file);
  }
}

//////////////////////////
// Public API functions //
//////////////////////////

tgac_state_t *tgac_init(tgac_image_type_t type, tgac_short_t width,
                        tgac_short_t height, const tgac_pixel_t *background) {
  tgac_state_t *tga = malloc(sizeof(tgac_state_t));
  tga->header = calloc(1, sizeof(tgac_header_t));
  tga->header->image_type = type;
  tga->header->width = width;
  tga->header->height = height;
  tga->header->bits_per_pixel = 32;
  tga->image = calloc(width * height, sizeof(tgac_pixel_t));

  // Initialize canvas to background color.
  if (background) {
    for (int y = 0; y < height; ++y) {
      for (int x = 0; x < width; ++x) {
        tgac_set(tga, x, y, *background);
      }
    }
  }
  return tga;
}

void tgac_set(tgac_state_t *tga, tgac_short_t x, tgac_short_t y,
              tgac_pixel_t pixel) {
  tga->image[x + (y * tga->header->height)] = pixel;
}

void tgac_write(FILE *file, tgac_state_t *tga) {
  tgac_write_header(file, tga->header);
  tgac_write_pixels(file, tga);
}

void tgac_dump(tgac_state_t *tga) {
  tgac_header_t *header = tga->header;

  printf("header {\n");
  printf("  id_length: %d\n", header->id_length);
  printf("  color_map_type: %d\n", header->color_map_type);
  printf("  image_type: %d\n", header->image_type);

  // Color map spec
  printf("  color_map_origin: %d\n", header->color_map_origin);
  printf("  color_map_length: %d\n", header->color_map_length);
  printf("  color_map_depth: %d\n", header->color_map_depth);

  // Image spec
  printf("  x_origin: %d\n", header->x_origin);
  printf("  y_origin: %d\n", header->y_origin);
  printf("  width: %d\n", header->width);
  printf("  height: %d\n", header->height);
  printf("  bits_per_pixel: %d\n", header->bits_per_pixel);
  printf("  image_descriptor: %d\n", header->image_descriptor);
  printf("}\n");
}
