/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_on_redirs.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/07/07 17:21:12 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Expands the file paths in the redirection list of a command.
 *
 * This function iterates through the redirection list of a command and
 * expands each file path using the `expand_string` function. It updates
 * the `file` field of each redirection node with the expanded value.
 * For heredoc delimiters, special quote handling is applied.
 *
 * @param cmd A pointer to the command structure containing the redirections.
 * @param env A pointer to the environment structure used for variable expansion.
 * @param exit_status The exit status of the last executed command, used in
 * expansions.
 * @return 1 on success, 0 if an error occurs during expansion.
 */
int	expand_on_redirs(t_command *cmd, t_env *env, int exit_status)
{
	t_redir	*r;
	char	*new;

	r = cmd->redir;
	while (r)
	{
		if (r->type == REDIR_HEREDOC)
		{
			/* For heredoc, check if original has quotes */
			if (r->original_file[0] == '\'' &&
				 r->original_file[ft_strlen(r->original_file) - 1] == '\'')
			{
				/* Single quoted delimiter - remove quotes, no expansion */
				new = ft_substr(r->original_file, 1, ft_strlen(r->original_file) - 2);
			}
			else if (r->original_file[0] == '"' &&
				 r->original_file[ft_strlen(r->original_file) - 1] == '"')
			{
				/* Double quoted delimiter - remove quotes and process basic escapes */
				char *temp = ft_substr(r->original_file, 1, ft_strlen(r->original_file) - 2);
				if (!temp)
					return (0);
				/* Simple escape processing for heredoc delimiters: \$ -> $ */
				new = ft_strdup("");
				int i = 0;
				while (temp[i])
				{
					if (temp[i] == '\\' && temp[i + 1] == '$')
					{
						char *old = new;
						new = ft_strjoin(new, "$");
						free(old);
						i += 2;
					}
					else
					{
						char *old = new;
						char single[2] = {temp[i], '\0'};
						new = ft_strjoin(new, single);
						free(old);
						i++;
					}
				}
				free(temp);
			}
			else
			{
				/* Unquoted delimiter - keep literal, no expansion for delimiter matching */
				new = ft_strdup(r->original_file);
			}
		}
		else
		{
			/* Normal redirection - expand as usual */
			new = expand_string(r->file, env, exit_status);
		}
		if (!new)
			return (0);
		free(r->file);
		r->file = new;
		r = r->next;
	}
	return (1);
}
