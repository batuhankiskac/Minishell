/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 22:02:32 by bkiskac           #+#    #+#             */
/*   Updated: 2025/05/16 22:04:54 by bkiskac          ###   ########.fr       */
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

static void	handle_exit_numeric_error(const char *arg_val, int *status_code)
{
	ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
	if (arg_val)
		ft_putstr_fd((char *)arg_val, STDERR_FILENO);
	else
		ft_putstr_fd("argument", STDERR_FILENO);
	ft_putendl_fd(": numeric argument required", STDERR_FILENO);
	*status_code = 2;
}

static void	determine_exit_status_and_print_errors(t_shell *shell,
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
			handle_exit_numeric_error(args[1], status_code);
	}
}

static void	free_shell_resources_for_exit(t_shell *shell)
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

int	builtin_exit(t_shell *shell)
{
	int	status_code;

	if (!shell)
		exit(255);
	status_code = shell->exit_status;
	determine_exit_status_and_print_errors(shell, &status_code);
	free_shell_resources_for_exit(shell);
	rl_clear_history();
	exit(status_code & 0xFF);
	return (status_code);
}
