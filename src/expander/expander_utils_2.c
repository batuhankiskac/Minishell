/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/05/30 15:14:23 by bkiskac          ###   ########.fr       */
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
 * @brief Appends a variable expansion to the result string.
 *
 * This function handles the expansion of a variable in the input string `s`
 * at the current index `*i`. It retrieves the variable's value from the
 * environment and appends it to `res`. If the variable is not found, it
 * appends an empty string.
 *
 * @param res The current result string to which the variable value will be appended.
 * @param s The source string containing the variable to expand.
 * @param i Pointer to the current index in `s` where the variable starts.
 * @param env The environment structure containing the variables.
 * @param exit_status The last exit status of the shell.
 * @return A new string with the appended variable value, or NULL on failure.
 */
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

/**
 * @brief Expands a dollar sign followed by a variable or exit status.
 *
 * This function handles the expansion of a dollar sign in the input string `s`
 * at the current index `*i`. It retrieves the value of the variable or exit
 * status and appends it to `res`.
 *
 * @param res The current result string to which the variable value will be appended.
 * @param s The source string containing the dollar sign and variable.
 * @param i Pointer to the current index in `s` where the dollar sign is located.
 * @param shell_context The shell context containing environment and exit status.
 * @return A new string with the appended variable value, or NULL on failure.
 */
static char	*expand_dollar_chunk(char *res, const char *s, int *i,
								t_shell *shell_context)
{
	return (append_variable(res, s, i, shell_context->env,
			shell_context->exit_status));
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
