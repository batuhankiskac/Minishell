/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 21:49:35 by bkiskac           #+#    #+#             */
/*   Updated: 2025/03/03 22:00:53 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
 * builtin_echo - Executes the echo builtin command.
 *
 * This function emulates the behavior of the shell's echo command.
 * It processes the command-line arguments
 * and prints them to the standard output.
 * If the first argument is "-n", it suppresses the trailing newline.
 *
 * Parameters:
 *   argc - The count of the command-line arguments.
 *   argv - An array of strings representing the command-line arguments.
 *
 * Returns:
 *   SUCCESS - Upon successful execution.
 */
int	builtin_echo(int argc, char *argv[])
{
	int	i;
	int	nl_flag;

	i = 1;
	nl_flag = 1;
	if (argc > 1 && !ft_strncmp(argv[1], "-n", 2))
	{
		nl_flag = 0;
		i++;
	}
	while (i < argc)
	{
		printf("%s", argv[i]);
		if (i + 1 < argc)
			printf(" ");
		i++;
	}
	if (nl_flag)
		printf("\n");
	return (SUCCESS);
}
