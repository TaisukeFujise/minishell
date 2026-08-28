/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_status.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/28 18:20:00 by fendo             #+#    #+#             */
/*   Updated: 2026/08/28 18:20:00 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/execute.h"
#include "../../../include/minishell.h"

/*
	The numeric status of a command comes from the command: from the
	builtin that ran, or from waiting for the child. This settles the
	status of what happens around it instead: an expansion, a fork or a
	redirect the shell could not carry out is a plain failure, and an
	internal failure that chose no code must not read as success.
	[review D37-12, D37-17]
*/
t_status	set_exit_code(t_ctx *ctx, t_status status)
{
	if (status == ST_OK)
		ctx->err.exit_code = 0;
	else if (status == ST_FAILURE)
		ctx->err.exit_code = 1;
	else if (status == ST_FATAL && ctx->err.exit_code == 0)
		ctx->err.exit_code = 1;
	return (status);
}
