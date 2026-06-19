/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_norm.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fchaput <fchaput@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/19 11:44:53 by fchaput           #+#    #+#             */
/*   Updated: 2026/06/19 12:00:55 by fchaput          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	print_failded_parsing(char *argv[])
{
	printf("Usage: %s <number_of_coders> <time_to_burnout>", argv[0]);
	printf(" <time_to_compile> <time_to_debug>");
	printf(" <time_to_refactor> <number_of_compiles_required>");
	printf(" <dongle_cooldown> <scheduler>\n");
}

int	*parsing_args(int argc, char *argv[])
{
	int	*args;
	int	i;

	if (argc != 9)
	{
		print_failded_parsing(argv);
		return (NULL);
	}
	args = malloc(sizeof(int) * 7);
	i = 1;
	while (i < argc)
	{
		if (!is_positive(i, argv[i])
			|| !is_valid_argument(i, argv[i])
			|| !is_valid_scheduler(i, argv[i]))
			return (NULL);
		i++;
	}
	i = 0;
	while (i < 7)
	{
		args[i] = atoi(argv[i + 1]);
		i++;
	}
	return (args);
}
