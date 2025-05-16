/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop_error_handling.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 21:31:28 by bkiskac           #+#    #+#             */
/*   Updated: 2025/05/16 21:31:28 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_command_parsing_error(char *raw_line_ptr, t_shell *shell)
{
	free(raw_line_ptr);
	clear_command_list(shell->command);
	shell->command = NULL;
	clear_token_list(&shell->tokens);
	shell->tokens = NULL;
	return (1);
}

void	cleanup_current_loop_iteration(char *raw_line_ptr, t_shell *shell)
{
	clear_command_list(shell->command);
	shell->command = NULL;
	clear_token_list(&shell->tokens);
	shell->tokens = NULL;
	free(raw_line_ptr);
}
