/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_command.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 17:46:32 by bkiskac           #+#    #+#             */
/*   Updated: 2025/07/08 17:46:39 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Validates the command structure before execution.
 *
 * This function checks if the command structure in the shell is valid.
 * It ensures that the command and its arguments are not NULL and that
 * the command string is not empty. If validation fails, an error message
 * is printed, and an appropriate error code is returned.
 *
 * @param shell A pointer to the shell structure containing the command.
 * @param env_array An array of environment variables for cleanup if needed.
 * @return 0 if the command is valid, or an error code if validation fails.
 */
int	validate_command(t_shell *shell, char **env_array)
{
	if (!shell->command || !shell->command->cmd || !shell->command->args)
	{
		ft_free_all(env_array);
		return (print_error(NULL, NULL, "command not found", 127));
	}
	if (shell->command->cmd[0] == '\0')
	{
		ft_free_all(env_array);
		return (print_error(NULL, NULL, "command not found", 127));
	}
	return (0);
}
