/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/05/29 18:26:53 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_target(int argc, char **args, t_env **env)
{
	char	*raw;

	if (argc < 2)
	{
		raw = get_env_value("HOME", *env);
		if (!raw)
		{
			ft_putendl_fd("cd: HOME not set", STDERR_FILENO);
			return (NULL);
		}
		return (raw);
	}
	return (ft_strdup(args[1]));
}

static int	handle_error(char *msg, char *target, char *old_pwd)
{
	if (target)
		free(target);
	if (old_pwd)
		free(old_pwd);
	ft_putendl_fd(msg, STDERR_FILENO);
	return (ERROR);
}

static int	change_directory(char *target, char **new_pwd)
{
	if (chdir(target) == -1)
	{
		ft_putstr_fd("cd: ", STDERR_FILENO);
		ft_putstr_fd(target, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		ft_putendl_fd(strerror(errno), STDERR_FILENO);
		return (ERROR);
	}
	*new_pwd = getcwd(NULL, 0);
	if (!*new_pwd)
		return (perror("getcwd error"), ERROR);
	return (0);
}

int	builtin_cd(int argc, char **args, t_env **env)
{
	char	*old_pwd;
	char	*new_pwd;
	char	*target;

	if (argc > 2)
		return (ft_putendl_fd("cd: too many arguments", 2), ERROR);
	old_pwd = getcwd(NULL, 0);
	if (!old_pwd && errno != ENOENT)
		return (perror("getcwd error"), ERROR);
	target = get_target(argc, args, env);
	if (!target)
		return (ERROR);
	if (change_directory(target, &new_pwd) == ERROR)
		return (handle_error("cd error", target, old_pwd));
	update_env("OLDPWD", old_pwd, env);
	update_env("PWD", new_pwd, env);
	return (free(old_pwd), free(new_pwd), free(target), 0);
}
