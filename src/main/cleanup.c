/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop_errors.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 21:31:28 by bkiskac           #+#    #+#             */
/*   Updated: 2025/07/01 14:47:37 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Cleans up resources after a command loop iteration.
 *
 * This function is called at the end of each command loop iteration to
 * clean up resources. It can be used both for normal cleanup and error
 * cleanup scenarios. It clears the command list, clears the token list,
 * and frees the raw line pointer.
 *
 * @param raw_line_ptr Pointer to the raw line string that needs to be freed.
 * @param shell Pointer to the shell structure containing command and tokens.
 */
void	cleanup_iteration_resources(char *raw_line_ptr, t_shell *shell)
{
	clear_command_list(shell->command);
	shell->command = NULL;
	clear_token_list(&shell->tokens);
	shell->tokens = NULL;
	free(raw_line_ptr);
}
