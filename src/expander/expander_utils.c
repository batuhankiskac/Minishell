/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 23:51:09 by bkiskac           #+#    #+#             */
/*   Updated: 2025/05/14 23:51:10 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * append_literal - append s[start..end-1] to res
 */
static char	*append_literal(char *res, const char *s, int start, int end)
{
	char	*tmp;

	tmp = ft_substr(s, start, end - start);
	if (!tmp)
		return (NULL);
	res = ft_strjoin(res, tmp);
	free(tmp);
	return (res);
}

/*
 * append_variable - expand one $-expression and append to res
 */
static char	*append_variable(char *res, const char *s,
		int *i, t_env *env, int exit_status)
{
	int		j;
	char	*key;
	char	*val;

	if (s[*i + 1] == '?')
		val = ft_itoa(exit_status), *i += 2;
	else if (is_env_char(s[*i + 1]))
	{
		j = *i + 1;
		while (is_env_char(s[j]))
			j++;
		key = ft_substr(s, *i + 1, j - (*i + 1));
		val = get_env_value(key, env);
		free(key);
		if (!val)
			val = ft_strdup("");
		*i = j;
	}
	else
		val = ft_strdup("$"), (*i)++;
	if (!val)
		return (NULL);
	res = ft_strjoin(res, val);
	free(val);
	return (res);
}

/*
 * expand_text_chunk - handle a run of non-$ chars
 */
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

/*
 * expand_dollar_chunk - handle a $-prefixed expression
 */
static char	*expand_dollar_chunk(const char *s, int *i,
		char *res, t_shell *shell)
{
	return (append_variable(res, s, i, shell->env, shell->exit_status));
}

/*
 * expand_string - perform all $VAR and $? expansions
 */
char	*expand_string(const char *s, t_env *env, int exit_status)
{
	char		*res;
	int			i;
	t_shell		shell_copy;

	/* pack env & exit_status for expand_dollar_chunk */
	shell_copy.env = env;
	shell_copy.exit_status = exit_status;

	res = safe_malloc(1);
	if (!res)
		return (NULL);
	res[0] = '\0';
	i = 0;
	while (s[i])
	{
		if (s[i] == '$')
			res = expand_dollar_chunk(s, &i, res, &shell_copy);
		else
			res = expand_text_chunk(res, s, &i);
		if (!res)
			return (NULL);
	}
	return (res);
}
