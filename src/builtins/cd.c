/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/05/16 20:32:05 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_target(int argc, char **args, t_env **env)
{
	char	*target;

	if (argc < 2)
	{
		target = get_env_value("HOME", *env);
		if (!target)
		{
			ft_putendl_fd("cd: HOME not set", 2);
			return (free(target), NULL);
		}
	}
	else
	{
		target = ft_strdup(args[1]);
		if (!target)
			return (perror("cd error"), NULL);
	}
	return (target);
}

static int	change_directory(char *target, char *new_pwd)
{
	if (chdir(target) == -1)
	{
		perror("cd error");
		return (ERROR);
	}
	if (!getcwd(new_pwd, PATH_MAX))
	{
		perror("getcwd error");
		return (ERROR);
	}
	return (0);
}

int	builtin_cd(int argc, char **args, t_env **env)
{
	char	old_pwd[PATH_MAX];
	char	new_pwd[PATH_MAX];
	char	*target;

	if (!getcwd(old_pwd, PATH_MAX))
		return (perror("getcwd error"), ERROR);
	target = get_target(argc, args, env);
	if (!target)
		return (ERROR);
	if (change_directory(target, new_pwd) == ERROR)
		return (free(target), ERROR);
	update_env("OLDPWD", old_pwd, env);
	update_env("PWD", new_pwd, env);
	free(target);
	return (0);
}
