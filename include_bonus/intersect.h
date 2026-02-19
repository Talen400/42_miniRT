/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersect.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgregori <rgregori@student.42sp.org.br>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-01-29 15:07:34 by rgregori          #+#    #+#             */
/*   Updated: 2026-01-29 15:07:34 by rgregori         ###   ########.fr       */
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

double	hit_cap(t_ray *r, t_vec3 cap_center, t_vec3 axis, double radius);
double	hit_sphere(t_ray *r, t_object *obj);
double	hit_plane(t_ray *r, t_object *obj);
double	hit_cylinder(t_ray *r, t_object *obj);
double	hit_cone(t_ray *r, t_object *obj);
bool	hit_scene(t_ray *r, t_object *objects, t_hit_record *rec, double t_min);
void	fill_hit_record(t_hit_record *rec, t_ray *r, t_object *obj, double t);

#endif