/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 21:35:19 by bkiskac           #+#    #+#             */
/*   Updated: 2025/03/03 21:44:10 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	builtin_pwd(void)
{
	char	*cwd;

	if (!(cwd = getcwd(cwd, sizeof(cwd))))
	{
		perror("pwd error");
		return (ERROR);
	}
	printf("%s\n", cwd);
	free(cwd);
	return (SUCCESS);
}
