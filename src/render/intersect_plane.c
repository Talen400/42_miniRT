#include "miniRt.h"

bool    hit_plane(t_ray ray, t_plane plane, double t_min,
            double t_max, t_hit_record *rec)
{
    double  denom;
    double  t;
    t_vec3  diff;

    denom = vec3_dot(plane.normal, ray.direction);
    if (fabs(denom) < EPSILON)
        return (false);
    diff = vec3_subtract(plane.point, ray.origin);
    t = vec3_dot(diff, plane.normal) / denom;
    if (t < t_min || t > t_max)
        return (false);
    rec->t = t;
    rec->point = ray_at(ray, t);
    rec->normal = plane.normal;
    if (denom > 0)
        rec->normal = vec3_multiply(rec->normal, -1);
    rec->color = plane.color;
    return (true);
}
