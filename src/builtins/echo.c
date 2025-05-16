/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/05/16 20:32:17 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_echo(int argc, char **args)
{
	int	i;
	int	nl_flag;

	i = 1;
	nl_flag = 1;
	if (argc > 1 && !ft_strncmp(args[1], "-n", 2))
	{
		nl_flag = 0;
		i++;
	}
	while (i < argc)
	{
		printf("%s", args[i]);
		if (i + 1 < argc)
			printf(" ");
		i++;
	}
	if (nl_flag)
		printf("\n");
	return (0);
}
