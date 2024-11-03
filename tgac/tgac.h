// Intro: https://paulbourke.net/dataformats/tga
// Spec: https://www.dca.fee.unicamp.br/~martino/disciplinas/ea978/tgaffs.pdf

#include <assert.h>
#include <stdint.h>
#include <stdio.h>

typedef uint8_t tgac_byte_t;
typedef uint16_t tgac_short_t;

/**
 * Opaque state object.
 */
struct tgac_state_t;

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

typedef struct {
    tgac_byte_t r; // red
    tgac_byte_t g; // green
    tgac_byte_t b; // blue
    tgac_byte_t a; // alpha (opacity percentage)
} tgac_pixel_t;

/* Initializes a new TGA image. */
struct tgac_state_t* tgac_init(tgac_image_type_t type, tgac_short_t width, tgac_short_t height);

/* Sets the pixel at (x, y) to `pixel', where `x' is the column and `y' is the row. */
void tgac_set(struct tgac_state_t* tga, tgac_short_t x, tgac_short_t y, tgac_pixel_t pixel);

/* Writes the TGA file described by `tga' to `file'. */
void tgac_write(FILE *file, struct tgac_state_t* tga);

/* Pretty-prints the TGA header to stdout. */
void tgac_dump(struct tgac_state_t* tga);
