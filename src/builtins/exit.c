/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 22:02:32 by bkiskac           #+#    #+#             */
/*   Updated: 2025/05/30 14:44:13 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Checks if a string is purely numeric.
 *
 * This function verifies if the given string contains only digits,
 * optionally preceded by a single '-' or '+' sign.
 *
 * @param str The string to check.
 * @return 1 if the string is numeric, 0 otherwise.
 */
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

/**
 * @brief Handles the error case for non-numeric exit arguments.
 *
 * This function prints an error message to STDERR indicating that a
 * numeric argument is required for the exit command and sets the
 * status code to 2.
 *
 * @param arg_val The invalid argument string.
 * @param status_code A pointer to the exit status code to be updated.
 */
static void	handle_exit(const char *arg_val, int *status_code)
{
	ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
	if (arg_val)
		ft_putstr_fd((char *)arg_val, STDERR_FILENO);
	else
		ft_putstr_fd("argument", STDERR_FILENO);
	ft_putendl_fd(": numeric argument required", STDERR_FILENO);
	*status_code = 2;
}

/**
 * @brief Determines the exit status and prints "exit" if in interactive mode.
 *
 * This function checks the number of arguments to the exit command.
 * If there are too many arguments, it prints an error and sets the status
 * code to 1. If there is one argument, it checks if it's numeric and sets
 * the status code accordingly. If not numeric, it calls handle_exit.
 * Prints "exit" to STDOUT if the shell is running in a TTY.
 *
 * @param shell A pointer to the shell structure.
 * @param status_code A pointer to the exit status code to be updated.
 */
static void	determine_exit_and_print(t_shell *shell,
		int *status_code)
{
	char	**args;
	int		argc;

	args = NULL;
	argc = 0;
	if (shell->command)
	{
		args = shell->command->args;
		argc = shell->command->argc;
	}
	if (isatty(STDOUT_FILENO))
		ft_putendl_fd("exit", STDOUT_FILENO);
	if (argc > 2)
	{
		ft_putendl_fd("minishell: exit: too many arguments", STDERR_FILENO);
		*status_code = 1;
	}
	else if (argc == 2)
	{
		if (args && args[1] && is_numeric(args[1]))
			*status_code = ft_atoi(args[1]);
		else
			handle_exit(args[1], status_code);
	}
}

/**
 * @brief Frees the memory allocated for the shell structure.
 *
 * This function releases memory for the command line, tokens, command
 * list, and environment list within the shell structure.
 *
 * @param shell A pointer to the shell structure.
 */
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
 * This function handles the termination of the minishell process.
 * It determines the exit status based on the provided arguments,
 * frees allocated memory, clears the readline history, and exits
 * the process with the determined status code.
 *
 * @param shell A pointer to the shell structure.
 * @return Returns the exit status code (though the process exits before
 *         returning).
 */
int	builtin_exit(t_shell *shell)
{
	int	status_code;

	if (!shell)
		exit(255);
	status_code = shell->exit_status;
	determine_exit_and_print(shell, &status_code);
	free_shell(shell);
	rl_clear_history();
	exit(status_code & 0xFF);
	return (status_code);
}
