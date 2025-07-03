/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_sequence.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 10:15:00 by bkiskac           #+#    #+#             */
/*   Updated: 2025/07/03 11:24:06 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Frees a command sequence structure and its allocated memory.
 *
 * @param seq A pointer to the command sequence structure to free.
 */
void	free_sequence(t_seq *seq)
{
	int	i;

	if (!seq)
		return ;
	if (seq->commands)
	{
		i = -1;
		while (++i < seq->count)
			free(seq->commands[i]);
		free(seq->commands);
	}
	free(seq);
}

/**
 * @brief Checks if a character is inside quotes in a string.
 *
 * @param str The string to check.
 * @param pos The position in the string.
 * @return 1 if the character is inside quotes, 0 otherwise.
 */
static int	is_inside_quotes(const char *str, int pos)
{
	int		i;
	int		in_single_quote;
	int		in_double_quote;

	i = -1;
	in_single_quote = 0;
	in_double_quote = 0;
	while (++i < pos)
	{
		if (str[i] == '\'' && !in_double_quote)
			in_single_quote = !in_single_quote;
		else if (str[i] == '"' && !in_single_quote)
			in_double_quote = !in_double_quote;
	}
	return (in_single_quote || in_double_quote);
}

/**
 * @brief Creates and initializes a command sequence structure.
 *
 * @param line The input line to analyze.
 * @return A pointer to an initialized command sequence structure,
 * or NULL on failure.
 */
static t_seq	*create_sequence_structure(const char *line)
{
	t_seq	*seq;
	int		i;
	int		count;

	seq = malloc(sizeof(t_seq));
	if (!seq)
		return (NULL);
	i = -1;
	count = 1;
	while (line[++i])
	{
		if (line[i] == ';' && !is_inside_quotes(line, i))
			count++;
	}
	seq->count = count;
	seq->current = 0;
	seq->commands = malloc(sizeof(char *) * seq->count);
	if (!seq->commands)
		return (free(seq), NULL);
	return (seq);
}

/**
 * @brief Parses commands from a line and fills the sequence structure.
 *
 * @param seq The command sequence structure to fill.
 * @param line The input line to parse.
 * @return 0 on success, ERROR on failure.
 */
static int	parse_commands_from_line(t_seq *seq, const char *line)
{
	int	i;
	int	start;
	int	cmd_idx;

	i = -1;
	start = 0;
	cmd_idx = 0;
	while (line[++i])
	{
		if (line[i] == ';' && !is_inside_quotes(line, i))
		{
			seq->commands[cmd_idx] = ft_substr(line, start, i - start);
			if (!seq->commands[cmd_idx])
				return (-1);
			cmd_idx++;
			start = i + 1;
		}
	}
	seq->commands[cmd_idx] = ft_substr(line, start, i - start);
	if (!seq->commands[cmd_idx])
		return (ERROR);
	return (0);
}

/**
 * @brief Splits a line into multiple commands based on semicolons.
 *
 * @param line The input line to split.
 * @return A pointer to a command sequence structure, or NULL on failure.
 */
t_seq	*split_command_line(const char *line)
{
	t_seq	*seq;

	seq = create_sequence_structure(line);
	if (!seq)
		return (NULL);
	if (parse_commands_from_line(seq, line) == ERROR)
		return (free_sequence(seq), NULL);
	return (seq);
}
