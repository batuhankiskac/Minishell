/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 22:02:32 by bkiskac           #+#    #+#             */
/*   Updated: 2025/07/09 15:05:18 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_numeric(char *str)
{
	int	i;
	int	has_digits;

	if (!str || *str == '\0')
		return (0);
	i = 0;
	has_digits = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	if (str[i] == '\0')
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		has_digits = 1;
		i++;
	}
	return (has_digits);
}

static void	free_shell(t_shell *shell)
{
	if (shell->line)
	{
		free(shell->line);
		shell->line = NULL;
	}
	if (shell->tokens != NULL)
		clear_token_list(&(shell->tokens));
	if (shell->command != NULL)
		clear_command_list(shell->command);
	if (shell->env)
	{
		free_env(shell->env);
		shell->env = NULL;
	}
}

/**
 * @brief Implements the built-in exit command.
 *
 * Handles the termination of the minishell process. It validates arguments,
 * sets the appropriate exit status, and cleans up all allocated resources
 * before exiting. It correctly handles "too many arguments" as a non-fatal
 * error, unlike other argument errors.
 *
 * @param shell A pointer to the t_shell structure.
 * @return Returns 1 if there are too many arguments (non-fatal error).
 * Does not return on successful exit, as the process terminates.
 */
int	builtin_exit(t_shell *shell)
{
	int	status_code;

	if (isatty(1))
		ft_printf(1, "exit\n");
	if (shell->command->argc >= 2 && !is_numeric(shell->command->args[1]))
	{
		print_error("exit", shell->command->args[1],
			"numeric argument required", 2);
		free_shell(shell);
		rl_clear_history();
		exit(2);
	}
	if (shell->command->argc > 2)
		return (print_error("exit", NULL, "too many arguments", 1));
	status_code = shell->exit_status;
	if (shell->command->argc == 2)
		status_code = ft_atoi(shell->command->args[1]);
	free_shell(shell);
	rl_clear_history();
	exit(status_code & 0xFF);
	return (255);
}
