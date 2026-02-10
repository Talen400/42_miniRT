#include "miniRt.h"

bool    hit_sphere(t_ray ray, t_sphere sphere, double t_min,
            double t_max, t_hit_record *rec)
{
    t_vec3  oc;
    double  a;
    double  b;
    double  c;
    double  discriminant;
    double  t;
    double  radius;

    radius = sphere.diameter / 2.0;
    oc = vec3_subtract(ray.origin, sphere.center);
    a = vec3_dot(ray.direction, ray.direction);
    b = 2.0 * vec3_dot(oc, ray.direction);
    c = vec3_dot(oc, oc) - radius * radius;
    discriminant = b * b - 4 * a * c;
    if (discriminant < 0)
        return (false);
    t = (-b - sqrt(discriminant)) / (2.0 * a);
    if (t < t_min || t > t_max)
    {
        t = (-b + sqrt(discriminant)) / (2.0 * a);
        if (t < t_min || t > t_max)
            return (false);
    }
    rec->t = t;
    rec->point = ray_at(ray, t);
    rec->normal = vec3_normalize(vec3_subtract(rec->point, sphere.center));
    rec->color = sphere.color;
    return (true);
}
