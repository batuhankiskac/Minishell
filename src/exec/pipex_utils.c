/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 12:26:40 by bkiskac           #+#    #+#             */
/*   Updated: 2025/03/29 20:38:09 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	show_error(char *str)
{
	perror(str);
	exit(EXIT_FAILURE);
}

int	open_file(const char *filename, int flags, int mode)
{
	int	fd;

	fd = open(filename, flags, mode);
	if (fd < 0)
		show_error("Error opening file");
	return (fd);
}

void	ft_free_all(char **str)
{
	int	i;

	i = 0;
	while (str[i])
		free(str[i++]);
	free(str);
}

char	*ft_getenv(char *name, char *envp[])
{
	int		i;
	int		j;
	char	*sub;

	i = 0;
	while (envp[i])
	{
		j = 0;
		while (envp[i][j] && envp[i][j] != '=')
			j++;
		sub = ft_substr(envp[i], 0, j);
		if (ft_strcmp(sub, name) == 0)
		{
			free(sub);
			return (envp[i] + j + 1);
		}
		free(sub);
		i++;
	}
	return (NULL);
}

void	execute_cmd(char *cmd, char *envp[])
{
	char	*cmd_path;
	char	**argv;

	if (!cmd || !*cmd)
	{
		ft_putstr_fd("Command not found\n", 2);
		exit(127);
	}
	argv = ft_split(cmd, ' ');
	if (!argv || !*argv)
	{
		ft_putstr_fd("Command not found\n", 2);
		exit(127);
	}
	cmd_path = find_path(argv[0], envp);
	if (!cmd_path)
	{
		ft_putstr_fd("Command not found\n", 2);
		exit(127);
	}
	if (execve(cmd_path, argv, envp) == -1)
		show_error("Error executing command");
}
