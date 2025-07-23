/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_on_args.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/07/23 17:59:30 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Checks if an expanded argument should be removed from the list.
 *
 * An argument is removed if it's an empty string AND its original,
 * unexpanded form did not contain any quotes. This mimics bash behavior.
 *
 * @param expanded_arg The argument after expansion.
 * @param original_arg The argument before expansion.
 * @return 1 if the argument should be removed, 0 otherwise.
 */
static int	should_remove_arg(const char *expanded_arg,
	const char *original_arg)
{
	if (expanded_arg[0] == '\0')
	{
		if (ft_strchr(original_arg, '\'') == NULL
			&& ft_strchr(original_arg, '"') == NULL)
		{
			return (1);
		}
	}
	return (0);
}

/**
 * @brief Processes and populates the new argument list by expanding old ones.
 *
 * This function iterates through the original arguments, expands each one,
 * decides whether to keep it based on bash's empty string rules, and
 * populates the new argument array.
 *
 * @param new_args The array to be populated with final arguments.
 * @param original_args The pre-expansion argument array.
 * @param shell_context A struct containing the env and exit_status.
 * @return The count of arguments in the new_args array, or -1 on failure.
 */
static int	populate_expanded_args(char **new_args, char **original_args,
	t_shell *shell_context)
{
	int		i;
	int		j;
	char	*expanded_arg;

	i = -1;
	j = 0;
	while (original_args[++i])
	{
		expanded_arg = expand_string(original_args[i], shell_context->env,
				shell_context->exit_status);
		if (!expanded_arg)
			return (-1);
		if (!should_remove_arg(expanded_arg, original_args[i]))
			new_args[j++] = expanded_arg;
		else
			free(expanded_arg);
	}
	return (j);
}

/**
 * @brief Finalizes the argument update by freeing the old list and
 * updating pointers.
 *
 * This helper function takes the newly created argument list and commits the
 * changes to the command structure. It frees the old argument array and
 * updates the command's argc, args, and cmd pointers.
 *
 * @param cmd The command structure to update.
 * @param new_args The newly created argument list.
 * @param new_argc The new argument count.
 * @param original_args The old argument list to be freed.
 */
static void	finalize_args_update(t_command *cmd, char **new_args,
	int new_argc, char **original_args)
{
	ft_free_all(original_args);
	cmd->args = new_args;
	cmd->argc = new_argc;
	if (cmd->argc > 0)
		cmd->cmd = cmd->args[0];
	else
		cmd->cmd = NULL;
}

/**
 * @brief Expands variables in arguments and filters out empty unquoted ones.
 *
 * This function orchestrates the safe expansion of command arguments. It
 * creates a new, clean argument list, frees the old one, and safely updates
 * all relevant pointers in the command structure, delegating tasks to
 * helper functions.
 *
 * @param cmd The command whose arguments are to be expanded.
 * @param env The environment for variable expansion.
 * @param exit_status The last exit status for expanding $?.
 * @return 1 on success, 0 on failure.
 */
int	expand_on_args(t_command *cmd, t_env *env, int exit_status)
{
	char	**new_args;
	int		new_argc;
	t_shell	shell_context;

	if (!cmd->args)
		return (1);
	shell_context.env = env;
	shell_context.exit_status = exit_status;
	new_args = ft_calloc(cmd->argc + 1, sizeof(char *));
	if (!new_args)
		return (0);
	new_argc = populate_expanded_args(new_args, cmd->args, &shell_context);
	if (new_argc == -1)
	{
		ft_free_all(new_args);
		return (0);
	}
	finalize_args_update(cmd, new_args, new_argc, cmd->args);
	return (1);
}
