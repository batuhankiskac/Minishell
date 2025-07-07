/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_line.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 15:16:30 by bkiskac           #+#    #+#             */
/*   Updated: 2025/07/07 17:40:10 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Updates shell line from heredoc content file if it exists.
 *
 * @param shell A pointer to the shell structure
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
 * @brief Handles the parsing phase of command processing.
 *
 * @param shell A pointer to the shell structure
 * @return 0 on success, 1 on failure
 */
static int	handle_parsing(t_shell *shell)
{
	if (tokenize_line(shell->line, shell) == ERROR
		|| !build_command_list(shell)
		|| !parse_commands(shell)
		|| !parse_redirections(shell)
		|| !expander(shell))
	{
		return (1);
	}
	return (0);
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
	shell->line_number++;
	shell->heredoc_eof = 0;
	if (shell->line)
		free(shell->line);
	shell->line = ft_strdup(raw_line_ptr);
	if (!shell->line)
	{
		ft_printf(2, "minishell: ft_strdup failed: %s\n", strerror(errno));
		free(raw_line_ptr);
		return (1);
	}
	if (handle_parsing(shell))
	{
		ft_printf(2, "minishell: syntax error\n");
		shell->exit_status = 2;
		cleanup_iteration_resources(raw_line_ptr, shell);
		return (1);
	}
	else if (shell->command && !shell->heredoc_eof)
		run_command(shell);
	update_line(shell);
	if (*shell->line)
		add_history(shell->line);
	cleanup_iteration_resources(raw_line_ptr, shell);
	return (0);
}
