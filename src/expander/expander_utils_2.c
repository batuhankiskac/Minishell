/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/05/16 19:29:44 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*append_literal(char *res, const char *s, int start, int end)
{
	char	*substr;
	char	*new_res;
	
	if (start >= end)
		return (res);
	substr = ft_substr(s, start, end - start);
	if (!substr)
	{
		free(res);
		return (NULL);
	}
	new_res = ft_strjoin(res, substr);
	free(res);
	free(substr);
	if (!new_res)
		return (NULL);
	return (new_res);
}

static char	*expand_text_chunk(char *res, const char *s, int *i)
{
	int	j;

	j = *i;
	while (s[j] && s[j] != '$' && s[j] != '\'' && s[j] != '"')
		j++;
	res = append_literal(res, s, *i, j);
	*i = j;
	return (res);
}

static char	*expand_dollar_chunk(char *res, const char *s, int *i,
								t_shell *shell_context)
{
	return (append_variable(res, s, i, shell_context->env,
			shell_context->exit_status));
}

char	*expand_string(const char *s, t_env *env, int exit_status)
{
	char	*res;
	int		i;

	t_shell	shell_context;
	shell_context.env = env;
	shell_context.exit_status = exit_status;
	res = ft_strdup("");
	if (!res)
		return (NULL);
	i = 0;
	while (s && s[i])
	{
		if (s[i] == '\'')
		{
			int j = i + 1;
			while (s[j] && s[j] != '\'')
				j++;
			res = append_literal(res, s, i + 1, j);
			i = (s[j] ? j + 1 : j);
		}
		else if (s[i] == '"')
		{
			int j = i + 1;
			while (s[j] && s[j] != '"')
				j++;
			{
				char *inner = ft_substr(s, i + 1, j - (i + 1));
				char *exp_inner;
				char *tmp;
				if (!inner)
				{
					free(res);
					return (NULL);
				}
				exp_inner = expand_string(inner, env, exit_status);
				free(inner);
				if (!exp_inner)
				{
					free(res);
					return (NULL);
				}
				tmp = ft_strjoin(res, exp_inner);
				free(res);
				free(exp_inner);
				if (!tmp)
					return (NULL);
				res = tmp;
			}
			i = (s[j] ? j + 1 : j);
		}
		else if (s[i] == '$')
			res = expand_dollar_chunk(res, s, &i, &shell_context);
		else
			res = expand_text_chunk(res, s, &i);
		if (!res)
			return (NULL);
	}
	return (res);
}

