/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 21:31:28 by bkiskac           #+#    #+#             */
/*   Updated: 2025/07/14 19:02:04 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Closes all file descriptors associated with here-documents.
 * @param shell A pointer to the `t_shell` structure containing the command list.
 */
static void	close_heredoc_pipes(t_shell *shell)
{
	t_command	*cmd;
	t_redir		*redir;

	if (!shell || !shell->command)
		return ;
	cmd = shell->command;
	while (cmd)
	{
		redir = cmd->redir;
		while (redir)
		{
			if (redir->type == REDIR_HEREDOC && redir->heredoc_fd > 2)
			{
				close(redir->heredoc_fd);
				redir->heredoc_fd = -1;
			}
			redir = redir->next;
		}
		cmd = cmd->next;
	}
}

/**
 * @brief Frees all resources used during a single iteration of the command loop.
 * @param raw_line_ptr A pointer to the raw input string read from `readline`.
 * @param shell A pointer to the `t_shell` structure.
 */
void	cleanup_iteration_resources(char *raw_line_ptr, t_shell *shell)
{
	if (!shell)
		return ;
	close_heredoc_pipes(shell);
	if (shell->command)
	{
		clear_command_list(shell->command);
		shell->command = NULL;
	}
	if (shell->tokens)
	{
		clear_token_list(&shell->tokens);
		shell->tokens = NULL;
	}
	if (raw_line_ptr)
		free(raw_line_ptr);
}

/**
 * @brief Frees memory allocated specifically within a child process
 * before exiting.
 *
 * This function should be called from a forked process before it exits,
 * especially on an error path (e.g., command not found).
 * It frees resources that are not managed by the main process, such as
 * the command table, token list, and the duplicated environment array
 * for execve.
 *
 * @param shell A pointer to the main shell data structure.
 * @param env_array The environment array created for execve, if any.
 */
void	cleanup_child_process(t_shell *shell, char **env_array)
{
	if (env_array)
		ft_free_all(env_array);
	if (shell && shell->command)
	{
		clear_command_list(shell->command);
		shell->command = NULL;
	}
	if (shell && shell->tokens)
	{
		clear_token_list(&shell->tokens);
		shell->tokens = NULL;
	}
	if (shell && shell->line)
	{
		free(shell->line);
		shell->line = NULL;
	}
	if (shell && shell->env)
	{
		free_env(shell->env);
		shell->env = NULL;
	}
}

/**
 * @brief Cleans up memory allocated for cd command operations.
 *
 * This utility function frees the memory allocated for old_pwd, new_pwd,
 * and target directory paths. Handles NULL pointers safely.
 *
 * @param old_pwd The old working directory path to free.
 * @param new_pwd The new working directory path to free.
 * @param target The target directory path to free.
 * @param return_value The value to return after cleanup.
 * @return The provided return_value parameter.
 */
int	cleanup_cd_memory(char *old_pwd, char *new_pwd, char *target,
	int return_value)
{
	if (old_pwd)
		free(old_pwd);
	if (new_pwd)
		free(new_pwd);
	if (target)
		free(target);
	return (return_value);
}

/**
 * @brief Helper function to cleanup child process and exit with given status.
 *
 * @param shell A pointer to the shell structure.
 * @param env_array A pointer to the environment array (can be NULL).
 * @param original_head A pointer to restore the original command head.
 * @param exit_code The exit status code.
 */
void	cleanup_child_and_exit(t_shell *shell, char **env_array,
	t_command *original_head, int exit_code)
{
	if (original_head)
		shell->command = original_head;
	cleanup_child_process(shell, env_array);
	exit(exit_code);
}
