/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_expander_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 15:35:00 by bkiskac           #+#    #+#             */
/*   Updated: 2025/07/08 11:38:55 by bkiskac          ###   ########.fr       */
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
	int	start;
	int	end;

	start = *i;
	end = *i;
	while (s[end] && s[end] != '$' && s[end] != '\'' && s[end] != '"')
		end++;
	res = append_literal(res, s, start, end);
	*i = end;
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
	int	start;
	int	end;

	start = *i + 1;
	end = start;
	while (s[end] && s[end] != '\'')
		end++;
	res = append_literal(res, s, start, end);
	if (s[end])
		*i = end + 1;
	else
		*i = end;
	return (res);
}

/**
 * @brief Processes content inside double quotes with variable expansion.
 *
 * @param inner The inner string content to expand.
 * @param shell_context Shell context for variable expansion.
 * @return Expanded string or NULL on failure.
 */
static char	*process_quoted_content(char *inner, t_shell *shell_context)
{
	char	*result;

	result = expand_string(inner, shell_context->env,
			shell_context->exit_status);
	free(inner);
	return (result);
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
	int		start;
	int		end;
	char	*inner;
	char	*expanded;
	char	*result;

	start = *i + 1;
	end = start;
	while (s[end] && s[end] != '"')
		end++;
	inner = ft_substr(s, start, end - start);
	if (!inner)
		return (free(res), NULL);
	expanded = process_quoted_content(inner, shell_context);
	if (!expanded)
		return (free(res), NULL);
	result = ft_strjoin(res, expanded);
	free(res);
	free(expanded);
	if (s[end])
		*i = end + 1;
	else
		*i = end;
	return (result);
}
