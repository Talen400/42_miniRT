/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgregori <rgregori@student.42sp.org.br>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-01-29 17:40:19 by rgregori          #+#    #+#             */
/*   Updated: 2026/02/09 17:14:21 by tlavared         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRt_bonus.h"
#include "parser_bonus.h"
#include "scene_bonus.h"
#include "mlx_rt_bonus.h"

static bool	validate_args(int argc, char **argv)
{
	int	len;

	if (argc != 2)
	{
		ft_print_error("Error: usage ./miniRT <scene.rt>\n");
		return (false);
	}
	len = ft_strlen(argv[1]);
	if (len < 4 || ft_strncmp(argv[1] + len - 3, ".rt", 3) != 0)
	{
		ft_print_error("Error: file must have .rt extension\n");
		return (false);
	}
	return (true);
}

static int	init_mlx(t_minirt *minirt)
{
	minirt->mlx.mlx_ptr = mlx_init(WIDTH, HEIGHT, "miniRt", true);
	if (!minirt->mlx.mlx_ptr)
		return (ft_errorinit(minirt->mlx.mlx_ptr));
	minirt->mlx.img_ptr = mlx_new_image(minirt->mlx.mlx_ptr, WIDTH, HEIGHT);
	if (!minirt->mlx.img_ptr)
		return (ft_errorinit(minirt->mlx.mlx_ptr));
	minirt->mlx.pixels = minirt->mlx.img_ptr->pixels;
	pthread_mutex_init(&minirt->mlx.render_mutex, NULL);
	return (0);
}

static void	helper_events(t_minirt *minirt)
{
	mlx_key_hook(minirt->mlx.mlx_ptr, ft_on_keypress, minirt);
	mlx_close_hook(minirt->mlx.mlx_ptr, ft_on_close, minirt);
	mlx_resize_hook(minirt->mlx.mlx_ptr, ft_on_resize, minirt);
}

static void	cleanup(t_minirt *minirt)
{
	destroy_scene(&minirt->scene);
	pthread_mutex_destroy(&minirt->mlx.render_mutex);
	mlx_terminate(minirt->mlx.mlx_ptr);
}

int	main(int argc, char **argv)
{
	t_minirt	minirt;

	if (!validate_args(argc, argv))
		return (1);
	ft_memset(&minirt, 0, sizeof(t_minirt));
	minirt.scene = scene_init();
	if (!parse_scene(argv[1], &minirt.scene))
	{
		ft_print_error("Failed to parse scene file");
		destroy_scene(&minirt.scene);
		return (1);
	}
	if (init_mlx(&minirt))
		return (destroy_scene(&minirt.scene), 1);
	draw(&minirt);
	if (mlx_image_to_window(minirt.mlx.mlx_ptr, minirt.mlx.img_ptr, 0, 0) == -1)
		return (ft_errorimg(minirt.mlx.mlx_ptr, minirt.mlx.img_ptr));
	helper_events(&minirt);
	mlx_loop(minirt.mlx.mlx_ptr);
	cleanup(&minirt);
	return (0);
}
