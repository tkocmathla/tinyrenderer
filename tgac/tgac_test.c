#include "tgac.h"

#include <stdlib.h>

// TODO: error handling everywhere

int main(int argc, char **argv) {
  struct tgac_state_t *tga = tgac_init(TGAC_TRUE_COLOR, 16, 16);
  tgac_dump(tga);

  // Mark the center and corners red.
  tgac_pixel_t red = {.r = 255, .g = 0, .b = 0, .a = 255};
  tgac_set(tga, 0, 0, red);
  tgac_set(tga, 15, 0, red);
  tgac_set(tga, 7, 7, red);
  tgac_set(tga, 0, 15, red);
  tgac_set(tga, 15, 15, red);

  FILE *file = fopen("./test.tga", "wb");
  tgac_write(file, tga);
  fclose(file);

  return 0;
}
