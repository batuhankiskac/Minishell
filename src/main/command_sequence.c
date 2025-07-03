/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_sequence.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 10:15:00 by bkiskac           #+#    #+#             */
/*   Updated: 2025/07/03 11:42:56 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Frees the memory allocated for a command sequence (`t_seq`) structure.
 *
 * This function safely deallocates the `t_seq` structure and all the
 * command strings it contains. It first iterates through the `commands` array,
 * freeing each individual command string. It then frees the `commands` array
 * itself, and finally, the `t_seq` structure.
 *
 * @param seq A pointer to the `t_seq` structure to be freed. If `seq` is NULL,
 *            the function does nothing.
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
 * @brief Checks if a character at a given position in a string is enclosed
 *        within single or double quotes.
 *
 * This function iterates through the string up to the specified position `pos`,
 * keeping track of the quoting state. It correctly handles nested and
 * alternating
 * quotes. For example, in `"'hello'"`, the single quotes are inside double
 * quotes. This is crucial for the parser to avoid splitting commands based on
 * semicolons that are part of a quoted string literal.
 *
 * @param str The input string to check.
 * @param pos The zero-based index of the character to check.
 * @return Returns 1 if the character at `pos` is inside any type of quotes,
 *         and 0 otherwise.
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
 * @brief Allocates and initializes a `t_seq` structure for storing a sequence
 *        of commands.
 *
 * This function first allocates memory for the `t_seq` structure itself. It then
 * scans the input `line` to count how many separate commands exist, based on
 * semicolons that are not inside quotes. Finally, it allocates the `commands`
 * array within the `t_seq` structure to hold the individual command strings.
 *
 * @param line The complete command line string containing one or more commands
 *             separated by semicolons.
 * @return Returns a pointer to the newly allocated and initialized `t_seq`
 *         structure. If any memory allocation fails, it returns NULL.
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
 * @brief Parses the input line and populates the `commands` array in the `t_seq`
 *        structure.
 *
 * This function iterates through the input `line`, identifying command
 * boundaries
 * marked by semicolons that are not inside quotes. For each command found, it
 * extracts the substring and stores it in the `seq->commands` array. The `start`
 * variable tracks the beginning of the current command being parsed.
 *
 * @param seq A pointer to the `t_seq` structure where the parsed commands
 * 			  will be stored.
 * @param line The complete command line string to be parsed.
 * @return Returns 0 on successful parsing. If a memory allocation for a command
 *         substring fails, it returns `ERROR` (-1).
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
 * @brief Splits a complete command line into a sequence of individual commands.
 *
 * This is the main entry point for parsing a line that may contain multiple
 * commands separated by semicolons. It orchestrates the creation of the `t_seq`
 * structure and the parsing of the command strings.
 *
 * @param line The raw command line input from the user.
 * @return Returns a pointer to a `t_seq` structure containing the parsed
 *         commands. If memory allocation or parsing fails, it cleans up any
 *         partially allocated memory and returns NULL.
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
