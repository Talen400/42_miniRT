/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: student <student@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 00:00:00 by student           #+#    #+#             */
/*   Updated: 2026/02/02 17:04:17 by tlavared         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

/* ========================================================================== */
/*                               BIBLIOTECAS                                  */
/* ========================================================================== */

# include <math.h>       // Funções matemáticas: sqrt, pow, sin, cos, etc
# include <stdlib.h>     // malloc, free, exit
# include <fcntl.h>      // open, close
# include <unistd.h>     // read, write
# include <stdio.h>      // printf (apenas para debugging)
# include <stdbool.h>    // tipo bool (true/false)

#include "vectors.h"   // Operações com vetores 3D

#include "../MLX42/include/MLX42/MLX42.h"	// Lib da mlx
#include "color.h"
#include "../libft/include/libft.h"			// lib da libft
/* ========================================================================== */
/*                              CONSTANTES                                    */
/* ========================================================================== */

# define CY_NARGS  6
# define PL_NARGS 4
# define SP_NARGS 4
# define CN_NARGS 6
# define B_NARGS 5
# define KS_DEFAULT 0.2
# define KD_DEFAULT 0.7
# define KA_DEFAULT 0.2
# define SHININESS_DEFAULT 30
# define REFLECTIVITY_DEFAULT 0
# define REFLECTION_DEPTH 5
/*
** Definições de tamanho da janela e precisão numérica
** Estas constantes controlam aspectos fundamentais da renderização
*/

# define WIDTH 1280          // Largura padrão da janela em pixels
# define HEIGHT 800         // Altura padrão da janela em pixels
# define EPSILON 1e-6        // Valor muito pequeno para comparações de float
                             // Usado para evitar problemas de precisão numérica

/*
** Constantes matemáticas importantes para ray tracing
** PI é usado em cálculos de ângulos e conversões
*/

# define PI 3.14159265358979323846

/*
** Limites para intersecções de raios
** T_MIN evita "shadow acne" (quando um objeto faz sombra em si mesmo)
** T_MAX define a distância máxima que um raio pode viajar
*/

# define T_MIN 0.001         // Distância mínima para considerar uma interseção
# define T_MAX 1e30          // Distância máxima (essencialmente infinito)

/* ========================================================================== */
/*                       ESTRUTURAS FUNDAMENTAIS                              */
/* ========================================================================== */


/*
** Aliases semânticos para tornar o código mais legível
** Mesmo sendo o mesmo tipo de estrutura, usar nomes diferentes
** ajuda a entender a intenção do programador:
*/

typedef t_vec3	t_point3;    // Quando representa uma posição no espaço

typedef struct s_ray
{
	t_point3	origin;       // Ponto de onde o raio parte
	t_vec3		direction;    // Direção para onde o raio vai (normalizado)
}	t_ray;


typedef struct s_camera
{
	t_point3	position;           // Onde a câmera está no espaço 3D
	t_vec3		orientation;        // Direção normalizada para onde olha
	double		fov;                // Campo de visão em graus [0, 180]
	
	/* Vetores do sistema de coordenadas da câmera (calculados) */
	t_vec3		forward;            // = orientation (normalizado)
	t_vec3		right;              // Perpendicular a forward e up
	t_vec3		up;                 // Perpendicular a forward e right
	
	/* Dimensões e posição do viewport (plano de projeção) */
	double		viewport_width;     // Largura do viewport em unidades do mundo
	double		viewport_height;    // Altura do viewport em unidades do mundo
	t_point3	lower_left_corner;  // Canto inferior esquerdo do viewport
	t_vec3		horizontal;         // Vetor horizontal do viewport
	t_vec3		vertical;           // Vetor vertical do viewport
}	t_camera;

typedef struct s_ambient
{
	double		intensity;    // Intensidade da luz [0.0, 1.0]
	t_color		color;        // Cor da luz ambiente (RGB)
}	t_ambient;

typedef struct s_light
{
	t_point3		position;     // Posição da luz no espaço 3D
	double			brightness;   // Intensidade/brilho [0.0, 1.0]
	t_color			color;        // Cor da luz (RGB)
	struct s_light	*next;        // Próxima luz na lista (pode haver várias)
}	t_light;

typedef struct s_sphere
{
	t_point3	center;       // Centro da esfera no espaço 3D
	double		diameter;     // Diâmetro (como especificado no .rt)
	double		radius;       // Raio calculado (diameter / 2.0)
	t_color		color;        // Cor da superfície (RGB)
}	t_sphere;


typedef struct s_plane
{
	t_point3	point;        // Qualquer ponto no plano
	t_vec3		normal;       // Vetor perpendicular ao plano (normalizado)
	t_color		color;        // Cor da superfície (RGB)
}	t_plane;

typedef struct s_cylinder
{
	t_point3	center;       // Centro da base inferior do cilindro
	t_vec3		axis;         // Vetor do eixo central (normalizado)
	double		diameter;     // Diâmetro (como especificado no .rt)
	double		radius;       // Raio calculado (diameter / 2.0)
	double		height;       // Altura do cilindro
	t_color		color;        // Cor da superfície (RGB)
}	t_cylinder;

typedef struct s_cone
{
	t_point3	apex;       // Ponta do cone (vértice)
	t_vec3		axis;       // Direção normalizada (da ponta para a base)
	double		diameter;   // Diâmetro da base
	double		radius;     // Raio (diameter / 2) — pré-calculado
	double		height;     // Altura do cone
	double		half_angle; // tan(ângulo de abertura/2) — pré-calculado
	t_color		color;      // Cor RGB
}	t_cone;
typedef enum e_cylinder_surface
{
	CYL_BODY,
	CYL_BOTTOM_CAP,
	CYL_TOP_CAP
}	t_cylinder_surface;


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
} t_quadratic_equation;

typedef struct s_scene
{
	t_camera	camera;       // A câmera que observa a cena (apenas uma)
	t_ambient	ambient;      // Luz ambiente da cena (apenas uma)
	t_light		*lights;      // Lista ligada de luzes pontuais (uma ou mais)
	t_object	*objects;     // Lista ligada de objetos (zero ou mais)
	int			width;        // Largura da imagem em pixels
	int			height;       // Altura da imagem em pixels
}	t_scene;

typedef struct s_mlx_data
{
	mlx_t		*mlx_ptr;         // Ponteiro para instância da MLX
	void		*win_ptr;         // Ponteiro para a janela
	mlx_image_t	*img_ptr;         // Ponteiro para a imagem
	uint8_t		*pixels;        // Array de pixels (dados brutos da imagem)
	int			bits_per_pixel;   // Bits usados por cada pixel
	int			line_length;      // Bytes por linha (com padding possível)
	int			endian;           // Ordem dos bytes (0=little, 1=big endian)
}	t_mlx_data;
typedef struct s_minirt
{
	t_scene		scene;
	t_mlx_data	mlx;
}	t_minirt;

t_ray		ray_create(t_point3 origin, t_vec3 direction);
void		render_scene(t_minirt *rt);
t_color		calculate_lighting(t_scene *scene, t_hit_record *rec, t_ray *r, int depth);
void		ft_free_split(char **split);
void		error_exit(const char *message);
void		ft_on_close(void *param);
void		ft_on_keypress(mlx_key_data_t keydata, void *param);
void		ft_on_resize(int32_t width, int32_t height, void *param);
t_color	sky_color(t_ray *r);
#endif
