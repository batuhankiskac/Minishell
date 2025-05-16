/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_on_redirs.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/05/16 17:20:57 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
