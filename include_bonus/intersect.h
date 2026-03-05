/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersect.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgregori <rgregori@student.42sp.org.br>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-01-29 15:07:34 by rgregori          #+#    #+#             */
/*   Updated: 2026/03/04 18:20:39 by tlavared         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INTERSECT_H
# define INTERSECT_H

# include "miniRt.h"

typedef struct s_cone_calc
{
	t_vec3	w;
	t_vec3	v;
	double	d_dot_a;
	double	x_dot_a;
}	t_cone_calc;

typedef struct s_bump_frame
{
	t_vec3	tangent;
	t_vec3	bitangent;
	double	du;
	double	dv;
	double	scale;
}	t_bump_frame;

double	hit_cap(t_ray *r, t_vec3 cap_center, t_vec3 axis, double radius);
double	hit_sphere(t_ray *r, t_object *obj);
double	hit_plane(t_ray *r, t_object *obj);
double	hit_cylinder(t_ray *r, t_object *obj);
double	hit_cone(t_ray *r, t_object *obj);
bool	hit_scene(t_ray *r, t_object *objects, t_hit_record *rec, double t_min);
void	fill_hit_record(t_hit_record *rec, t_ray *r, t_object *obj, double t);
t_color	get_object_color(t_object *obj);
t_color	get_surface_color(t_vec3 point, t_object *obj);
void	perturb_normal(t_vec3 point, t_vec3 *normal, t_object *obj);
t_color	calculate_specular(t_scene *scene, t_hit_record *rec,
		t_vec3 light_dir, t_light *light);
t_color	calculate_reflection(t_scene *scene, t_hit_record *rec,
		t_ray *ray, int depth);
double	sample_map(t_bump_map *bm, double u, double v);
void	calc_grad_proc(t_bump_frame *f, double u, double v);
void	calc_grad_image(t_bump_frame *f, t_bump_map *bm, double u, double v);
void	apply_bump(t_vec3 *normal, t_bump_frame *f);
void	bump_sphere(t_vec3 point, t_object *obj, t_vec3 *normal);
void	bump_plane(t_vec3 point, t_object *obj, t_vec3 *normal);
void	bump_cylinder(t_vec3 point, t_object *obj, t_vec3 *normal);
void	bump_cone(t_vec3 point, t_object *obj, t_vec3 *normal);
t_color	pick_checker(double u, double v, t_object *obj);


#endif
