//
// Created by Hassan Khurram on 2021-04-03.
//

#include "c_img.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>



void create_img(struct rgb_img **im, size_t height, size_t width){
    /// creates a new image with specified height and width, stored in *im
    *im = (struct rgb_img *)malloc(sizeof(struct rgb_img));
    (*im)->height = height;
    (*im)->width = width;
    (*im)->raster = (uint8_t *)malloc(3 * height * width); // 3 bytes per pixel (r,g,b)
}


int read_2bytes(FILE *fp){
    uint8_t bytes[2];
    fread(bytes, sizeof(uint8_t), 1, fp);
    fread(bytes+1, sizeof(uint8_t), 1, fp);
    return (  ((int)bytes[0]) << 8)  + (int)bytes[1];
}

void write_2bytes(FILE *fp, int num){
    uint8_t bytes[2];
    bytes[0] = (uint8_t)((num & 0XFFFF) >> 8);
    bytes[1] = (uint8_t)(num & 0XFF);
    fwrite(bytes, 1, 1, fp);
    fwrite(bytes+1, 1, 1, fp);
}


//////////////////////////////////////////////////////////

// whats the difference between read_in_img and write_img????

void read_in_img(struct rgb_img **im, char *filename){
    FILE *fp = fopen(filename, "rb"); // reading the .bin file and write to im // modify *im
    size_t height = read_2bytes(fp);
    size_t width = read_2bytes(fp);

    create_img(im, height, width);

    fread((*im)->raster, 1, 3*width*height, fp); // store the contents (each pixel) from fp to img
    // created
    fclose(fp);
}

// because im is modifying

// changes then written back to the file (.bin? file)



void write_img(struct rgb_img *im, char *filename){
    // writes a struct rgb_img (after changes are made to the rgb_img) to a .bin file
    FILE *fp = fopen(filename, "wb");
    write_2bytes(fp, im->height);
    write_2bytes(fp, im->width);
    fwrite(im->raster, 1, im->height * im->width * 3, fp);
    fclose(fp);
}




uint8_t get_pixel(struct rgb_img *im, int y, int x, int col){
    return im->raster[3 * (y*(im->width) + x) + col]; // raster is an array of pixels
    // col = 0 red, 1 green, 2 blue
}



void set_pixel(struct rgb_img *im, int y, int x, int r, int g, int b){
    // modifies the pixel value (r,g,b)
    im->raster[3 * (y*(im->width) + x) + 0] = r;
    im->raster[3 * (y*(im->width) + x) + 1] = g;
    im->raster[3 * (y*(im->width) + x) + 2] = b;
}

void destroy_image(struct rgb_img *im)
{
    free(im->raster);
    free(im);
}


void print_grad(struct rgb_img *grad){
    int height = grad->height;
    int width = grad->width;
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            printf("%d\t", get_pixel(grad, i, j, 0));
        }
        printf("\n");
    }
}

















/////////////////////////////////////////////////////////////////////////////

