/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 22:02:32 by bkiskac           #+#    #+#             */
/*   Updated: 2025/07/19 16:34:51 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Compares a numeric string against long long limits.
 *
 * This is a helper function that performs the actual comparison of a numeric
 * string against the string representation of LLONG_MAX or LLONG_MIN's
 * absolute value.
 *
 * @param num_start Pointer to the start of the digits.
 * @param len The length of the numeric part.
 * @param sign The sign of the number (1 for positive, -1 for negative).
 * @return 1 if the number is within limits, 0 otherwise.
 */
static int	compare_to_limit(const char *num_start, int len, int sign)
{
	const char	*limit;

	limit = "9223372036854775807";
	if (sign == -1)
		limit = "9223372036854775808";
	if (len > (int)ft_strlen(limit)
		|| (len == (int)ft_strlen(limit) && ft_strcmp(num_start, limit) > 0))
		return (0);
	return (1);
}

/**
 * @brief Checks if a string represents a valid long long number.
 *
 * This function validates if a string is a valid number within the range of a
 * long long. It parses spaces and signs, then delegates the core comparison
 * to a helper function.
 *
 * @param str The string to validate.
 * @return 1 if valid, 0 otherwise.
 */
static int	is_valid_long_long(const char *str)
{
	int			i;
	int			sign;
	int			len;
	const char	*num_start;

	i = 0;
	sign = 1;
	while (ft_isspace(str[i]))
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	num_start = &str[i];
	len = 0;
	while (ft_isdigit(num_start[len]))
		len++;
	if (len == 0 || num_start[len] != '\0')
		return (0);
	return (compare_to_limit(num_start, len, sign));
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
		status_code = ft_atoll(shell->command->args[1]);
	free_shell(shell);
	rl_clear_history();
	exit(status_code & 0xFF);
	return (255);
}
