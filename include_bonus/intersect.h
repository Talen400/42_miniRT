#ifndef INTERSECT_H
# define INTERSECT_H

# include "miniRt.h"

double	hit_sphere(t_ray *r, t_object *obj);
double	hit_plane(t_ray *r, t_object *obj);
double	hit_cylinder(t_ray *r, t_object *obj);
bool	hit_scene(t_ray *r, t_object *objects, t_hit_record *rec, double t_min);
void	fill_hit_record(t_hit_record *rec, t_ray *r, t_object *obj, double t);

#endif
