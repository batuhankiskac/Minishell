/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_line.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 15:16:30 by bkiskac           #+#    #+#             */
/*   Updated: 2025/07/16 22:57:20 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Parses the tokenized line into commands and handles expansions.
 *
 * This function orchestrates the main parsing logic after tokenization.
 * It builds the command list, parses the commands and their redirections,
 * and finally applies expansions.
 *
 * @param shell A pointer to the shell structure.
 * @return 0 on success, or a non-zero error code on failure.
 */
static int	parse_and_expand_pipeline(t_shell *shell)
{
	if (!build_command_list(shell) || !parse_commands(shell)
		|| !parse_redirections(shell) || !expander(shell))
	{
		shell->exit_status = 2;
		return (print_error(NULL, NULL, "parse error", 2));
	}
	return (0);
}

/**
 * @brief Tokenizes the input line and then parses it into commands.
 *
 * This function handles the initial stages of processing: tokenizing the
 * input line and, if successful, calling the parsing and expansion pipeline.
 *
 * @param shell A pointer to the shell structure.
 * @return 0 on success, or a non-zero error code if tokenization
 * or parsing fails.
 */
static int	tokenize_and_parse(t_shell *shell)
{
	if (tokenize_line(shell->line, shell) == ERROR)
	{
		shell->exit_status = 2;
		return (print_error(NULL, NULL, "parse error", 2));
	}
	if (shell->tokens == NULL)
		return (0);
	return (parse_and_expand_pipeline(shell));
}

/**
 * @brief Handles heredoc processing and executes the command pipeline.
 *
 * This function first processes any here-documents defined in the commands.
 * If successful and not interrupted, it proceeds to execute
 * the command pipeline.
 *
 * @param shell A pointer to the shell structure.
 * @return 0 on success, or a specific exit code on failure or interruption.
 */
static int	execute_pipeline(t_shell *shell)
{
	int	heredoc_result;

	heredoc_result = handle_heredoc_redir(shell);
	if (heredoc_result != 0)
	{
		if (heredoc_result == 1)
			return (130);
		return (1);
	}
	if (shell->command)
		run_command(shell);
	return (shell->exit_status);
}

/**
 * @brief Processes a single line of input from the user.
 *
 * This is the high-level orchestrator for processing a command line. It
 * initializes, tokenizes, parses, executes, and cleans up resources.
 *
 * @param raw_line_ptr A pointer to the raw line string entered by the user.
 * @param shell A pointer to the `t_shell` structure containing the shell state.
 * @return Returns 0, as the main loop should continue. The exit status is
 * managed within the shell structure.
 */
int	process_line(char *raw_line_ptr, t_shell *shell)
{
	int	status;

	if (shell->line)
		free(shell->line);
	shell->line = ft_strdup(raw_line_ptr);
	if (!shell->line)
		shell->exit_status = print_error(NULL, NULL, strerror(errno), 1);
	else
	{
		status = tokenize_and_parse(shell);
		if (status == 0 && shell->command)
			shell->exit_status = execute_pipeline(shell);
		else if (status != 0)
			shell->exit_status = status;
	}
	if (shell->line && *shell->line)
		add_history(shell->line);
	cleanup_iteration_resources(raw_line_ptr, shell);
	return (0);
}
