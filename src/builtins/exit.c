/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 22:02:32 by bkiskac           #+#    #+#             */
/*   Updated: 2025/07/07 13:34:46 by bkiskac          ###   ########.fr       */
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
 * @brief Validates exit arguments and determines the exit status.
 *
 * @param shell A pointer to the t_shell structure.
 * @param status_code A pointer to store the determined exit status.
 * @return 0 if shell should exit, 1 if shell should continue (too many args).
 */
static int	validate_exit_args(t_shell *shell, int *status_code)
{
	if (shell->command->argc == 1)
	{
		*status_code = shell->exit_status;
		return (0);
	}
	if (!is_numeric(shell->command->args[1]))
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(shell->command->args[1], 2);
		ft_putendl_fd(": numeric argument required", 2);
		*status_code = 2;
		return (0);
	}
	if (shell->command->argc > 2)
	{
		ft_putendl_fd("minishell: exit: too many arguments", 2);
		return (1);
	}
	*status_code = ft_atoi(shell->command->args[1]);
	return (0);
}

/**
 * @brief Implements the built-in exit command according to bash behavior.
 *
 * It handles argument validation with specific logic:
 * - `exit`: Exits with the last command's status.
 * - `exit [numeric]`: Exits with the given status code.
 * - `exit [non-numeric] ...`: Prints a "numeric argument required" error
 * and exits with status 2.
 * - `exit [numeric] [any] ...`: Prints a "too many arguments" error,
 * sets $? to 1, but DOES NOT exit the shell.
 *
 * @param shell A pointer to the t_shell structure.
 * @return Returns 1 if there are too many arguments (non-fatal error).
 * Does not return on successful exit, as the process terminates.
 */
int	builtin_exit(t_shell *shell)
{
	int	status_code;
	int	should_exit;

	if (isatty(STDOUT_FILENO))
		ft_putendl_fd("exit", STDOUT_FILENO);
	should_exit = validate_exit_args(shell, &status_code);
	if (should_exit == 1)
		return (1);
	free_shell(shell);
	rl_clear_history();
	exit(status_code & 0xFF);
	return (255);
}
