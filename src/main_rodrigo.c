/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_rodrigo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgregori <rgregori@student.42sp.org.br>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-02-02 17:06:08 by rgregori          #+#    #+#             */
/*   Updated: 2026-02-02 17:06:08 by rgregori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRt.h"
#include <stdio.h>


int main(int argc, char **argv)
{
	t_scene	*scene;

	if (argc != 2)
	{
		printf("Usage: %s <scene.rt>\n", argv[0]);
		return (1);
	}
	scene = malloc (sizeof(t_scene));
	if (!parse_scene(argv[1], scene))
	{
		printf("Error parsing scene file: %s\n", argv[1]);
		free(scene);
		return (1);
	}
	return (0);
}
