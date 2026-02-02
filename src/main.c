/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgregori <rgregori@student.42sp.org.br>    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-01-29 17:40:19 by rgregori          #+#    #+#             */
/*   Updated: 2026-01-29 17:40:19 by rgregori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRt.h"
#include <stdio.h>

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		printf("Usage: %s <scene.rt>\n", argv[0]);
		return 1;
	}
	printf("Parsing scene file: %s\n", argv[1]);
	return 0;
}
