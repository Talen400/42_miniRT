#ifndef SCENE_H
# define SCENE_H

# include "miniRt.h"

bool	    set_specular(t_object *obj, char **tokens, int base_count);
t_object	*create_sphere_obj(t_sphere sphere, char **tokens);
t_object	*create_plane_obj(t_plane plane, char **tokens);
t_object	*create_cylinder_obj(t_cylinder cylinder, char **tokens);
void		add_object_to_scene(t_scene *scene, t_object *new_obj);
t_scene		scene_init(void);
void		destroy_scene(t_scene *scene);
void		free_scene(t_scene *scene);

#endif
