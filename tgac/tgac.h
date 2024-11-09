// Intro: https://paulbourke.net/dataformats/tga
// Spec: https://www.dca.fee.unicamp.br/~martino/disciplinas/ea978/tgaffs.pdf
#ifndef TINYRENDERER_TGAC_TGAC_H
#define TINYRENDERER_TGAC_TGAC_H

#include <assert.h>
#include <stdint.h>
#include <stdio.h>

typedef uint8_t tgac_byte_t;
typedef uint16_t tgac_short_t;

/// Opaque state object.
struct tgac_state_t;

/// TGA image type.
typedef enum {
  TGAC_NONE = 0,
  // Uncompressed types.
  TGAC_COLOR_MAPPED = 1,
  TGAC_TRUE_COLOR = 2,
  TGAC_BLACK_WHITE = 3,
  // Run-length-encoded types.
  TGAC_RLE_COLOR_MAPPED = 9,
  TGAC_RLE_TRUE_COLOR = 10,
  TGAC_RLE_BLACK_WHITE = 11,
} tgac_image_type_t;

/// Pixel type.
typedef struct {
  tgac_byte_t r;  ///< Red
  tgac_byte_t g;  ///< Green
  tgac_byte_t b;  ///< Blue
  tgac_byte_t a;  ///< Alpha
} tgac_pixel_t;

static const tgac_pixel_t TGAC_RED = {.r = 255, .g = 0, .b = 0, .a = 255};
static const tgac_pixel_t TGAC_GREEN = {.r = 0, .g = 255, .b = 0, .a = 255};
static const tgac_pixel_t TGAC_BLUE = {.r = 0, .g = 0, .b = 255, .a = 255};
static const tgac_pixel_t TGAC_BLACK = {.r = 0, .g = 0, .b = 0, .a = 255};
static const tgac_pixel_t TGAC_WHITE = {.r = 255, .g = 255, .b = 255, .a = 255};

/**
 * Initializes a new TGA image.
 *
 * @param[in] type TGA image type.
 * @param[in] width Image width in pixels.
 * @param[in] height Image height in pixels.
 * @param[in] background An optional color used to initialize the image.
 */
struct tgac_state_t *tgac_init(tgac_image_type_t type, tgac_short_t width, tgac_short_t height,
                               const tgac_pixel_t *background);

/* Sets the pixel at (x, y) to `pixel'. */
void tgac_set(struct tgac_state_t *tga, tgac_short_t x, tgac_short_t y, tgac_pixel_t pixel);

/* Writes the TGA file described by `tga' to `file'. */
void tgac_write(FILE *file, struct tgac_state_t *tga);

/* Pretty-prints the TGA header from `tga' to stdout. */
void tgac_dump(struct tgac_state_t *tga);

#endif  // TINYRENDERER_TGAC_TGAC_H
