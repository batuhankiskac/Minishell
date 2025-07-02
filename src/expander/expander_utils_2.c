/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megoz <megoz@student.42istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/06/29 12:40:56 by megoz            ###   ########.fr       */
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
	return (append_variable(res, (char *)s, i, shell_context));
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


// void	quote_handler(char *res, const char *s, int i)
// {
// 	if (s[i] == '\'')
// 		single_quote_handler(res, s, i);
// 	else if (s[i] == '"')
// 		double_quote_handler(res, s, i);
// }

// void	single_quote_handler(char *res, const char *s, int i)
// {
// 	int j = i + 1;
// 	while (s[j] && s[j] != '\'')
// 		j++;
// 	res = append_literal(res, s, i + 1, j);
// 	if (s[j])
// 		i = j + 1;
// 	else
// 		i = j;
// }

// void	double_quote_handler_util(char *res, const char *s, int i)
// {
// 	int j;
	
// 	j = i + 1;
// 	while (s[j] && s[j] != '"')
// 		j++;
// }

// void	double_quote_handler(char *res, const char *s, int i)
// {
// 	int j;

// 	j = i + 1;
// 	double_quote_handler_util(res, s, i);
// 	char *inner = ft_substr(s, i + 1, j - (i + 1));
// 	char *exp_inner;
// 	char *tmp;
// 	if (!inner)
// 	{
// 		free(res);
// 		return (NULL);
// 	}
// 	exp_inner = expand_string(inner, env, exit_status);
// 	free(inner);
// 	if (!exp_inner)
// 	{
// 		free(res);
// 		return (NULL);
// 	}
// 	tmp = ft_strjoin(res, exp_inner);
// 	free(res);
// 	free(exp_inner);
// 	if (!tmp)
// 		return (NULL);
// 	res = tmp;
// }

// expand ederken aynı türden quote gelene kadar arguman almaya devam et
// aynısını gorunce flag i sifirla
// flag char olsun, char turunde ' veya " yapılabilir
// tek tırnakta dolar ozelligini kaybediyor
// cift tirnakta dolarlıgını koruyor

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
		// quote_handler(res, s, i); // redirects the single quote or the double
		if (s[i] == '\'')
		{
			int j = i + 1;
			while (s[j] && s[j] != '\'')
				j++;
			res = append_literal(res, s, i + 1, j);
			if (s[j]) // saibeli
				i = j + 1;
			else
				i = j;
			// i = (s[j] ? j + 1 : j);
		}
		// single_quote_handler(res, s, i); // redirects the single quote or the double
		else if (s[i] == '\"')
		{
			int j = i + 1;
			while (s[j] && s[j] != '\"')
				j++;
			{
				char *inner = ft_substr(s, i + 1, j - (i + 1));
				// char *exp_inner;
				char *tmp;
				// char	inner_quote;
				// int		inner_i;
				// int		inner_i_count;

				// inner_i_count = 0;
				// inner_i = 0;
				// inner_quote = '\'';
				if (!inner)
				{
					free(res);
					return (NULL);
				}
				// exp_inner = expand_string(inner, env, exit_status);  // guncellendi
				// free(inner);
				// if (!exp_inner)
				// {
				// 	free(res);
				// 	return (NULL);
				// }
				// printf("inner i suan: %d\n", inner_i);
				// if (inner[inner_i] = inner_quote)
				// {
				// 	inner_i_count++;
				// }
				tmp = ft_strjoin(res, inner);
				free(res);
				// free(exp_inner);
				if (!tmp)
					return (NULL);
				res = tmp;
			}
			// i = (s[j] ? j + 1 : j); guncellendi
			if (s[j])
				i = j + 1;
			else
				i = j;
		}
		// double_quote_handler(res, s, i); // redirects the single quote or the double
		else if (s[i] == '$')
			res = expand_dollar_chunk(res, s, &i, &shell_context);
		else
			res = expand_text_chunk(res, s, &i);
		if (!res)
			return (NULL);
	}
	return (res);
}
