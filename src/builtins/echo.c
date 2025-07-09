/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/07/09 15:31:25 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Checks for -n flags and returns the index where arguments start.
 *
 * This function processes all consecutive -n flags (like -n, -nn, -nnn)
 * and determines if newline should be suppressed.
 *
 * @param argc The number of arguments.
 * @param args The array of arguments.
 * @param nl_flag Pointer to newline flag that will be set.
 * @return The index where actual text arguments start.
 */
static int	process_n_flags(int argc, char **args, int *nl_flag)
{
	int	i;
	int	j;

	i = 1;
	*nl_flag = 1;
	while (i < argc && args[i] && args[i][0] == '-' && args[i][1] == 'n')
	{
		j = 1;
		while (args[i][j] == 'n')
			j++;
		if (args[i][j] == '\0')
		{
			*nl_flag = 0;
			i++;
		}
		else
			break ;
	}
	return (i);
}

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

	i = process_n_flags(argc, args, &nl_flag);
	while (i < argc)
	{
		ft_printf(1, "%s", args[i]);
		if (i + 1 < argc)
			ft_printf(1, " ");
		i++;
	}
	if (nl_flag)
		ft_printf(1, "\n");
	return (0);
}
