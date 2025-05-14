/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_on_redirs.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 23:38:31 by bkiskac           #+#    #+#             */
/*   Updated: 2025/05/14 23:40:17 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * expand_on_redirs - expand each redirection filename in a command’s redir list
 *
 * @cmd:          pointer to the command whose redirs will be processed
 * @env:          head of the environment variables list
 * @exit_status:  last command’s exit status for `$?` expansion
 *
 * Returns 1 on success, 0 on malloc error.
 */
int	expand_on_redirs(t_command *cmd, t_env *env, int exit_status)
{
	t_redir	*r;
	char	*new;

	r = cmd->redir;
	while (r)
	{
		new = expand_string(r->file, env, exit_status);
		if (!new)
			return (0);
		free(r->file);
		r->file = new;
		r = r->next;
	}
	return (1);
}
