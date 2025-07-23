/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_expander_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 15:35:00 by bkiskac           #+#    #+#             */
/*   Updated: 2025/07/23 18:05:09 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief      Expands text chunks that do not contain special characters.
 * This function appends a literal segment of a string until it hits a
 * special character ('$', ''', '"').
 * @param res  The current result string.
 * @param s    The source string.
 * @param i    A pointer to the current index in the source string.
 * @return     An updated result string, or NULL on failure.
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
 * @brief      Handles single-quoted strings by treating their content literally.
 * @param res  The current result string.
 * @param s    The source string.
 * @param i    A pointer to the current index in the source string.
 * @return     An updated result string, or NULL on failure.
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
 * @brief      Processes the content within double quotes, expanding only '$'.
 * This is a specialized helper that iterates through a substring,
 * expands variables, and treats single quotes as literal characters.
 * @param content The substring from within the double quotes.
 * @param shell_context Shell context for variable expansion.
 * @return      A new string with the expanded content.
 */
static char	*process_double_quoted_content(const char *content,
		t_shell *shell_context)
{
	char	*result;
	int		i;
	int		last;

	result = ft_strdup("");
	i = 0;
	last = 0;
	while (content[i])
	{
		if (content[i] == '$')
		{
			result = append_literal(result, content, last, i);
			last = i;
			result = append_variable(result, (char *)content, &last,
					shell_context);
			i = last;
		}
		else
			i++;
	}
	result = append_literal(result, content, last, i);
	return (result);
}

/**
 * @brief      Expands the double-quoted content and joins it to the result.
 *
 * This helper takes the raw content from within double quotes, calls a
 * function to expand variables within it, joins the expanded result to the
 * main result string, and handles all intermediate memory deallocations.
 *
 * @param res           The main result string to be appended to.
 * @param content       The raw string extracted from within the double quotes.
 * @param shell_context Shell context for variable expansion.
 * @return              The new, final result string, or NULL on failure.
 */
static char	*expand_and_join_dquote_content(char *res, const char *content,
		t_shell *shell_context)
{
	char	*expanded_content;
	char	*final_res;

	expanded_content = process_double_quoted_content(content, shell_context);
	if (!expanded_content)
	{
		free(res);
		return (NULL);
	}
	final_res = ft_strjoin(res, expanded_content);
	free(res);
	free(expanded_content);
	return (final_res);
}

/**
 * @brief      Handles double-quoted strings by processing their content.
 *
 * It extracts the content within the quotes and passes it to a specialized
 * helper function (`expand_and_join_dquote_content`) to handle the
 * expansion, joining, and memory management.
 *
 * @param res           The current result string.
 * @param s             The source string.
 * @param i             A pointer to the current index in the source string.
 * @param shell_context Shell context for variable expansion.
 * @return              An updated result string, or NULL on failure.
 */
char	*handle_double_quote(char *res, const char *s, int *i,
		t_shell *shell_context)
{
	int		start;
	int		end;
	char	*content;
	char	*final_res;

	start = *i + 1;
	end = start;
	while (s[end] && s[end] != '"')
		end++;
	content = ft_substr(s, start, end - start);
	if (!content)
	{
		free(res);
		return (NULL);
	}
	final_res = expand_and_join_dquote_content(res, content, shell_context);
	free(content);
	if (s[end])
		*i = end + 1;
	else
		*i = end;
	return (final_res);
}
