/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/07/07 19:32:01 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Implements the built-in pwd command.
 *
 * This function prints the current working directory to standard output.
 * It uses the getcwd system call to retrieve the current directory path.
 * Handles errors if getcwd fails.
 *
 * @return 0 on success, ERROR on failure.
 */
int	builtin_pwd(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		ft_printf(2, "pwd: %s\n", strerror(errno));
		return (ERROR);
	}
	ft_printf(1, "%s\n", cwd);
	free(cwd);
	return (0);
}
