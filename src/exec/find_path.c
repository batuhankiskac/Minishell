/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 22:12:49 by bkiskac           #+#    #+#             */
/*   Updated: 2025/07/24 11:56:28 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Searches for an executable in paths, checking only for existence.
 *
 * This function iterates through each path in the `all_path` array and
 * constructs a full potential path for the command. It then checks if this
 * path exists using `access(exec, F_OK)`.
 *
 * @param all_path A null-terminated array of directory paths.
 * @param cmd The command name to search for.
 * @return A dynamically allocated string with the full path if found,
 * otherwise NULL.
 */
static char	*process_path_search(char **all_path, char *cmd)
{
	char	*exec;
	char	*path_part;
	int		i;

	i = -1;
	while (all_path[++i])
	{
		path_part = ft_strjoin(all_path[i], "/");
		if (!path_part)
			return (ft_free_all(all_path), NULL);
		exec = ft_strjoin(path_part, cmd);
		free(path_part);
		if (!exec)
			return (ft_free_all(all_path), NULL);
		if (access(exec, F_OK) == 0)
		{
			ft_free_all(all_path);
			return (exec);
		}
		free(exec);
	}
	ft_free_all(all_path);
	return (NULL);
}

/**
 * @brief Finds the full path of an executable command.
 *
 * This function determines the path of a command. If the command string
 * contains a '/', it's treated as a direct (relative or absolute) path.
 * Otherwise, it searches the directories in the PATH environment variable
 * for a file with a matching name. This version no longer splits the
 * command string, correctly treating names with spaces like "ls -a" as a
 * single entity to search for, which mimics bash behavior for quoted commands.
 *
 * @param cmd The command string (e.g., "ls", "/bin/ls", "ls -a").
 * @param envp The environment array.
 * @return A dynamically allocated string with the full path if found,
 * otherwise NULL.
 */
char	*find_path(char *cmd, char *envp[])
{
	char	*path_env_val;

	if (!cmd || !*cmd)
		return (NULL);
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, F_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	path_env_val = find_in_envp("PATH", envp);
	if (!path_env_val)
		return (NULL);
	return (process_path_search(ft_split(path_env_val, ':'), cmd));
}
