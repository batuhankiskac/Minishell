/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils_1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/05/16 17:18:30 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*handle_exit_status_expansion(int *i, int exit_status)
{
	*i += 2;
	return (ft_itoa(exit_status));
}

static char	*handle_env_variable_expansion(const char *s, int *i, t_env *env)
{
	int		j;
	char	*key;
	char	*val;
	j = *i + 1;
	while (is_env_char(s[j]))
		j++;
	key = ft_substr(s, *i + 1, j - (*i + 1));
	if (!key)
		return (NULL);
	val = get_env_value(key, env);
	free(key);
	if (!val)
	{
		val = ft_strdup("");
		if (!val)
			return (NULL);
	}
	*i = j;
	return (val);
}

static char	*handle_literal_dollar_expansion(int *i)
{
	(*i)++;
	return (ft_strdup("$"));
}

char	*append_variable(char *res, const char *s,
						int *i, t_env *env, int exit_status)
{
	char	*val;
	char	*new_res;
	if (s[*i + 1] == '?')
		val = handle_exit_status_expansion(i, exit_status);
	else if (is_env_char(s[*i + 1]))
		val = handle_env_variable_expansion(s, i, env);
	else
		val = handle_literal_dollar_expansion(i);
	if (!val)
	{
		free(res);
		return (NULL);
	}
	new_res = ft_strjoin(res, val);
	free(res);
	free(val);
	if (!new_res)
		return (NULL);
	return (new_res);
}

