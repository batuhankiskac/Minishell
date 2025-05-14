/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 00:00:43 by bkiskac           #+#    #+#             */
/*   Updated: 2025/05/15 00:04:05 by bkiskac          ###   ########.fr       */
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
	while (s[j] && s[j] != '$')
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
	shell_context.command = NULL;
	shell_context.tokens = NULL;
	shell_context.line = NULL;
	res = ft_strdup("");
	if (!res)
		return (NULL);
	i = 0;
	while (s && s[i])
	{
		if (s[i] == '$')
			res = expand_dollar_chunk(res, s, &i, &shell_context);
		else
			res = expand_text_chunk(res, s, &i);
		if (!res)
			return (NULL);
	}
	return (res);
}
