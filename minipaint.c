#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "unistd.h"
#include "math.h"

typedef struct s_plane
{
    int width;
    int height;
    char background;
}              t_plane;


typedef struct s_shape
{
    char t;
    float x;
    float y;
    float r;
    char c;
    t_plane *data;
}               t_shape;

void    init_structs(t_plane **plane, t_shape **shape)
{
    *plane = malloc(sizeof(t_plane));
    memset(*plane, 0, sizeof(t_plane));
    *shape = malloc(sizeof(t_shape));
    memset(*shape, 0, sizeof(t_shape));
    (*shape)->data = *plane;
}

char    *fill_plane(t_shape *shape)
{
    float   i;
    float   j;
    char    *ret;

    i = 0;
    j = 0;
    ret = malloc(sizeof(char) * (shape->data->width * shape->data->height));
    while (i < shape->data->width)
    {
        while (j < shape->data->height)
        {
            ret[(int)(i + j * shape->data->width)] = shape->data->background;
            j++;
        }
        j = 0;
        i++;
    }
    return (ret);
}
/*
000000
000x00  4 + 1 * 6 = 10
i = 4
j = 1
10
*/
// srqt((Xa - Xb) * (Xa - Xb) + (Ya - Yb) * (Ya - Yb))

int is_in_circle(t_shape *shape, float x, float y)
{
    float distance;

    distance = sqrtf(powf(x - shape->y, 2.0) + powf(y - shape->x, 2.0));
    if (distance <= shape->r)
    {
        if (distance - shape->r <= 1.00000000 && distance - shape->r >= -1.00000000)
            return (1);
        return (0);
    }
    return (-1);
}

void    fill_shape(char **drawing, t_shape *shape)
{
    float   i;
    float   j;

    i = 0;
    j = 0;
    while (i < shape->data->width)
    {
        while (j < shape->data->height)
        {
            if ((is_in_circle(shape, i, j) >= 0 && shape->t == 'C') || \
            (is_in_circle(shape, i, j) == 1 && shape->t == 'c'))
                (*drawing)[(int)(i + j * shape->data->width)] = shape->c;
            j++;
        }
        j = 0;
        i++;
    }
}

void    draw_drawing(char *drawing, t_shape *shape)
{
    float i;
    float j;

    i = 0;
    j = 0;
    while (i < shape->data->width)
    {
        while (j < shape->data->height)
        {
            write(1, &drawing[(int)(i + j * shape->data->width)], 1);
            j++;
        }
        write(1, "\n", 1);
        j = 0;
        i++;
    }
}

int record_infos(t_shape **shape, FILE *file, char *drawing)
{
    int ret;

    ret = fscanf(file, "%d %d %c\n", &((*shape)->data->height), \
    &((*shape)->data->width), &((*shape)->data->background));
    if (ret != 3)
        return (1);
    ret = fscanf(file, "%c %f %f %f %c\n", &((*shape)->t), \
    &((*shape)->x), &((*shape)->y), &((*shape)->r), &((*shape)->c));
    if (ret != 5)
        return (1);
    if ((*shape)->data->width <= 0 || (*shape)->data->width > 300 || \
    (*shape)->data->height <= 0 || (*shape)->data->height > 300 || \
    ((*shape)->t != 'c' && (*shape)->t != 'C'))
        return (1);
    if ((*shape)->r <= 0)
        return (1);
    drawing = fill_plane(*shape);
    fill_shape(&drawing, *shape);
    while ((ret = fscanf(file, "%c %f %f %f %c\n", &((*shape)->t), \
    &((*shape)->x), &((*shape)->y), &((*shape)->r), &((*shape)->c))) == 5)
    {
        if ((*shape)->data->width <= 0 || (*shape)->data->width > 300 || \
        (*shape)->data->height <= 0 || (*shape)->data->height > 300 || \
        ((*shape)->t != 'c' && (*shape)->t != 'C'))
            return (1);
        if ((*shape)->r <= 0)
            return (1);
        fill_shape(&drawing, *shape);
    }
    draw_drawing(drawing, *shape);
    return (0);
}

int main(int argc, char **argv)
{
    FILE     *ret;
    char    *drawing;
    t_shape *shape;
    t_plane *plane;

    ret = 0;
    drawing = NULL;
    shape = 0;
    plane = 0;
    if (argc != 2)
        write(1, "Error: argument\n", 16);
    ret = fopen(argv[1], "r");
    if (!ret)
        write(1, "Error: Operation file corrupted\n", 32);
    if (argc != 2 || !ret)
        return (1);
    init_structs(&plane, &shape);
    if (record_infos(&shape, ret, drawing))
        write(1, "Error: Operation file corrupted\n", 32);
    fclose(ret);
    free(drawing);
    free(shape->data);
    free(shape);
    return (0);  
}