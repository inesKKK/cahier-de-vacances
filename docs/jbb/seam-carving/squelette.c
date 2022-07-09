#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define STBI_NO_FAILURE_STRINGS
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#include "stb_image.h"
#include "stb_image_write.h"

#include "seam_carving.h"

image *image_load(char *filename){
    int w, h, channels;
    uint8_t *data = stbi_load(filename, &w, &h, &channels, 0);
    if (!data) {
        fprintf(stderr, "Erreur de lecture.\n");
        stbi_failure_reason();
        exit(EXIT_FAILURE);
    }
    if (channels != 1){
        fprintf(stdout, "Pas une image en niveaux de gris.\n");
        exit(EXIT_FAILURE);
    }
    image *im = image_new(h, w);
    for (int i = 0; i < h; i++){
        for (int j = 0; j < w; j++){
            im->at[i][j] = data[j + i * w];
        }
    }
    free(data);
    return im;
}

void image_save(image *im, char *filename){
    int h = im->h;
    int w = im->w;
    int stride_length = w;
    uint8_t *data = malloc(w * h * sizeof(uint8_t));
    for (int i = 0; i < h; i++){
        for (int j = 0; j < w; j++){
            data[j + i * w] = im->at[i][j];
        }
    }
    if (!stbi_write_png(filename, w, h, 1, data, stride_length)){
        fprintf(stderr, "Erreur d'écriture.\n");
        image_delete(im);
        free(data);
        exit(EXIT_FAILURE);
    }
    free(data);
}


image *image_new(int h, int w);

void image_delete(image *im);

void invert(image *im);

void binarize(image *im);

void flip_horizontal(image *im);




energy *energy_new(int h, int w);

void energy_delete(energy *e);

void compute_energy(image *im, energy *e);

image *energy_to_image(energy *e);



void remove_pixel(uint8_t *line, double *e, int w);

void reduce_one_pixel(image *im, energy *e);

void reduce_pixels(image *im, int n);



int best_column(energy *e);

void reduce_one_column(image *im, energy *e);

void reduce_column(image *im, int n);



void energy_min_path(energy *e);

path *path_new(int n);

void path_delete(path *p);

void compute_min_path(energy *e, path *p);

void reduce_seam_carving(image *im, int n);



int main(int argc, char *argv[]) {
    if (argc < 3){
        printf("Fournir le fichier d'entrée et de sortie.\n");
        exit(EXIT_FAILURE);
    }
    char *f_in = argv[1];
    char *f_out = argv[2];
    image *im = image_load(f_in);

    // Do some processing here

    image_save(im, f_out);

    image_delete(im);
    return 0;
}
