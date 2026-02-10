#include "miniRt.h"

t_ray    ray_create(t_point3 origin, t_vec3 direction)
{
    t_ray   ray;

    ray.origin = origin;
    ray.direction = direction;
    return (ray);
}

t_point3    ray_at(t_ray ray, double t)
{
    return (vec3_add(ray.origin, vec3_multiply(ray.direction, t)));
}
