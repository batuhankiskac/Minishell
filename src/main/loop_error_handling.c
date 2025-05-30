/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop_error_handling.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 21:31:28 by bkiskac           #+#    #+#             */
/*   Updated: 2025/05/30 15:20:04 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Handles command parsing errors by cleaning up resources.
 *
 * This function is called when an error occurs during command parsing.
 * It frees the raw line pointer, clears the command list, and clears the
 * token list. It also sets the command pointer to NULL and returns 1 to
 * indicate an error.
 *
 * @param raw_line_ptr Pointer to the raw line string that needs to be freed.
 * @param shell Pointer to the shell structure containing command and tokens.
 * @return 1 to indicate an error occurred.
 */
int	handle_command_parsing_error(char *raw_line_ptr, t_shell *shell)
{
	free(raw_line_ptr);
	clear_command_list(shell->command);
	shell->command = NULL;
	clear_token_list(&shell->tokens);
	shell->tokens = NULL;
	return (1);
}

/**
 * @brief Cleans up resources after a command loop iteration.
 *
 * This function is called at the end of each command loop iteration to
 * clean up resources. It clears the command list, clears the token list,
 * and frees the raw line pointer.
 *
 * @param raw_line_ptr Pointer to the raw line string that needs to be freed.
 * @param shell Pointer to the shell structure containing command and tokens.
 */
void	cleanup_current_loop_iteration(char *raw_line_ptr, t_shell *shell)
{
	clear_command_list(shell->command);
	shell->command = NULL;
	clear_token_list(&shell->tokens);
	shell->tokens = NULL;
	free(raw_line_ptr);
}
