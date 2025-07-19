/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rebuild_args.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 07:24:25 by bkiskac           #+#    #+#             */
/*   Updated: 2025/07/19 07:24:42 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Counts the number of non-empty strings in an argument array.
 * @param args The argument array to count.
 * @return The number of non-empty arguments.
 */
static int	count_non_empty_args(char **args)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (args && args[i])
	{
		if (args[i][0] != '\0')
			count++;
		i++;
	}
	return (count);
}

/**
 * @brief Populates a new argument array with non-empty strings from the old one.
 *
 * It moves the string pointers from the old array to the new one and
 * frees the original empty strings.
 *
 * @param new_args The destination array.
 * @param old_args The source array.
 */
static void	populate_new_args(char **new_args, char **old_args)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (old_args && old_args[i])
	{
		if (old_args[i][0] != '\0')
		{
			new_args[j] = old_args[i];
			j++;
		}
		else
			free(old_args[i]);
		i++;
	}
	new_args[j] = NULL;
}

/**
 * @brief Rebuilds the command's argument list to remove empty strings.
 *
 * After expansion, some arguments might become empty strings (e.g., from
 * an empty environment variable). This function filters them out, creating
 * a new, clean argument list, and updates the command structure.
 *
 * @param cmd The command to process.
 * @return 1 on success, 0 on failure (malloc error).
 */
int	rebuild_command_args(t_command *cmd)
{
	char	**new_args;
	int		new_argc;

	if (!cmd || !cmd->args)
		return (1);
	new_argc = count_non_empty_args(cmd->args);
	if (new_argc == cmd->argc)
		return (1);
	new_args = ft_calloc(new_argc + 1, sizeof(char *));
	if (!new_args)
		return (print_error(NULL, NULL, strerror(errno), 0));
	populate_new_args(new_args, cmd->args);
	free(cmd->args);
	cmd->args = new_args;
	cmd->argc = new_argc;
	if (cmd->argc > 0)
		cmd->cmd = cmd->args[0];
	else
		cmd->cmd = NULL;
	return (1);
}
