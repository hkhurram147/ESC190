#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "seamcarving.h"
#include "c_img.h"



int edge(int ind, int max_len)
{
    if (ind == max_len)
        return 0;
    else if (ind == -1)
        return max_len-1;

    else return ind;
}


void calc_energy(struct rgb_img *im, struct rgb_img **grad) {

    // PART 1: Dual-Gradient Energy Function

    int height = im->height;
    int width = im->width;
    create_img(grad, height, width);

    for (int y = 0; y < height; y++){
        for (int x = 0; x < width; x++) {

            double R_x, G_x, B_x, R_y, G_y, B_y;

            R_x = (double)get_pixel(im, y, edge(x + 1, width), 0) - (double)get_pixel(im, y, edge(x - 1, width), 0);
            G_x = (double)get_pixel(im, y, edge(x + 1, width), 1) - (double)get_pixel(im, y, edge(x - 1, width), 1);
            B_x = (double)get_pixel(im, y, edge(x + 1, width), 2) - (double)get_pixel(im, y, edge(x - 1, width), 2);

            R_y = (double)get_pixel(im, edge(y + 1, height), x, 0) - (double)get_pixel(im,edge(y-1,height), x,0);
            G_y = (double)get_pixel(im, edge(y + 1, height), x, 1) - (double)get_pixel(im,edge(y-1, height), x,1);
            B_y = (double)get_pixel(im, edge(y + 1, height), x, 2) - (double)get_pixel(im,edge(y-1,height), x, 2);

            float d_x2 = (float) ((R_x * R_x) + (G_x * G_x) + (B_x * B_x));
            float d_y2 = (float) ((R_y * R_y) + (G_y * G_y) + (B_y * B_y));

            float energy = sqrt(d_x2 + d_y2);

            uint8_t dual_grad_energy = (uint8_t) (energy / 10);
            //printf("%d\n", (int)dual_grad_energy);

            set_pixel(*grad, y, x, dual_grad_energy, dual_grad_energy, dual_grad_energy);
            //printf("%d , %d \n", x, y);
        }
    }
}




void dynamic_seam(struct rgb_img *grad, double **best_arr) {
    // allocates and computes the dynamic array *best_arr
    // best array cumulates (memoizes) the energy sums from the top to the bottom row
    // compare each elem below the first row with 3 elems stored in best array right above it

    int height = grad->height;
    int width = grad->width;
    *best_arr = (double *)malloc(sizeof(double) * (width * height)); // NOT a 2D array

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (i == 0)
            {
                // copy the top row with values as is
                (*best_arr)[j] = grad->raster[3 * (i*(width) + j) + 0];
            } // bottom up approach 'base case' (not summing with anything else)

            else
            {
                double min_energy_above = (double)(*best_arr)[(i-1)*width+j]; // elem directly
                // above cur node
                for (int k = j-1; k < j+2; k++)
                {
                    if (k >= 0 && k < width)
                    {
                        if ( (*best_arr)[(i-1)*width+k] < min_energy_above)
                        {
                            min_energy_above = (*best_arr)[(i-1)*width+k];
                        }
                    }
                }

                (*best_arr)[(i)*width+j] = grad->raster[3 * (i*(width) + j) + 0] + min_energy_above;
            }
        }
    }
}




/*
double **make_2d(double *best, int height, int width)
{
    double **arr = (double **)malloc(height * sizeof(double *));
    for (int i = 0; i < height; i ++)
    {
        arr[i] = (double*)malloc(width * sizeof(double));
        for (int j = 0; j < width; j++)
        {
            arr[i][j] = best[i*width+j];
        }
    }
    return arr;
}
*/


