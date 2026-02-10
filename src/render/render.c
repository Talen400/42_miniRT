
#include "miniRt.h"

static uint32_t color_to_uint(t_color color)
{
    uint8_t r;
    uint8_t g;
    uint8_t b;

    r = (uint8_t)fmin(255, fmax(0, color.x * 255.0));
    g = (uint8_t)fmin(255, fmax(0, color.y * 255.0));
    b = (uint8_t)fmin(255, fmax(0, color.z * 255.0));
    return ((r << 24) | (g << 16) | (b << 8) | 0xFF);
}

static void	put(uint8_t *pixels, int x, int y, uint32_t color)
{
	int	index;

	if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT)
	{
		index = (y * WIDTH + x) * 4;
		pixels[index + 0] = (color >> 24) & 0xFF;
		pixels[index + 1] = (color >> 16) & 0xFF;
		pixels[index + 2] = (color >> 8) & 0xFF;
		pixels[index + 3] = color & 0xFF;
    }
}

static t_ray generate_ray(t_camera *cam, double u, double v)
{
    t_ray       ray;
    t_point3    point_on_viewport;

    point_on_viewport = vec3_add(cam->lower_left_corner,
        vec3_add(vec3_multiply(cam->horizontal, u),
                 vec3_multiply(cam->vertical, v)));
    ray.origin = cam->position;
    ray.direction = vec3_normalize(
        vec3_subtract(point_on_viewport, cam->position));
    return (ray);
}

static bool    trace_ray(t_scene *scene, t_ray ray, t_hit_record *rec)
{
    t_object        *obj;
    t_hit_record    temp;
    double          closest;
    bool            hit_anything;

    closest = T_MAX;
    hit_anything = false;
    obj = scene->objects;
    while (obj)
    {
        if (obj->type == SPHERE
            && hit_sphere(ray, obj->shape.sphere, T_MIN, closest, &temp))
        {
            hit_anything = true;
            closest = temp.t;
            *rec = temp;
        }
        else if (obj->type == PLANE
            && hit_plane(ray, obj->shape.plane, T_MIN, closest, &temp))
        {
            hit_anything = true;
            closest = temp.t;
            *rec = temp;
        }
        obj = obj->next;
    }
    return (hit_anything);
}

void    render_scene(t_minirt *rt)
{
    int             x;
    int             y;
    double          u;
    double          v;
    t_ray           ray;
    t_hit_record    rec;
    t_color         color;

    y = 0;
    while (y < HEIGHT)
    {
        x = 0;
        while (x < WIDTH)
        {
            u = (double)x / (double)(WIDTH - 1);
            v = (double)(HEIGHT - 1 - y) / (double)(HEIGHT - 1);
            ray = generate_ray(&rt->scene.camera, u, v);
            if (trace_ray(&rt->scene, ray, &rec))
                color = calculate_lighting(&rt->scene, &rec);
            else
                color = vec3_create(0, 0, 0);
            put(rt->mlx.pixels, x, y, color_to_uint(color));
            x++;
        }
        y++;
    }
}
