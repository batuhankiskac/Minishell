/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 16:53:36 by bkiskac           #+#    #+#             */
/*   Updated: 2025/04/13 15:16:53 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
 * get_env - Retrieves the value of an environment variable from the envp array.
 *
 * This function searches for an environment variable in the envp array
 * and returns its value if found. The variable name is expected to be
 * in the format "NAME=VALUE".
 *
 * Parameters:
 *   char *name - The name of the environment variable to search for.
 *   char **envp - The array of environment variables.
 *
 * Returns:
 *   A pointer to the value of the environment variable, or NULL if not found.
 */
char	*get_env(char *name, char *envp[])
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen(name);
	while (envp[i])
	{
		if (ft_strncmp(envp[i], name, len) == 0 && envp[i][len] == '=')
			return (envp[i] + len + 1);
		i++;
	}
	return (NULL);
}

/*
 * find_path - Searches for the full path of a command
 * in the PATH environment variable.
 *
 * This function splits the PATH environment variable into its components
 * and checks each component to see if the command exists and is executable.
 * If found, it returns the full path to the command; otherwise, it returns
 * the original command string.
 *
 * Parameters:
 *   char *cmd - The command to search for.
 *   char **envp - The array of environment variables.
 *
 * Returns:
 *   A pointer to the full path of the command if found, or the original
 *   command string if not found.
 */
char	*find_path(char *cmd, char *envp[])
{
	int		i;
	char	*exec;
	char	**allpath;
	char	*path_part;
	char	**s_cmd;

	allpath = ft_split(get_env("PATH", envp), ':');
	s_cmd = ft_split(cmd, ' ');
	i = -1;
	while (allpath[++i])
	{
		path_part = ft_strjoin(allpath[i], "/");
		exec = ft_strjoin(path_part, s_cmd[0]);
		free(path_part);
		if (access(exec, F_OK | X_OK) == 0)
		{
			ft_free_all(allpath);
			ft_free_all(s_cmd);
			return (exec);
		}
		free(exec);
	}
	ft_free_all(allpath);
	ft_free_all(s_cmd);
	return (cmd);
}
