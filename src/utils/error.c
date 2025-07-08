/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 15:08:43 by bkiskac           #+#    #+#             */
/*   Updated: 2025/07/08 15:08:49 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Prints a standardized error message to STDERR.
 * @param cmd The command that caused the error (e.g., "cd").
 * @param arg The argument related to the error (can be NULL).
 * @param msg The specific error message.
 * @param err_code The error code to return.
 * @return Returns the specified error code.
 */
int	print_error(char *cmd, char *arg, char *msg, int err_code)
{
	ft_printf(2, "minishell: ");
	if (cmd)
		ft_printf(2, "%s: ", cmd);
	if (arg)
		ft_printf(2, "%s: ", arg);
	ft_printf(2, "%s\n", msg);
	return (err_code);
}

/**
 * @brief Prints error message and returns NULL pointer.
 * @param cmd The command that caused the error.
 * @param arg The argument related to the error (can be NULL).
 * @param msg The specific error message.
 * @return Always returns NULL.
 */
void	*print_error_null(char *cmd, char *arg, char *msg)
{
	print_error(cmd, arg, msg, 0);
	return (NULL);
}
