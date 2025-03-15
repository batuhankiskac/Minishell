/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 14:07:50 by bkiskac           #+#    #+#             */
/*   Updated: 2025/03/15 16:55:21 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
 * parse_export_arg - Parses a single export argument.
 *
 * This helper function splits an export argument into key and value parts.
 * It first validates the identifier, then splits it at '=' if present.
 * Allocates memory for key and value, returning ERROR if allocation fails.
 *
 * Parameters:
 *   arg   - The export argument string.
 *   key   - Address to store the pointer for the exported key.
 *   value - Address to store the pointer for the exported value.
 *
 * Returns:
 *   0     - On success.
 *   ERROR - On failure.
 */
static int	parse_export_arg(char *arg, char **key, char **value)
{
	char	*equal_sign;

	if (!is_valid_env(arg))
		return (ERROR);
	equal_sign = ft_strchr(arg, '=');
	if (equal_sign)
	{
		*key = ft_substr(arg, 0, equal_sign - arg);
		*value = ft_strdup(equal_sign + 1);
	}
	else
	{
		*key = ft_strdup(arg);
		*value = ft_strdup("");
	}
	if (!(*key) || !(*value))
	{
		free(*key);
		free(*value);
		return (ERROR);
	}
	return (0);
}

/*
 * builtin_export - Executes the export builtin command.
 *
 * This function processes command-line arguments for
 * exporting environment variables.
 * If no argument is provided, it prints the sorted environment list.
 * Otherwise, it validates each argument, updates the environment accordingly,
 * and prints an error message if an invalid identifier is found.
 *
 * Parameters:
 *   argc - The count of command-line arguments.
 *   argv - An array of strings representing command-line arguments.
 *   env  - A pointer to the linked list of environment variables.
 *
 * Returns:
 *   0     - Upon successful execution.
 *   ERROR - Upon encountering an error.
 */
int	builtin_export(int argc, char **args, t_env **env)
{
	int		i;
	char	*key;
	char	*value;

	i = 0;
	if (argc == 1)
		return (print_sorted_env(*env), 0);
	while (++i < argc)
	{
		if (parse_export_arg(args[i], &key, &value) == ERROR)
		{
			ft_putstr_fd("export: not a valid identifier: ", 2);
			ft_putendl_fd(args[i], 2);
			return (ERROR);
		}
		else
		{
			update_env(key, value, env);
			free(key);
			free(value);
		}
	}
	return (0);
}
