#include "image.h"

#include <png.h>
#include <stdio.h>
#include <stdlib.h>

image *image_load(char *filename) {
    unsigned char header[8];
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        printf("Le fichier %s n'existe pas.\n", filename);
        return NULL;
    }
    fread(header, 1, 8, fp);
    if (png_sig_cmp(header, 0, 8))
        return NULL;
    png_structp png_ptr =
        png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr == NULL)
        return NULL;
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL)
        return NULL;
    if (setjmp(png_jmpbuf(png_ptr)))
        return NULL;
    png_init_io(png_ptr, fp);
    png_set_sig_bytes(png_ptr, 8);
    png_read_info(png_ptr, info_ptr);
    int png_width = png_get_image_width(png_ptr, info_ptr);
    int png_height = png_get_image_height(png_ptr, info_ptr);
    png_byte color_type = png_get_color_type(png_ptr, info_ptr);
    if (color_type != PNG_COLOR_TYPE_GRAY)
        return NULL;
    png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    if (bit_depth != 8)
        return NULL;
    png_read_update_info(png_ptr, info_ptr);
    if (setjmp(png_jmpbuf(png_ptr)))
        return NULL;
    png_bytep *row_pointers = malloc(sizeof(png_bytep) * png_height);
    unsigned long nb_colors = png_get_rowbytes(png_ptr, info_ptr) / png_width;
    for (int i = 0; i < png_height; ++i) {
        row_pointers[i] = malloc(png_get_rowbytes(png_ptr, info_ptr));
    }
    png_read_image(png_ptr, row_pointers);
    image *im = image_new(png_height, png_width);
    for (int i = 0; i < png_height; i++) {
        png_bytep row = row_pointers[i];
        for (int j = 0; j < png_width; j++) {
            im->at[i][j] = row[j];
        }
    }
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    for (int i = 0; i < png_height; i++) {
        free(row_pointers[i]);
    }
    free(row_pointers);
    fclose(fp);
    return im;
}

int image_save(image *im, char *filename) {
    FILE *fp = fopen(filename, "wb");
    if (fp == NULL) {
        printf("Le fichier %s ne peut pas être ouvert.\n", filename);
        return 1;
    }
    png_structp png_ptr =
        png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr == NULL)
        return 1;
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL)
        return 1;
    if (setjmp(png_jmpbuf(png_ptr)))
        return 1;
    png_init_io(png_ptr, fp);
    if (setjmp(png_jmpbuf(png_ptr)))
        return 1;
    png_uint_32 png_width = im->width;
    png_uint_32 png_height = im->height;
    int bit_depth = 8;
    png_set_IHDR(png_ptr, info_ptr, png_width, png_height, bit_depth,
                 PNG_COLOR_TYPE_GRAY, PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
    png_write_info(png_ptr, info_ptr);
    if (setjmp(png_jmpbuf(png_ptr)))
        return 1;
    png_bytep *row_pointers = malloc(im->height * sizeof(png_bytep));
    for (int i = 0; i < im->height; i++) {
        row_pointers[i] = malloc(png_get_rowbytes(png_ptr, info_ptr));
    }
    for (int i = 0; i < im->height; i++) {
        png_bytep row = row_pointers[i];
        for (int j = 0; j < im->width; j++) {
            row[j] = im->at[i][j];
        }
    }
    png_write_image(png_ptr, row_pointers);
    if (setjmp(png_jmpbuf(png_ptr)))
        return 1;
    png_write_end(png_ptr, NULL);
    png_destroy_write_struct(&png_ptr, &info_ptr);
    for (int i = 0; i < im->height; i++) {
        free(row_pointers[i]);
    }
    free(row_pointers);
    return 0;
}
