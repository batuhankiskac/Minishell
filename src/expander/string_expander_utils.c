/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_expander_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 15:35:00 by bkiskac           #+#    #+#             */
/*   Updated: 2025/07/02 23:20:41 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Expands text chunks without special characters.
 *
 * @param res The current result string.
 * @param s The source string.
 * @param i Pointer to the current index.
 * @return Updated result string or NULL on failure.
 */
char	*expand_text_chunk(char *res, const char *s, int *i)
{
	int	j;

	j = *i;
	while (s[j] && s[j] != '$' && s[j] != '\'' && s[j] != '"')
		j++;
	res = append_literal(res, s, *i, j);
	*i = j;
	return (res);
}

/**
 * @brief Handles single quote expansion by copying content literally.
 *
 * @param res The current result string.
 * @param s The source string.
 * @param i Pointer to the current index.
 * @return Updated result string or NULL on failure.
 */
char	*handle_single_quote(char *res, const char *s, int *i)
{
	int	j;

	j = *i + 1;
	while (s[j] && s[j] != '\'')
		j++;
	res = append_literal(res, s, *i + 1, j);
	if (s[j])
		*i = j + 1;
	else
		*i = j;
	return (res);
}

/**
 * @brief Processes content inside double quotes.
 *
 * @param inner The inner string content.
 * @param shell_context Shell context for variable expansion.
 * @return Expanded string or NULL on failure.
 */
static char	*expand_inner_content(char *inner, t_shell *shell_context)
{
	char	*expanded;

	expanded = expand_string(inner, shell_context->env,
			shell_context->exit_status);
	free(inner);
	return (expanded);
}

/**
 * @brief Handles double quote expansion with variable expansion inside.
 *
 * @param res The current result string.
 * @param s The source string.
 * @param i Pointer to the current index.
 * @param shell_context Shell context for variable expansion.
 * @return Updated result string or NULL on failure.
 */
char	*handle_double_quote(char *res, const char *s, int *i,
						t_shell *shell_context)
{
	int		j;
	char	*inner;
	char	*expanded;
	char	*tmp;

	j = *i + 1;
	while (s[j] && s[j] != '"')
		j++;
	inner = ft_substr(s, *i + 1, j - (*i + 1));
	if (!inner)
	{
		free(res);
		return (NULL);
	}
	expanded = expand_inner_content(inner, shell_context);
	if (!expanded)
	{
		free(res);
		return (NULL);
	}
	tmp = ft_strjoin(res, expanded);
	free(res);
	free(expanded);
	if (s[j])
		*i = j + 1;
	else
		*i = j;
	return (tmp);
}
