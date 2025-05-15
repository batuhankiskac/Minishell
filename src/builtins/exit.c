/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/05/15 14:27:02 by nkahrima         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
static int	is_numeric(char *str)
{
	int	i;
	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

static void	free_all_resources(t_env **env)
{
	if (env && *env)
		free_env(*env);
	*env = NULL;
}

int	builtin_exit(int argc, char **args, t_env **env)
{
	int	status;
	if (argc == 1)
	{
		free_all_resources(env);
		exit(0);
	}
	else if (argc == 2)
	{
		if (is_numeric(args[1]))
		{
			status = ft_atoi(args[1]);
			free_all_resources(env);
			exit(status);
		}
		else
			return (ft_putendl_fd("exit: numeric argument required", 2), ERROR);
	}
	else
		return (ft_putendl_fd("exit: too many arguments", 2), ERROR);
}

