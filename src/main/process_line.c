/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_line.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 15:16:30 by bkiskac           #+#    #+#             */
/*   Updated: 2025/07/14 12:27:18 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Handles the parsing phase of command processing.
 *
 * @param shell A pointer to the shell structure
 * @return 0 on success, 1 on failure
 */
static int	handle_parsing(t_shell *shell)
{
	if (!build_command_list(shell))
		return (1);
	if (!parse_commands(shell))
		return (1);
	if (!parse_redirections(shell))
		return (1);
	if (!expander(shell))
		return (1);
	return (0);
}

/**
 * @brief Initializes the shell for processing a new line.
 *
 * @param raw_line_ptr A pointer to the raw line string
 * @param shell A pointer to the shell structure
 * @return 0 on success, 1 on failure
 */
static int	initialize_shell_line(char *raw_line_ptr, t_shell *shell)
{
	shell->line_number++;
	if (shell->line)
		free(shell->line);
	shell->line = ft_strdup(raw_line_ptr);
	if (!shell->line)
	{
		free(raw_line_ptr);
		return (print_error(NULL, NULL, strerror(errno), 1));
	}
	return (0);
}

/**
 * @brief Handles parsing and tokenization errors and cleanup.
 *
 * @param raw_line_ptr A pointer to the raw line string
 * @param shell A pointer to the shell structure
 * @return Error code
 */
static int	handle_parse_error(char *raw_line_ptr, t_shell *shell)
{
	shell->exit_status = 2;
	cleanup_iteration_resources(raw_line_ptr, shell);
	return (print_error(NULL, NULL, "parse error", 2));
}

/**
 * @brief Processes a single line of input from the user.
 *
 * This function takes a raw line of input, tokenizes it, builds a command list,
 * parses commands and redirections, and executes the commands. It handles errors
 * during parsing and execution, cleaning up resources as necessary.
 *
 * @param raw_line_ptr A pointer to the raw line string entered by the user.
 * @param shell A pointer to the `t_shell` structure containing the shell state.
 * @return Returns 0 on success, or 1 if an error occurs during processing.
 */
int	process_line(char *raw_line_ptr, t_shell *shell)
{
	printf("DEBUG: process_line: called with line='%s'\n", raw_line_ptr);
	if (initialize_shell_line(raw_line_ptr, shell))
		return (1);
	if (tokenize_line(shell->line, shell) == ERROR)
		return (handle_parse_error(raw_line_ptr, shell));
	if (shell->tokens == NULL)
	{
		printf("DEBUG: process_line: no tokens, cleaning up\n");
		cleanup_iteration_resources(raw_line_ptr, shell);
		return (0);
	}
	if (handle_parsing(shell))
		return (handle_parse_error(raw_line_ptr, shell));
	printf("DEBUG: process_line: before handle_heredoc_redir\n");
	if (handle_heredoc_redir(shell) == ERROR)
	{
		shell->exit_status = 130;
		printf("DEBUG: process_line: heredoc error, exit_status=%d\n", shell->exit_status);
		cleanup_iteration_resources(raw_line_ptr, shell);
		return (1);
	}
	printf("DEBUG: process_line: after handle_heredoc_redir\n");
	if (shell->command)
		run_command(shell);
	if (*shell->line)
		add_history(shell->line);
	cleanup_iteration_resources(raw_line_ptr, shell);
	printf("DEBUG: process_line: finished, exit_status=%d\n", shell->exit_status);
	return (0);
}
