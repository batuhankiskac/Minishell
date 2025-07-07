/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/07/07 19:32:01 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Implements the built-in echo command.
 *
 * This function prints arguments to the standard output.
 * It handles the '-n' flag to suppress the newline character at the end.
 * It iterates through the arguments, printing each one separated by a space.
 *
 * @param argc The number of arguments passed to the echo command.
 * @param args The array of arguments passed to the echo command.
 * @return Always returns 0.
 */
int	builtin_echo(int argc, char **args)
{
	int	i;
	int	nl_flag;

	i = 1;
	nl_flag = 1;
	while (i < argc && args[i] && ft_strcmp(args[i], "-n") == 0)
	{
		nl_flag = 0;
		i++;
	}
	while (i < argc)
	{
		ft_putstr_fd(args[i], 1);
		if (i + 1 < argc)
			ft_putchar_fd(' ', 1);
		i++;
	}
	if (nl_flag)
		ft_putchar_fd('\n', 1);
	return (0);
}
