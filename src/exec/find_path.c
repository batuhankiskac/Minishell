/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 22:12:49 by bkiskac           #+#    #+#             */
/*   Updated: 2025/05/30 20:56:12 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Splits a command string into an array of arguments.
 *
 * This utility function takes a command string (`cmd`) and splits it into
 * an array of strings using space (' ') as the delimiter. This is typically
 * used to separate the command from its arguments, though `find_path` primarily
 * uses the first element (the command itself).
 * It handles cases where `cmd` is `NULL` or empty, returning `NULL`.
 * If `ft_split` returns `NULL` or an empty array, it also ensures proper
 * cleanup and returns `NULL`.
 *
 * @param cmd The command string to be split (e.g., "ls -l").
 * @return A null-terminated array of strings (char **) representing the
 *         split command and arguments. Returns `NULL` on error or if `cmd`
 *         is invalid.
 */
static char	**split_cmd(char *cmd)
{
	char	**s_cmd;

	if (!cmd || !*cmd)
		return (NULL);
	s_cmd = ft_split(cmd, ' ');
	if (!s_cmd || !s_cmd[0])
		return (ft_free_all(s_cmd), NULL);
	return (s_cmd);
}

/**
 * @brief Searches for an executable command within a list of directory paths.
 *
 * This function iterates through each path in the `allpath` array. For each
 * path, it constructs a full potential executable path by joining the directory
 * path, a '/', and the command name (`s_cmd_arg[0]`). It then checks if this
 * constructed path exists and is executable using `access(exec, F_OK | X_OK)`.
 * If an executable is found, it frees `allpath` and `s_cmd_arg` and returns
 * the found executable path (`exec`). If no executable is found after checking
 * all paths, it frees `exec` (last attempted path) and returns `NULL`.
 *
 * @param allpath A null-terminated array of strings, where each string is a
 *                directory path from the PATH environment variable.
 * @param s_cmd_arg A null-terminated array of strings, where `s_cmd_arg[0]`
 *                  is the command name to search for.
 * @return A dynamically allocated string containing the full path to the
 *         executable if found. Returns `NULL` if the command is not found
 *         in any of the provided paths or if an error occurs.
 */
static char	*process_path_search_loop(char **allpath, char **s_cmd_arg)
{
	char	*exec;
	char	*path_part;
	int		i;

	i = -1;
	while (allpath[++i])
	{
		path_part = ft_strjoin(allpath[i], "/");
		exec = ft_strjoin(path_part, s_cmd_arg[0]);
		free(path_part);
		if (access(exec, F_OK | X_OK) == 0)
			return (ft_free_all(allpath), ft_free_all(s_cmd_arg), exec);
		free(exec);
	}
	return (NULL);
}

/**
 * @brief Searches for a command in directories specified by a PATH string.
 *
 * This function takes the command arguments (`s_cmd_arg`) and a colon-separated
 * string of paths (`path_list_str`, typically from the PATH environment
 * variable). It first splits `path_list_str` into an array of individual paths.
 * If `path_list_str` is `NULL` or `ft_split` fails (e.g., `path_list_str`
 * is empty or memory allocation fails), it checks if the command itself
 * (`s_cmd_arg[0]`) is a directly accessible executable. If so, it duplicates
 * `s_cmd_arg[0]` and returns it. Otherwise, it calls `process_path_search_loop`
 * to search through the split paths. It ensures `s_cmd_arg` and the array of
 * paths are freed before returning.
 *
 * @param s_cmd_arg A null-terminated array of strings, where `s_cmd_arg[0]`
 *                  is the command name.
 * @param path_list_str A colon-separated string of directory paths (e.g.,
 *                      "/bin:/usr/bin").
 * @return A dynamically allocated string containing the full path to the
 *         executable if found. Returns `NULL` if not found or on error.
 */
static char	*search_env_paths(char **s_cmd_arg, char *path_list_str)
{
	char	**allpath_arr;
	char	*exec_res;

	allpath_arr = ft_split(path_list_str, ':');
	if (!allpath_arr)
	{
		if (access(s_cmd_arg[0], F_OK | X_OK) == 0)
			exec_res = ft_strdup(s_cmd_arg[0]);
		else
			exec_res = NULL;
		ft_free_all(s_cmd_arg);
		return (exec_res);
	}
	exec_res = process_path_search_loop(allpath_arr, s_cmd_arg);
	if (exec_res)
		return (exec_res);
	return (ft_free_all(allpath_arr), ft_free_all(s_cmd_arg), NULL);
}

/**
 * @brief Finds the full path of an executable command.
 *
 * This function determines the absolute or relative path of an executable
 * command.
 * It first splits the input `cmd` string (which might include arguments) using
 * `split_cmd`. If the command name (`s_cmd_arr[0]`) contains a '/', it's
 * treated as a direct path (absolute or relative). The function then checks
 * its accessibility using `access()`. If accessible, the command name is
 * duplicated and returned. Otherwise, `NULL` is returned.
 *
 * If the command name does not contain a '/', the function retrieves the PATH
 * environment variable using `get_env()`. It then calls `search_env_paths()`
 * to look for the command in the directories listed in PATH.
 * The `s_cmd_arr` is freed before returning the result.
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
	char	**s_cmd_arr;
	char	*result_path;
	char	*path_env_val;

	s_cmd_arr = split_cmd(cmd);
	if (!s_cmd_arr)
		return (NULL);
	if (ft_strchr(s_cmd_arr[0], '/'))
	{
		if (access(s_cmd_arr[0], F_OK | X_OK) == 0)
			result_path = ft_strdup(s_cmd_arr[0]);
		else
			result_path = NULL;
		return (ft_free_all(s_cmd_arr), result_path);
	}
	else
	{
		if (!envp)
			return (ft_free_all(s_cmd_arr), NULL);
		path_env_val = get_env("PATH", envp);
		result_path = search_env_paths(s_cmd_arr, path_env_val);
		return (result_path);
	}
}
