/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 21:35:19 by bkiskac           #+#    #+#             */
/*   Updated: 2025/03/03 21:57:40 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * builtin_pwd - Prints the current working directory to stdout.
 *
 * This function retrieves the current working directory using getcwd(),
 * prints it to standard output, and frees the allocated memory.
 * If getcwd() fails, it outputs an error message
 * via perror and returns an error code.
 *
 * Return:
 *   SUCCESS - if the working directory was successfully retrieved and printed.
 *   ERROR   - if an error occurred while retrieving the current directory.
 */
int	builtin_pwd(void)
{
	char	*cwd;

	cwd = getcwd(cwd, sizeof(cwd));
	if (!cwd)
	{
		perror("pwd error");
		return (ERROR);
	}
	printf("%s\n", cwd);
	free(cwd);
	return (SUCCESS);
}
