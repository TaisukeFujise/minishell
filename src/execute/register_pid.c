/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   register_pid.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 00:09:40 by tafujise          #+#    #+#             */
/*   Updated: 2026/02/06 18:32:13 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/parser.h"
#include "../../include/execute.h"
/*
	register_pid stack pid on ctx->pids, and increment ctx->npid
	- calloc by (npid + 1) size
	- memcpy pids to new_pids
	- set new_pids to ctx->pids
	- increment ctx->npid
*/
t_status	register_pid(t_ctx *ctx, pid_t pid)
{
	pid_t	*new_pids;

	new_pids = ft_calloc(ctx->npid + 1, sizeof(pid_t));
	if (new_pids == NULL)
		return (ST_FATAL);
	ft_memcpy(new_pids, ctx->pids, sizeof(pid_t) * ctx->npid);
	new_pids[ctx->npid] = pid;
	free(ctx->pids);
	ctx->pids = new_pids;
	ctx->npid++;
	return (ST_OK);
}
