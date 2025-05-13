/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 12:54:03 by bkiskac           #+#    #+#             */
/*   Updated: 2025/05/13 22:21:55 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Fonksiyon proje bitiminde tekrar yazilacak.
// Su an sadece env ile alakali yerleri freeleyip cikiyor.

static int	is_numeric(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

static void	free_all_resources(t_env **env)
{
	if (env && *env)
		free_env(*env);
	*env = NULL;
}

/*
 * builtin_exit - Executes the exit builtin command.
 *
 * This function processes command-line arguments for
 * the exit builtin command. If no arguments are provided,
 * the exit status is set to 0. If an argument is provided,
 * the exit status is set to the argument converted to an integer.
 * If the argument is not a valid integer, an error message is printed.
 *
 * Parameters:
 *   args - A pointer to an array of strings containing the command-line arguments.
 *
 * Returns:
 *   0 - Upon successful execution.
 */
int	builtin_exit(int argc, char **args, t_env **env)
{
	int	status;

	if (argc == 1)
	{
		free_all_resources(env);
		exit(0);
	}
	else if (argc == 2)
	{
		if (is_numeric(args[1]))
		{
			status = ft_atoi(args[1]);
			free_all_resources(env);
			exit(status);
		}
		else
			return (ft_putendl_fd("exit: numeric argument required", 2), ERROR);
	}
	else
		return (ft_putendl_fd("exit: too many arguments", 2), ERROR);
}
