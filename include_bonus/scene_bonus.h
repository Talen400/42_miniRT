/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgregori <rgregori@student.42sp.org.br>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-03-04 16:31:21 by rgregori          #+#    #+#             */
/*   Updated: 2026-03-04 16:31:21 by rgregori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCENE_H
# define SCENE_H

# include "miniRt_bonus.h"

void		init_object_defaults(t_object *obj);
bool		set_bump(t_object *obj, char **tokens, int base_count, int extra);
bool		set_extra_args(t_object *obj, char **tokens, int base_count);
t_object	*create_sphere_obj(t_sphere sphere, char **tokens);
t_object	*create_plane_obj(t_plane plane, char **tokens);
t_object	*create_cylinder_obj(t_cylinder cylinder, char **tokens);
t_object	*create_cone_obj(t_cone cone, char **tokens);
void		add_object_to_scene(t_scene *scene, t_object *new_obj);
t_scene		scene_init(void);
void		destroy_scene(t_scene *scene);
void		free_scene(t_scene *scene);

#endif
