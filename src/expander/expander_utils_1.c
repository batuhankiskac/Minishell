/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils_1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/05/30 15:09:29 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Handles the expansion of the exit status variable "$?".
 *
 * This function is called when the input string contains "$?" to expand
 * it to the last exit status of the shell. It increments the index
 * to skip over the "$?" characters and returns a string representation
 * of the exit status.
 *
 * @param i Pointer to the current index in the input string.
 * @param exit_status The last exit status of the shell.
 * @return A dynamically allocated string containing the exit status.
 */
static char	*handle_exit_status_expansion(int *i, int exit_status)
{
	*i += 2;
	return (ft_itoa(exit_status));
}

/**
 * @brief Handles the expansion of environment variables in the input string.
 *
 * This function is called when the input string contains a dollar sign
 * followed by a valid environment variable name. It extracts the variable
 * name, retrieves its value from the environment, and returns it as a
 * dynamically allocated string. If the variable is not found, it returns
 * an empty string.
 *
 * @param s The input string containing the environment variable.
 * @param i Pointer to the current index in the input string.
 * @param env The environment structure containing the variables.
 * @return A dynamically allocated string containing the value of the variable,
 *         or an empty string if not found.
 */
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

/**
 * @brief Handles the expansion of a literal dollar sign "$".
 *
 * This function is called when the input string contains a dollar sign
 * that is not followed by a valid environment variable or exit status.
 * It increments the index to skip over the dollar sign and returns a
 * dynamically allocated string containing just the dollar sign.
 *
 * @param i Pointer to the current index in the input string.
 * @return A dynamically allocated string containing a single dollar sign.
 */
static char	*handle_literal_dollar_expansion(int *i)
{
	(*i)++;
	return (ft_strdup("$"));
}

/**
 * @brief Appends the expanded variable value to the result string.
 *
 * This function takes the current result string, the input string, and
 * the current index. It checks if the next character is a valid environment
 * variable or exit status expansion. It retrieves the corresponding value,
 * appends it to the result string, and returns the new result string.
 *
 * @param res The current result string to which the variable will be appended.
 * @param s The input string containing the variable to expand.
 * @param i Pointer to the current index in the input string.
 * @param env The environment structure containing the variables.
 * @param exit_status The last exit status of the shell.
 * @return A dynamically allocated string containing the updated result,
 *         or NULL if an error occurs (e.g., memory allocation failure).
 */
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
