/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_line.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 15:16:30 by bkiskac           #+#    #+#             */
/*   Updated: 2025/07/03 11:43:08 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Reads content from a temporary heredoc file and updates the shell's
 *        current line.
 *
 * This function is called after a command execution to check for the existence
 * of a temporary file (`/tmp/minishell_heredoc_content`) that may have been
 * created by a heredoc redirection. If the file exists, its content is read
 * into the shell's `line` buffer. This is used to preserve the input for
 * history.
 *
 * @param shell A pointer to the `t_shell` structure, which contains the `line`
 *              field to be updated.
 */
static void	update_line(t_shell *shell)
{
	int		fd;
	char	buffer[4096];
	int		bytes_read;

	fd = open("/tmp/minishell_heredoc_content", O_RDONLY);
	if (fd != -1)
	{
		bytes_read = read(fd, buffer, sizeof(buffer) - 1);
		close(fd);
		unlink("/tmp/minishell_heredoc_content");
		if (bytes_read > 0)
		{
			buffer[bytes_read] = '\0';
			free(shell->line);
			shell->line = ft_strdup(buffer);
		}
	}
}

/**
 * @brief Executes the core parsing pipeline for a command line.
 *
 * This function orchestrates the sequence of parsing stages:
 * 1. `tokenize_line`: Converts the raw command string into a list of tokens.
 * 2. `build_command_list`: Groups tokens into command structures.
 * 3. `parse_commands`: Validates and finalizes command syntax.
 * 4. `parse_redirections`: Handles I/O redirection operators (`<`, `>` etc.).
 * 5. `expander`: Performs variable expansion (`$VAR`), tilde expansion (`~`),
 *    and quote removal.
 *
 * If any of these stages fail, the function returns an error, and the shell
 * will report a syntax error.
 *
 * @param shell A pointer to the `t_shell` structure containing the line to parse
 *              and where the resulting command structures will be stored.
 * @return Returns 0 on success, or 1 if any parsing stage fails.
 */
static int	handle_parsing(t_shell *shell)
{
	if (tokenize_line(shell->line, shell) == ERROR
		|| !build_command_list(shell)
		|| !parse_commands(shell)
		|| !parse_redirections(shell)
		|| !expander(shell))
		return (1);
	return (0);
}

/**
 * @brief Processes a line containing multiple commands separated by semicolons.
 *
 * This function takes a raw input line, splits it into individual command
 * strings using `split_command_line`, and then processes each command
 * sequentially by calling `handle_command_string`. It ensures that all
 * commands in the sequence are executed, regardless of the success or failure
 * of previous ones.
 *
 * @param raw_line_ptr The raw input string from the user. This function takes
 *                     ownership of the pointer and frees it.
 * @param shell A pointer to the `t_shell` structure.
 * @return Returns 0 if all commands were processed (even if they failed), or 1
 *         if the initial command splitting fails.
 */
int	handle_command_sequence(char *raw_line_ptr, t_shell *shell)
{
	t_seq	*seq;
	int		i;
	int		result;

	seq = split_command_line(raw_line_ptr);
	if (!seq)
		return (free(raw_line_ptr), 1);
	result = 0;
	i = -1;
	while (++i < seq->count)
	{
		if (handle_command_string(seq->commands[i], shell))
			result = 1;
	}
	free_sequence(seq);
	free(raw_line_ptr);
	return (result);
}

/**
 * @brief Processes a single command line (which may contain pipes
 * and redirections).
 *
 * This is the main handler for a line that does not contain semicolons. It
 * duplicates the raw input, runs the full parsing and execution pipeline, and
 * then cleans up. If parsing fails, it prints a syntax error. Otherwise, it
 * executes the command, updates the command history, and performs cleanup.
 *
 * @param raw_line_ptr The raw input string from the user. This function takes
 *                     ownership of the pointer and frees it.
 * @param shell A pointer to the `t_shell` structure.
 * @return Returns 0 on success, 1 on failure (e.g., syntax error).
 */
int	handle_single_command(char *raw_line_ptr, t_shell *shell)
{
	shell->heredoc_eof = 0;
	if (shell->line)
		free(shell->line);
	shell->line = ft_strdup(raw_line_ptr);
	if (!shell->line)
		return (perror("ft_strdup failed"), free(raw_line_ptr), 1);
	if (handle_parsing(shell))
	{
		ft_putendl_fd("minishell: syntax error", STDERR_FILENO);
		shell->exit_status = 2;
		return (cleanup_iteration_resources(raw_line_ptr, shell), 1);
	}
	else if (shell->command && !shell->heredoc_eof)
		run_command(shell);
	update_line(shell);
	if (*shell->line)
		add_history(shell->line);
	cleanup_iteration_resources(raw_line_ptr, shell);
	return (0);
}

/**
 * @brief A variant of `handle_single_command` that takes a command string
 *        directly.
 *
 * This function is used by `handle_command_sequence` to process individual
 * commands from a semicolon-separated list. It performs the same parsing and
 * execution logic as `handle_single_command` but does not manage the
 * `raw_line_ptr`, as the caller (`handle_command_sequence`) is responsible for
 * managing the memory of the command sequence.
 *
 * @param line_content The command string to process.
 * @param shell A pointer to the `t_shell` structure.
 * @return Returns 0 on success, 1 on failure.
 */
int	handle_command_string(char *line_content, t_shell *shell)
{
	shell->heredoc_eof = 0;
	if (shell->line)
		free(shell->line);
	shell->line = ft_strdup(line_content);
	if (!shell->line)
		return (perror("ft_strdup failed"), 1);
	if (handle_parsing(shell))
	{
		ft_putendl_fd("minishell: syntax error", STDERR_FILENO);
		shell->exit_status = 2;
		return (cleanup_iteration_resources(NULL, shell), 1);
	}
	else if (shell->command && !shell->heredoc_eof)
		run_command(shell);
	update_line(shell);
	if (*shell->line)
		add_history(shell->line);
	cleanup_iteration_resources(NULL, shell);
	return (0);
}
