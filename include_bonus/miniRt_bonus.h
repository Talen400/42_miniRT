/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: student <student@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 00:00:00 by student           #+#    #+#             */
/*   Updated: 2026/03/09 18:01:02 by tlavared         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

/* ========================================================================== */
/*                               BIBLIOTECAS                                  */
/* ========================================================================== */

# include <math.h>
# include <stdlib.h>
# include <fcntl.h>
# include <unistd.h>
# include <stdio.h>
# include <stdbool.h>
# include <pthread.h>

# include "vectors_bonus.h"

# include "../MLX42/include/MLX42/MLX42.h"
# include "color_bonus.h"
# include "../libft/include/libft.h"	
/* ========================================================================== */
/*                              CONSTANTES                                    */
/* ========================================================================== */

# define CY_NARGS  6
# define PL_NARGS 4
# define SP_NARGS 4
# define CN_NARGS 6
# define B_NARGS 9
# define B_PHONG_NARGS 4
# define B_REFL_NARGS 5
# define B_CHECKER_NARGS 7
# define B_BUMP_NARGS 9
# define KS_DEFAULT 0.2
# define KD_DEFAULT 0.7
# define KA_DEFAULT 0.2
# define SHININESS_DEFAULT 30
# define REFLECTIVITY_DEFAULT 0
# define REFLECTION_DEPTH 5
# define CHECKER_SCALE_DEFAULT 1.0
# define CHECKER_COLOR2_DEFAULT 0x000000FF
# define BUMP_SCALE_DEFAULT 1.0
# define SAMPLES_PER_PIXEL 16
# define WIDTH 1280
# define HEIGHT 800
# define EPSILON 1e-6
# define CAP_EPSILON 1e-4
# define PI 3.14159265358979323846
# define T_MIN 0.001
# define T_MAX 1e30

/*
 * Variável para quantidade threads;
 * E tamanho dos blocos de pixel;
 */

# define TILES_SIZE 8
# define NUM_THREADS 8

/* ========================================================================== */
/*                       ESTRUTURAS FUNDAMENTAIS                              */
/* ========================================================================== */

typedef t_vec3	t_point3;

typedef struct s_ray
{
	t_point3	origin;
	t_vec3		direction;
}	t_ray;

typedef struct s_camera
{
	t_point3	position;
	t_vec3		orientation;
	double		fov;
	t_vec3		forward;
	t_vec3		right;
	t_vec3		up;
	double		viewport_width;
	double		viewport_height;
	t_point3	lower_left_corner;
	t_vec3		horizontal;
	t_vec3		vertical;
}	t_camera;

typedef struct s_ambient
{
	double		intensity;
	t_color		color;
}	t_ambient;

typedef struct s_light
{
	t_point3		position;
	double			brightness;
	t_color			color;
	struct s_light	*next;
}	t_light;

typedef struct s_sphere
{
	t_point3	center;
	double		diameter;
	double		radius;
	t_color		color;
}	t_sphere;

typedef struct s_plane
{
	t_point3	point;
	t_vec3		normal;
	t_color		color;
}	t_plane;

typedef struct s_cylinder
{
	t_point3	center;
	t_vec3		axis;
	double		diameter;
	double		radius;
	double		height;
	t_color		color;
}	t_cylinder;

typedef struct s_cone
{
	t_point3	apex;
	t_vec3		axis;
	double		diameter;
	double		radius;
	double		height;
	double		half_angle;
	t_color		color;
}	t_cone;

typedef enum e_cylinder_surface
{
	CYL_BODY,
	CYL_BOTTOM_CAP,
	CYL_TOP_CAP
}	t_cylinder_surface;

typedef struct s_bump_map
{
	unsigned char	*data;
	unsigned int	width;
	unsigned int	height;
}	t_bump_map;

typedef enum e_object_type
{
	SPHERE,
	PLANE,
	CYLINDER,
	CONE
}	t_object_type;

typedef struct s_object
{
	t_object_type	type;
	union u_shape
	{
		t_sphere	sphere;
		t_plane		plane;
		t_cylinder	cylinder;
		t_cone		cone;
	}				shape;
	double			ks;
	double			kd;
	double			ka;
	double			shininess;
	double			reflectivity;
	bool			has_checker;
	t_color			color2;
	double			checker_scale;
	double			bump_scale;
	t_bump_map		*bump_map;
	struct s_object	*next;
}	t_object;

typedef struct s_hit_record
{
	t_point3			point;
	t_vec3				normal;
	double				t;
	t_color				color;
	bool				front_face;
	t_object			*object;
	t_cylinder_surface	cylinder_surface;
}	t_hit_record;

typedef struct s_quadratic_equation
{
	double	a;
	double	b;
	double	c;
	double	discriminant;
	double	t;
}	t_quadratic_equation;

typedef struct s_scene
{
	t_camera	camera;
	t_ambient	ambient;
	t_light		*lights;
	t_object	*objects;
	t_object	*obj_tail;
	int			width;
	int			height;
}	t_scene;

typedef struct s_tile
{
	int	y_start;
	int	y_end;
	int	x_start;
	int	x_end;
}	t_tile;

typedef struct s_tiles_queue
{
	int				idx;
	int				count;
	t_tile			*tiles;
	pthread_mutex_t	mutex;
}	t_tiles_queue;

typedef struct s_mlx_data
{
	mlx_t			*mlx_ptr;
	void			*win_ptr;
	mlx_image_t		*img_ptr;
	uint8_t			*pixels;
	int				bits_per_pixel;
	int				line_length;
	int				endian;
	t_tiles_queue	tiles_queue;
	pthread_mutex_t	render_mutex;
}	t_mlx_data;

typedef struct s_minirt
{
	t_scene		scene;
	t_mlx_data	mlx;
}	t_minirt;

typedef struct s_thread_data
{
	int				id;
	t_minirt		*minirt;
}	t_thread_data;

long long	get_time_us(void);
t_color		get_heat_color(long long duration, long long max_expected_time);
void		*render_thread(void *arg);
void		init_tiles(t_minirt	*minirt);
void		destroy_tiles(t_tiles_queue *queue);
t_ray		ray_create(t_point3 origin, t_vec3 direction);
void		render_scene(t_minirt *rt);
t_color		calculate_lighting(t_scene *scene, t_hit_record *rec,
				t_ray *r, int depth);
void		ft_free_split(char **split);
void		ft_print_error(const char *msg);
void		error_exit(const char *message);
void		ft_on_close(void *param);
void		ft_on_keypress(mlx_key_data_t keydata, void *param);
void		ft_on_resize(int32_t width, int32_t height, void *param);
t_color		sky_color(t_ray *r);
#endif

