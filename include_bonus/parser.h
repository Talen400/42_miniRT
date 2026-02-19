/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: student <student@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 00:00:00 by student           #+#    #+#             */
/*   Updated: 2026/02/02 00:00:00 by student          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "miniRt.h"
# include "scene.h"

typedef struct s_parse_context	t_parse_context;

typedef bool (*t_parse_function)(t_parse_context *ctx, t_scene *scene, const char *line);

typedef struct s_light_data
{
	t_point3	position;
	double		brightness;
	t_color		color;
}	t_light_data;

// Para câmera
typedef struct s_camera_data
{
	t_point3	position;
	t_vec3		orientation;
	double		fov;
}	t_camera_data;

// Para esfera
typedef struct s_sphere_data
{
	t_point3	center;
	double		diameter;
	t_color		color;
}	t_sphere_data;

// Para plano
typedef struct s_plane_data
{
	t_point3	point;
	t_vec3		normal;
	t_color		color;
}	t_plane_data;

// Para cilindro
typedef struct s_cylinder_data
{
	t_point3	center;
	t_vec3		axis;
	double		diameter;
	double		height;
	t_color		color;
}	t_cylinder_data;

typedef struct s_cone_data
{
	t_point3	apex;
	t_vec3		axis;
	double		diameter;
	double		height;
	double		half_angle;
	t_color		color;
}t_cone_data;

typedef struct s_element_parser
{
	const char			*identifier;
	t_parse_function	parser;
}	t_element_parser;

struct s_parse_context
{
	int			camera_count;
	int			ambient_count;
	int			light_count;
	int			current_line;
	const char	*filename;
	bool		error_occurred;
};

bool		validate_and_normalize_axis(t_vec3 *axis, t_parse_context *ctx,
										char **tokens);
bool		parse_scene(const char *filename, t_scene *scene);
bool		parse_ambient(t_parse_context *ctx, t_scene *scene, const char *line);
bool		parse_camera(t_parse_context *ctx, t_scene *scene, const char *line);
bool		parse_light(t_parse_context *ctx, t_scene *scene, const char *line);
bool		parse_sphere(t_parse_context *ctx, t_scene *scene, const char *line);
bool		parse_plane(t_parse_context *ctx, t_scene *scene, const char *line);
bool		parse_cylinder(t_parse_context *ctx, t_scene *scene, const char *line);
bool		parse_cone(t_parse_context *ctx, t_scene *scene, const char *line);
bool		parse_vector(const char *str, t_vec3 *vec);
bool		parse_color(const char *str, t_color *color);
void		calculate_viewport(t_camera *camera, int width, int height);
bool		is_valid_identifier(const char *str);
const char	*skip_whitespace(const char *str);
bool		is_empty_line(const char *line);
bool		is_comment_line(const char *line);
bool		validate_normalized(t_vec3 vec);
bool		validate_color_component(double value);
bool		validate_ratio(double value);
bool		validate_fov(double fov);
bool		ft_isempty_or_comment(const char *line);
char		*extract_identifier(const char *line);
bool		process_line(t_parse_context *context, t_scene *scene,
				const char *line);
void		print_parse_error(const char *filename, int line_num, 
				const char *message);
void		ft_error_and_free(t_parse_context *ctx, char **tokens,
				const char *message);
/*
** ===========================================================================
**                          TABELA DE LOOKUP
** ===========================================================================
**
** A tabela de lookup é declarada como extern aqui porque ela é definida
** em parser.c. Outros arquivos do módulo de parsing podem referenciá-la
** através deste header.
**
** A tabela é um array de t_element_parser terminado por uma entrada com
** identifier NULL (sentinela). Isso permite iterar pela tabela sem saber
** o tamanho em tempo de compilação.
**
** ESTRUTURA DA TABELA (em parser.c):
**
**     const t_element_parser g_element_parsers[] = {
**         {"A", parse_ambient},
**         {"C", parse_camera},
**         {"L", parse_light},
**         {"sp", parse_sphere},
**         {"pl", parse_plane},
**         {"cy", parse_cylinder},
**         {"cn", parse_cone},
**         {NULL, NULL}  // Sentinela marca o fim
**     };
**
** COMO USAR:
** Itere pela tabela até encontrar identifier NULL. Para cada entrada,
** compare o identifier com a string que você extraiu da linha. Se bater,
** chame a parse_func associada.
**
** EXEMPLO DE BUSCA:
**     int i = 0;
**     while (g_element_parsers[i].identifier != NULL)
**     {
**         if (strcmp(g_element_parsers[i].identifier, id) == 0)
**         {
**             // Encontrou! Chama a função
**             return g_element_parsers[i].parse_func(line, scene, line_num);
**         }
**         i++;
**     }
**     // Não encontrou - identificador inválido
**
** VANTAGENS DESTA ABORDAGEM:
** - Para adicionar novo tipo: adicione uma linha na tabela
** - Não precisa modificar lógica de busca
** - Fácil de desabilitar temporariamente um tipo (comente a linha)
** - Código de busca é genérico e não muda nunca
*/

extern const t_element_parser	g_element_parsers[];

#endif
