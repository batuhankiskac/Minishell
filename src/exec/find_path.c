/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 22:12:49 by bkiskac           #+#    #+#             */
/*   Updated: 2025/07/23 21:24:55 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Extracts the command name from a command string.
 *
 * This utility function takes a command string and extracts just the
 * command name (first word) by finding the first space or using the
 * entire string if no space is found.
 *
 * @param cmd The command string (e.g., "ls -l" or "ls").
 * @return A dynamically allocated string containing just the command name.
 * Returns `NULL` on error or if `cmd` is invalid.
 */
static char	*extract_cmd_name(char *cmd)
{
	char	*space_pos;
	int		cmd_len;

	if (!cmd || !*cmd)
		return (NULL);
	space_pos = ft_strchr(cmd, ' ');
	if (space_pos)
		cmd_len = space_pos - cmd;
	else
		cmd_len = ft_strlen(cmd);
	return (ft_substr(cmd, 0, cmd_len));
}

/**
 * @brief Searches for an executable in paths, checking only for existence.
 *
 * This function iterates through each path in the `allpath` array and
 * constructs a full potential path for the command. It then checks if this
 * path exists using `access(exec, F_OK)`. The executability check (X_OK) is
 * deferred to the execution stage to allow for more precise error messages
 * (e.g., "Permission denied" instead of "command not found").
 *
 * @param allpath A null-terminated array of directory paths.
 * @param cmd_name The command name to search for.
 * @return A dynamically allocated string with the full path if found,
 * otherwise NULL.
 */
static char	*process_path_search(char **all_path, char *cmd_name)
{
	char	*exec;
	char	*path_part;
	int		i;

	i = -1;
	while (all_path[++i])
	{
		path_part = ft_strjoin(all_path[i], "/");
		exec = ft_strjoin(path_part, cmd_name);
		free(path_part);
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
 * @brief Searches for a command in directories specified by a PATH string.
 *
 * @param cmd_name The command name.
 * @param path_list_str A colon-separated string of directory paths.
 * @return A dynamically allocated string containing the full path to the
 * executable if found. Returns `NULL` if not found or on error.
 */
static char	*find_command_in_path(char *cmd_name, char *path_list_str)
{
	char	**all_path_arr;
	char	*exec_res;

	all_path_arr = ft_split(path_list_str, ':');
	if (!all_path_arr)
	{
		if (access(cmd_name, F_OK) == 0)
			return (ft_strdup(cmd_name));
		return (NULL);
	}
	exec_res = process_path_search(all_path_arr, cmd_name);
	return (exec_res);
}

/**
 * @brief Processes command path resolution for commands without '/'.
 *
 * @param cmd_name The extracted command name to search for.
 * @param envp An array of strings representing the environment variables.
 * @return A dynamically allocated string containing the full path to the
 * executable if found, or NULL if not found or on error.
 */
static char	*process_path_resolution(char *cmd_name, char *envp[])
{
	char	*path_env_val;
	char	*result_path;

	if (!envp)
		return (NULL);
	path_env_val = find_in_envp("PATH", envp);
	if (!path_env_val)
		return (NULL);
	result_path = find_command_in_path(cmd_name, path_env_val);
	return (result_path);
}

/**
 * @brief Finds the full path of an executable command.
 *
 * This function determines the path of a command. If the command contains a
 * '/', it's treated as a direct path. Otherwise, it searches the directories
 * in the PATH environment variable. It only checks for file existence (F_OK),
 * deferring the executability check (X_OK) to the execution stage to provide
 * bash-compliant error messages like "Permission denied".
 *
 * @param cmd The command string (e.g., "ls", "/bin/ls").
 * @param envp The environment array.
 * @return A dynamically allocated string with the full path if found,
 * otherwise NULL.
 */
char	*find_path(char *cmd, char *envp[])
{
	char	*cmd_name;
	char	*result_path;

	cmd_name = extract_cmd_name(cmd);
	if (!cmd_name)
		return (NULL);
	if (ft_strchr(cmd_name, '/'))
	{
		if (access(cmd_name, F_OK) == 0)
			result_path = ft_strdup(cmd_name);
		else
			result_path = NULL;
	}
	else
		result_path = process_path_resolution(cmd_name, envp);
	free(cmd_name);
	return (result_path);
}
