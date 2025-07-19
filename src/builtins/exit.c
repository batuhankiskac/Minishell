/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 22:02:32 by bkiskac           #+#    #+#             */
/*   Updated: 2025/07/19 11:06:22 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Parses the numeric part of the string, checking for overflow.
 *
 * This is a helper for is_valid_long_long. It iterates through the digits
 * of the string, building the number and continuously checking if adding the
 * next digit would cause an overflow or underflow against long long limits.
 *
 * @param str The string containing just the digits.
 * @param sign The sign of the number (1 or -1).
 * @return 1 if the numeric part is valid and within limits, 0 otherwise.
 */
static int	parse_numeric_part(const char *str, int sign)
{
	long long	result;
	int			i;

	result = 0;
	i = 0;
	if (str[i] == '\0')
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		if (sign == 1 && result > (LLONG_MAX - (str[i] - '0')) / 10)
			return (0);
		if (sign == -1 && result > (LLONG_MAX - (str[i] - '0'))
			/ 10 + (LLONG_MAX % 10 == 9))
			return (0);
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (1);
}

/**
 * @brief Checks if a string represents a valid long long number.
 *
 * This function validates if a string is a valid number within the range
 * of a long long by checking for an optional sign and then passing the
 * numeric part to a helper function that handles overflow checks.
 *
 * @param str The string to validate.
 * @return 1 if valid, 0 otherwise.
 */
static int	is_valid_long_long(char *str)
{
	int			i;
	int			sign;

	i = 0;
	sign = 1;
	while (ft_isspace(str[i]))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	return (parse_numeric_part(str + i, sign));
}

/**
 * @brief Frees all resources allocated by the shell.
 *
 * This function performs a complete cleanup of all resources allocated by
 * the shell, including heredoc pipes, input line, tokens, commands, and
 * environment variables. It's typically called before exiting the shell
 * to ensure proper cleanup and prevent memory leaks.
 *
 * @param shell A pointer to the t_shell structure containing all resources.
 */
static void	free_shell(t_shell *shell)
{
	close_heredoc_pipes(shell);
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
 * error, and validates numeric arguments against long long limits.
 *
 * @param shell A pointer to the t_shell structure.
 * @return Returns 1 if there are too many arguments (non-fatal error).
 * Does not return on successful exit, as the process terminates.
 */
int	builtin_exit(t_shell *shell)
{
	int	status_code;

	if (isatty(STDOUT_FILENO))
		ft_printf(1, "exit\n");
	if (shell->command->argc >= 2
		&& !is_valid_long_long(shell->command->args[1]))
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