/*
void recover_path(double *best, int height, int width, int **path)
{
    *path = (int *)malloc(sizeof(int) * height);
    int min_first_row = best[0];
    int min_idx = 0;
    for (int j = 1; j < width; j++)
    {
        if (best[j] < min_first_row)
        {
            min_first_row = best[j];
            min_idx = j;
        }
    }
    (*path)[0] = min_idx;
    for (int i = 1; i < height; i ++)
    {
        int orig = min_idx;
        int min = best[i*width+orig];
        if (orig-1 >= 0)
        {
            if (best[i*width+(orig-1)] < min)
            {
                min_idx = orig-1;
                min = best[i*width+(orig-1)];
            }
        }
        if (orig+1 <= width - 1)
        {
            if (best[i*width+(orig+1)] < min)
            {
                min_idx = orig+1;
                min = best[i*width+(orig+1)];
            }
        }
        (*path)[i] = min_idx;
    }
}
*/
void recover_path(double *best, int height, int width, int **path)
    {
        *path = (int *)malloc(sizeof(int) * height);
        int min_bottom_row = best[(height-1)*width];
        int min_idx = 0;
        for (int j = 1; j <width; j ++)
        {
            if (best[(height-1)*width + j] < min_bottom_row)
            {
                min_bottom_row = best[(height-1)*width + j];
                min_idx = j;
            }
        }
        (*path)[height-1] = min_idx;

    for (int i = height-2; i >= 0; i --)
    {
        int orig = min_idx;
        int min = best[i*width+orig];
        if (orig-1 >= 0)
        {
            if (best[i*width+(orig-1)] < min)
            {
                min_idx = orig-1;
                min = best[i*width+(orig-1)];
            }
        }
        if (orig+1 <= width-1)
        {
            if (best[i*width+(orig+1)] < min)
            {
                min_idx = orig+1;
                min = best[i*width+(orig+1)];
            }
        }
        (*path)[i] = min_idx;
    }
}
/*
void helper(double *best, int height, int width, int **path, int start)
{
    *path = (int *)malloc(sizeof(int) * height);
    (*path)[0] = start;
    for (int i = 1; i < height; i ++)
    {
        int orig = start;
        int min = best[i*width+orig];
        if (orig-1 >= 0)
        {
            if (best[i*width+(orig-1)] < min)
            {
                start = orig-1;
                min = best[i*width+(orig-1)];
            }
        }
        if (orig+1 <= width - 1)
        {
            if (best[i*width+(orig+1)] < min)
            {
                start = orig+1;
                min = best[i*width+(orig+1)];
            }
        }
        (*path)[i] = start;
    }
}
void recover_path(double *best, int height, int width, int **path)
{
    int *path_first;
    helper(best, height, width, &path_first, 0);
    int min_sum = 0;
    for (int i = 0; i < height; i++)
    {
        min_sum += best[i*width + path_first[i]];
    }
    int *best_path = path_first;
    for (int j = 1; j < width; j ++)
    {
        int *path_temp;
        helper(best, height, width, &path_temp, j);
        int new_sum = 0;
        for (int i = 0; i < height; i++)
        {
            new_sum += best[i*width + path_temp[i]];
        }
        if (new_sum < min_sum)
        {
            min_sum = new_sum;
            best_path = path_temp;
        }
    }
    *path = (int *)malloc(sizeof(int) * height);
    *path = best_path;
}
*/






/*
void remove_seam(struct rgb_img *src, struct rgb_img **dest, int *path)
{
    int height = src->height;
    int width = src->width - 1;
    create_img(dest, height, width);
    for (int y = 0; y < height; y ++)
    {
        for (int x = 0; x < width; x++)
        {
            if (x != path[y])
            {
                int x_new = x;
                if (x > path[y])
                {
                    x_new = x-1;
                }
                int r = (int)get_pixel(src, y, x, 0);
                int g = (int)get_pixel(src, y, x, 1);
                int b = (int)get_pixel(src, y, x, 2);
                set_pixel(*dest, y, x_new, r, g, b);
            }
        }
    }
}
*/


void remove_seam(struct rgb_img *src, struct rgb_img **dest, int *path) {
    // function creates the dest img and writes it to src with seam removed
    // write the source img to the dest img
    int height = src->height;
    int width = src->width;

    create_img(dest, height, width-1); // 1 less width


    for (int y = 0; y < height; y++) {
        int offset = 0;

        for (int x = 0; x < width; x++) {

            //printf("%d, %d \n ", x,y);
            if (x == path[y])  // dont copy over the element in path[cur_row]
            {
                offset++;
                continue;
            }
            else {
                float r_Val = get_pixel(src, y, x, 0); // r
                float g_Val = get_pixel(src, y, x, 1); // g
                float b_Val = get_pixel(src, y, x, 2); // b

                set_pixel(*dest, y, x - offset, (uint8_t)r_Val, (uint8_t)g_Val, (uint8_t)b_Val);
            }
        }
    }
}






///////



/*
int main(void)
{
    struct rgb_img *im;
    struct rgb_img *cur_im;
    struct rgb_img *grad;
    double *best;
    int *path;

    read_in_img(&im, "HJoceanSmall.bin");

    for(int i = 0; i < ; i++){
        printf("i = %d\n", i);
        calc_energy(im,  &grad);
        dynamic_seam(grad, &best);
        recover_path(best, grad->height, grad->width, &path);
        remove_seam(im, &cur_im, path);

        char filename[200];
        sprintf(filename, "img%d.bin", i);
        write_img(cur_im, filename);


        destroy_image(im);
        destroy_image(grad);
        free(best);
        free(path);
        im = cur_im;
    }
    destroy_image(im);
}
*/



/*
int main(void) {

    struct rgb_img *grad;
    struct rgb_img *im;
    struct rgb_img *cur_im;

    read_in_img(&im, "3x4.bin");  // og image bin file

    calc_energy(im, &grad);

//print_grad(grad);


    double *best_arr;
    dynamic_seam(grad, &best_arr);

    for (int i = 0; i < grad->height; i++)
    {
        for (int j = 0; j < grad->width; j++)
        {
            printf("%f ", best_arr[i*(grad->width) + j]);
        }
        printf("\n");
    }

    double best_arr2[] = {0.3, 1234, 1,   0,1234,2,   5560,0,5,   68770,132445,258};

    int *path;
    recover_path(best_arr, grad->height, grad->width, &path);

    for (int c = 0; c < grad->height; c++)
    {
        printf("%d ", path[c]);
    }

    remove_seam(im, &cur_im, path);
}
*/