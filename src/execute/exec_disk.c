/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_disk.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 00:49:57 by tafujise          #+#    #+#             */
/*   Updated: 2026/02/03 19:14:08 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/parser.h"

/*
	Todo
	execute disk command(external command).
*/
t_status	exec_disk_command(t_simple_cmd *cmd, t_ctx *ctx, int pipe_in, int pipe_out)
{
	t_status	result;
	// Here!!!
	// apply_assigns_to_tmp_env
	if (apply_redirects(cmd->redirects) == ST_FATAL)
		return (ST_FATAL);
	return (result);
}
