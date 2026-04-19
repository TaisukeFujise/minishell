/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait_children.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 23:52:55 by tafujise          #+#    #+#             */
/*   Updated: 2026/04/19 20:16:36 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int			status_to_exitcode(int status);
void		reset_ctx_pid(t_ctx *ctx);

/*
	Todo
	- Wait the arrays of pids
	- Convert last pid's status to exit code and store in ctx->exit_code
	- Clean-up ctx's the arrays of pids.
*/
t_status	collect_child_result(t_ctx *ctx)
{
	int	i;
	int	status;

	i = 0;
	status = 0;
	if (ctx->pids == NULL || ctx->npid < 1)
		return (ST_FATAL);
	while (i < ctx->npid)
	{
		if (waitpid(ctx->pids[i], &status, 0) < 0)
		{
			if (errno != EINTR)
				return (ST_FATAL);
			ctx->err.exit_code = 130;
			continue ;
		}
		i++;
	}
	if (ctx->err.exit_code != 130)
		ctx->err.exit_code = status_to_exitcode(status);
	reset_ctx_pid(ctx);
	return (ST_OK);
}

int	status_to_exitcode(int status)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}

void	reset_ctx_pid(t_ctx *ctx)
{
	free(ctx->pids);
	ctx->pids = NULL;
	ctx->npid = 0;
}
