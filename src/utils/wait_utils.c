/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 17:30:00 by bkiskac           #+#    #+#             */
/*   Updated: 2025/07/19 17:28:40 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Processes wait status and returns appropriate exit code.
 *
 * This function handles the status returned by wait/waitpid and converts
 * it to the appropriate shell exit code. It handles both signaled and
 * normally exited processes, printing appropriate messages for signals.
 * Optionally frees an environment array if provided.
 *
 * @param status The status returned by wait/waitpid
 * @param env_array Optional environment array to free (can be NULL)
 * @return The appropriate exit code
 * (0-255 for normal exit, 128+signal for signals)
 */
int	handle_wait_status(int status, char **env_array)
{
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
			ft_printf(2, "\n");
		else if (WTERMSIG(status) == SIGQUIT)
			ft_printf(2, "Quit (core dumped)\n");
		if (env_array)
			ft_free_all(env_array);
		return (128 + WTERMSIG(status));
	}
	else if (WIFEXITED(status))
	{
		if (env_array)
			ft_free_all(env_array);
		return (WEXITSTATUS(status));
	}
	if (env_array)
		ft_free_all(env_array);
	return (status);
}
