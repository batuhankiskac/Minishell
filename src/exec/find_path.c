/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 22:12:49 by bkiskac           #+#    #+#             */
/*   Updated: 2025/07/08 21:57:05 by bkiskac          ###   ########.fr       */
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
 *         Returns `NULL` on error or if `cmd` is invalid.
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
 * @brief Searches for an executable command within a list of directory paths.
 *
 * This function iterates through each path in the `allpath` array. For each
 * path, it constructs a full potential executable path by joining the directory
 * path, a '/', and the command name. It then checks if this constructed path
 * exists and is executable using `access(exec, F_OK | X_OK)`. If an executable
 * is found, it frees `allpath` and returns the found executable path (`exec`).
 * If no executable is found after checking all paths, it returns `NULL`.
 *
 * @param allpath A null-terminated array of strings, where each string is a
 *                directory path from the PATH environment variable.
 * @param cmd_name The command name to search for.
 * @return A dynamically allocated string containing the full path to the
 *         executable if found. Returns `NULL` if the command is not found
 *         in any of the provided paths or if an error occurs.
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
		if (access(exec, F_OK | X_OK) == 0)
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
 * This function takes the command name and a colon-separated string of paths
 * (typically from the PATH environment variable). It first splits
 * `path_list_str` into an array of individual paths. If `path_list_str` is
 * `NULL` or `ft_split` fails, it checks if the command itself is a directly
 * accessible executable. If so, it duplicates the command name and returns it.
 * Otherwise, it calls `process_path_search` to search through the split paths.
 *
 * @param cmd_name The command name.
 * @param path_list_str A colon-separated string of directory paths (e.g.,
 *                      "/bin:/usr/bin").
 * @return A dynamically allocated string containing the full path to the
 *         executable if found. Returns `NULL` if not found or on error.
 */
static char	*find_command_in_path(char *cmd_name, char *path_list_str)
{
	char	**all_path_arr;
	char	*exec_res;

	all_path_arr = ft_split(path_list_str, ':');
	if (!all_path_arr)
	{
		if (access(cmd_name, F_OK | X_OK) == 0)
			return (ft_strdup(cmd_name));
		return (NULL);
	}
	exec_res = process_path_search(all_path_arr, cmd_name);
	return (exec_res);
}

/**
 * @brief Handles direct path commands (containing '/').
 *
 * This function processes commands that contain a '/' character, treating
 * them as direct paths (absolute or relative). It checks if the path is
 * accessible and executable.
 *
 * @param cmd_name The command name that contains '/'.
 * @return A dynamically allocated string containing the command name if
 *         accessible and executable, or NULL if not accessible.
 */
static char	*handle_direct_path(char *cmd_name)
{
	if (access(cmd_name, F_OK | X_OK) == 0)
		return (ft_strdup(cmd_name));
	return (NULL);
}

/**
 * @brief Finds the full path of an executable command.
 *
 * This function determines the absolute or relative path of an executable
 * command. It first extracts the command name from the input `cmd` string.
 * If the command name contains a '/', it's treated as a direct path (absolute
 * or relative). The function then checks its accessibility using `access()`.
 * If accessible, the command name is duplicated and returned. Otherwise,
 * `NULL` is returned.
 *
 * If the command name does not contain a '/', the function retrieves the PATH
 * environment variable. It then calls `find_command_in_path()` to look for
 * the command in the directories listed in PATH.
 *
 * @param cmd The command string (e.g., "ls", "/bin/ls", "./myprog").
 * @param envp An array of strings representing the environment variables.
 * @return A dynamically allocated string containing the full path to the
 *         executable if found and accessible. Returns `NULL` if the command
 *         is not found, not executable, or if an error occurs (e.g., memory
 *         allocation failure, invalid `cmd`).
 */
char	*find_path(char *cmd, char *envp[])
{
	char	*cmd_name;
	char	*result_path;
	char	*path_env_val;

	cmd_name = extract_cmd_name(cmd);
	if (!cmd_name)
		return (NULL);
	if (ft_strchr(cmd_name, '/'))
	{
		result_path = handle_direct_path(cmd_name);
		free(cmd_name);
		return (result_path);
	}
	if (!envp)
	{
		free(cmd_name);
		return (NULL);
	}
	path_env_val = find_in_envp("PATH", envp);
	if (!path_env_val)
	{
		free(cmd_name);
		return (NULL);
	}
	result_path = find_command_in_path(cmd_name, path_env_val);
	free(cmd_name);
	return (result_path);
}
