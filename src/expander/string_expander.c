/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_expander.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/07/02 23:20:41 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Appends a literal substring from `s` to `res`.
 *
 * This function extracts a substring from `s` starting at `start` and ending
 * at `end`, then appends it to `res`. If the substring is empty, it returns
 * `res` unchanged. It handles memory allocation and deallocation for the
 * substring and the new result string.
 *
 * @param res The current result string to which the substring will be appended.
 * @param s The source string from which the substring is extracted.
 * @param start The starting index of the substring in `s`.
 * @param end The ending index of the substring in `s`.
 * @return A new string with the appended substring, or NULL on failure.
 */
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

/**
 * @brief Expands a string by processing quotes and environment variables.
 *
 * This function takes an input string `s` and expands it by handling single
 * quotes, double quotes, and dollar sign expansions. It constructs a new
 * string `res` that contains the expanded result.
 *
 * @param s The input string to expand.
 * @param env The environment structure containing variables.
 * @param exit_status The last exit status of the shell.
 * @return A dynamically allocated string containing the expanded result,
 *         or NULL on failure.
 */
char	*expand_string(const char *s, t_env *env, int exit_status)
{
	t_shell	shell_context;
	char	*res;
	int		i;

	shell_context.env = env;
	shell_context.exit_status = exit_status;
	res = ft_strdup("");
	if (!res)
		return (NULL);
	i = 0;
	while (s && s[i])
	{
		if (s[i] == '\'')
			res = handle_single_quote(res, s, &i);
		else if (s[i] == '"')
			res = handle_double_quote(res, s, &i, &shell_context);
		else if (s[i] == '$')
			res = append_variable(res, (char *)s, &i, &shell_context);
		else
			res = expand_text_chunk(res, s, &i);
		if (!res)
			return (NULL);
	}
	return (res);
}
