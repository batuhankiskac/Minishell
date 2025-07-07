/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   append_variable.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/07/02 23:25:55 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief      Handles brace variable expansion ${VAR}.
 * @param s    The source string.
 * @param i    A pointer to the current position, which will be advanced.
 * @param env  The environment list to search for the variable.
 * @return     A dynamically allocated string with the variable's value,
 * an empty string if not found, or NULL on allocation failure.
 */
static char	*handle_brace_variable_expansion(char *s, int *i, t_env *env)
{
	char	*key;
	char	*value;
	int		j;

	j = *i + 2;
	while (s[j] && s[j] != '}' && is_env_char(s[j]))
		j++;
	if (s[j] != '}')
	{
		(*i)++;
		return (ft_strdup("$"));
	}
	key = ft_substr(s, *i + 2, j - (*i + 2));
	if (!key)
		return (NULL);
	*i = j + 1;
	value = get_env_value(key, env);
	free(key);
	if (!value)
		return (ft_strdup(""));
	return (value);
}

/**
 * @brief      Reads a variable name after a '$' and returns its expanded value.
 * @param s    The source string.
 * @param i    A pointer to the current position, which will be advanced.
 * @param env  The environment list to search for the variable.
 * @return     A dynamically allocated string with the variable's value,
 * an empty string if not found, or NULL on allocation failure.
 */
static char	*handle_variable_expansion(char *s, int *i, t_env *env)
{
	char	*key;
	char	*value;
	int		j;

	j = *i + 1;
	while (is_env_char(s[j]))
		j++;
	key = ft_substr(s, *i + 1, j - (*i + 1));
	if (!key)
		return (NULL);
	*i = j;
	value = get_env_value(key, env);
	free(key);
	if (!value)
		return (ft_strdup(""));
	return (value);
}

/**
 * @brief      Determines the type of expansion ($?, $VAR, or literal $)
 * and returns the corresponding expanded string value.
 * @param s    The source string being processed.
 * @param i    A pointer to the current position in the source string.
 * @param shell The shell context, containing env and exit_status.
 * @return     A dynamically allocated string with the expanded value,
 * or NULL on allocation failure.
 */
static char	*get_expansion_value(char *s, int *i, t_shell *shell)
{
	if (s[*i + 1] == '?')
	{
		*i += 2;
		return (ft_itoa(shell->exit_status));
	}
	else if (s[*i + 1] == '{')
		return (handle_brace_variable_expansion(s, i, shell->env));
	else if (is_env_char(s[*i + 1]))
		return (handle_variable_expansion(s, i, shell->env));
	else
	{
		(*i)++;
		return (ft_strdup("$"));
	}
}

/**
 * @brief      Expands a variable starting with '$' and appends it to the
 * result string.
 * @param res  The result string to be appended to.
 * @param s    The source string being processed.
 * @param i    A pointer to the current position in the source string.
 * @param shell The shell context for expansion.
 * @return     The new result string with the expanded variable appended,
 * or NULL on failure.
 */
char	*append_variable(char *res, char *s, int *i, t_shell *shell)
{
	char	*val;
	char	*new_res;

	val = get_expansion_value(s, i, shell);
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
