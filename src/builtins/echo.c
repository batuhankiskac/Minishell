/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/07/07 13:25:36 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Checks if a string is a valid -n flag (e.g., -n, -nn, -nnn, etc.)
 *
 * @param str The string to check
 * @return 1 if it's a valid -n flag, 0 otherwise
 */
static int	is_n_flag(char *str)
{
	int	i;

	if (!str || str[0] != '-' || str[1] != 'n')
		return (0);
	i = 2;
	while (str[i])
	{
		if (str[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

/**
 * @brief Implements the built-in echo command.
 *
 * This function prints arguments to the standard output.
 * It handles the '-n' flag to suppress the newline character at the end.
 * It accepts multiple forms of -n flag like -n, -nn, -nnn, etc.
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
	while (i < argc && args[i] && is_n_flag(args[i]))
	{
		nl_flag = 0;
		i++;
	}
	while (i < argc)
	{
		ft_putstr_fd(args[i], STDOUT_FILENO);
		if (i + 1 < argc)
			ft_putchar_fd(' ', STDOUT_FILENO);
		i++;
	}
	if (nl_flag)
		ft_putchar_fd('\n', STDOUT_FILENO);
	return (0);
}
