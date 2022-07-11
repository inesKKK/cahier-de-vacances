#ifndef SEAMCARVING_IMAGE_H
#define SEAMCARVING_IMAGE_H

#include <stdint.h>

struct image {
    uint8_t **at;
    int height;
    int width;
};

typedef struct image image;

image *image_new(int h, int w);
void image_delete(image *im);

void invert(image *im);
void binarize(image *im);
void flip_horizontal(image *im);
image *reduce_half(image *im);

image *image_load(char *filename);
int image_save(image *im, char *filename);


#endif //SEAMCARVING_IMAGE_H
