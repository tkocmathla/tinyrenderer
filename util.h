#ifndef TINYRENDERER_UTIL_H
#define TINYRENDERER_UTIL_H

#define MIN(a, b) ((a) < (b) ? (a) : (b))

#define MAX(a, b) ((a) > (b) ? (a) : (b))

#define SWAP(a, b, type) \
  do {                   \
    type tmp = a;        \
    a = b;               \
    b = tmp;             \
  } while (0)

#endif  // TINYRENDERER_UTIL_H
