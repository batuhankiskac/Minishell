/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_flag.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 00:23:05 by bkiskac           #+#    #+#             */
/*   Updated: 2025/05/15 00:26:50 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static volatile sig_atomic_t  g_signal_flag = 0;

void	set_signal_flag(int sig)
{
	g_signal_flag = sig;
}

int	get_signal_flag(void)
{
	return (g_signal_flag);
}

void	reset_signal_flag(void)
{
	g_signal_flag = 0;
}
