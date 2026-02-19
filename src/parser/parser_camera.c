/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_camera.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgregori <rgregori@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-02-06 13:00:00 by rgregori          #+#    #+#             */
/*   Updated: 2026-02-06 13:00:00 by rgregori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRt.h"
#include "parser.h"
#include <math.h>

static bool	helper_validate_camera(char **tokens, t_parse_context *ctx,
							t_camera_data *data)
{
	if (ft_array_size((void **)tokens) != 4)
	{
		ft_error_and_free(ctx, tokens,
			"Camera: expected format 'C <x,y,z> <nx,ny,nz> <fov>'");
		return (false);
	}
	if (!parse_vector(tokens[1], &data->position))
	{
		ft_error_and_free(ctx, tokens, "Camera: invalid position format");
		return (false);
	}
	if (!parse_vector(tokens[2], &data->orientation))
	{
		ft_error_and_free(ctx, tokens, "Camera: invalid orientation format");
		return (false);
	}
	return (true);
}

static bool	validate_camera(char **tokens, t_parse_context *ctx,
							t_camera_data *data)
{
	double	length;

	if (!helper_validate_camera(tokens, ctx, data))
		return (false);
	length = vec3_length(data->orientation);
	if (length < 1e-6)
	{
		ft_error_and_free(ctx, tokens,
			"Camera: orientation vector cannot be zero");
		return (false);
	}
	data->orientation = vec3_normalize(data->orientation);
	data->fov = ft_atod(tokens[3]);
	if (data->fov <= 0.0 || data->fov >= 180.0)
	{
		ft_error_and_free(ctx, tokens, "Camera: FOV must be in range (0, 180)");
		return (false);
	}
	return (true);
}

static void	calculate_camera_basis(t_camera *camera)
{
	t_vec3	world_up;
	t_vec3	right;
	t_vec3	up;
	double	dot;

	world_up = (t_vec3){0.0, 1.0, 0.0};
	dot = fabs(vec3_dot(camera->forward, world_up));
	if (dot > 0.999)
		world_up = (t_vec3){1.0, 0.0, 0.0};
	right = vec3_cross(camera->forward, world_up);
	camera->right = vec3_normalize(right);
	up = vec3_cross(camera->right, camera->forward);
	camera->up = vec3_normalize(up);
}

static t_camera	*create_camera_data(t_camera_data *data, int width, int height)
{
	t_camera	*camera;

	camera = (t_camera *)malloc(sizeof(t_camera));
	if (!camera)
		return (NULL);
	camera->position = data->position;
	camera->forward = data->orientation;
	camera->fov = data->fov;
	calculate_camera_basis(camera);
	calculate_viewport(camera, width, height);
	return (camera);
}

bool	parse_camera(t_parse_context *ctx, t_scene *scene, const char *line)
{
	char			**tokens;
	t_camera_data	data;
	t_camera		*camera;

	tokens = ft_split(line, ' ');
	if (!tokens)
		return (false);
	if (!validate_camera(tokens, ctx, &data))
		return (false);
	camera = create_camera_data(&data, scene->width, scene->height);
	if (!camera)
	{
		ft_error_and_free(ctx, tokens, "Camera: memory allocation failed");
		return (false);
	}
	scene->camera = *camera;
	free(camera);
	ctx->camera_count++;
	ft_free_split(tokens);
	return (true);
}
