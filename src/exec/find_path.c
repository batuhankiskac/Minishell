/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 22:12:49 by bkiskac           #+#    #+#             */
/*   Updated: 2025/05/16 22:30:03 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
