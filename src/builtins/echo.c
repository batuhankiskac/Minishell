/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/05/29 18:15:21 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_echo(int argc, char **args)
{
	int	i;
	int	nl_flag;

	i = 1;
	nl_flag = 1;
	while (i < argc && args[i] && ft_strncmp(args[i], "-n", 2) == 0)
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
