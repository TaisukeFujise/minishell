/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_null.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 00:51:30 by tafujise          #+#    #+#             */
/*   Updated: 2026/01/28 23:07:23 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/parser.h"

/*
	Todo
	execute null command, meaning no args command.
*/
t_status	exec_null_command(t_redirect *redirects, t_ctx *ctx, int pipe_in, int pipe_out)
{
	t_status	result;
	if (apply_redirects(redirects, ctx) == ST_FATAL)
		return (ST_FATAL);
	return (result);
}
