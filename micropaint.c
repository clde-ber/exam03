#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "unistd.h"

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
    float w;
    float h;
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

int record_infos(t_shape **shape, FILE *file)
{
    int ret;

    ret = fscanf(file, "%d %d %c\n", &((*shape)->data->width), \
    &((*shape)->data->height), &((*shape)->data->background));
    if (ret != 3)
	{
		write(1, "a\n", 2);
        return (1);
	}
	ret = fscanf(file, "%c %f %f %f %f %c\n", &((*shape)->t), \
    &((*shape)->x), &((*shape)->y), &((*shape)->w), &((*shape)->h), &((*shape)->c));
    printf("%d\n", ret);
	if (ret != 6)
	{
		write(1, "b\n", 2);
        return (1);
	}
	if ((*shape)->data->width <= 0 || (*shape)->data->width > 300 || \
    (*shape)->data->height <= 0 || (*shape)->data->height > 300 || \
    ((*shape)->t != 'r' && (*shape)->t != 'R'))
	{
		write(1, "c\n", 2);
		return (1);
	}
	if ((*shape)->w <= 0 || (*shape)->h <= 0)
        return (1);
    return (0);
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
j = 2
10
*/

int is_in_rectangle(t_shape *shape, float x, float y)
{
	printf("%f %f %f %f\n", shape->x, shape->y, x, y);
    if ((int)x >= (int)shape->x && (int)x <= (int)(shape->x + shape->w) && \
    (int)y >= (int)shape->y && (int)y <= (int)(shape->y + shape->h))
	{
		if ((shape->x - x < 1 && shape->x - x >= 0) || (shape->x + shape->w - x < 1 && shape->x + shape->w - x >= 0) || 
			(shape->y - y < 1 && shape->y - y >= 0) || (shape->y + shape->h - y < 1 && shape->y + shape->h - y >= 0))
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
            if ((is_in_rectangle(shape, i, j) >= 0 && shape->t == 'R') ||
				(is_in_rectangle(shape, i, j) == 1 && shape->t == 'r'))
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
    if (record_infos(&shape, ret))
        write(1, "Error: Operation file corrupted\n", 32);
    drawing = fill_plane(shape);
    fill_shape(&drawing, shape);
    draw_drawing(drawing, shape);
    return (0);  
}
