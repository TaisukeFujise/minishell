/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 00:45:59 by tafujise          #+#    #+#             */
/*   Updated: 2026/01/28 23:08:42 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/parser.h"
#include "../../include/execute.h"

/*
	Todo
	execute builtin command in child process.
*/
void	exec_subshell_builtin(t_simple_cmd *cmd, t_ctx *ctx, int pipe_in, int pipe_out)
{
	t_status	result;

	if (apply_redirects(cmd->redirects, ctx) == ST_FATAL)
		exit (result);
}

/*
	Todo
	execute builtin command in parent process
	- reverse redirect
*/
t_status	exec_builtin(t_simple_cmd *cmd, t_ctx *ctx, int pipe_in, int pipe_out)
{
	t_status	result;
	if (apply_redirects(cmd->redirects, ctx) == ST_FATAL)
		return (ST_FATAL);
	return (result);
}
