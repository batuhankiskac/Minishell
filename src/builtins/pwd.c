/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/05/31 14:58:37 by bkiskac          ###   ########.fr       */
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
		ft_putstr_fd("pwd: ", 2);
		ft_putendl_fd(strerror(errno), 2);
		return (ERROR);
	}
	ft_putendl_fd(cwd, STDOUT_FILENO);
	return (free(cwd), 0);
}
