/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 00:00:00 by codex             #+#    #+#             */
/*   Updated: 2026/02/17 00:00:00 by codex            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "subshell_test.h"

static t_node	new_subshell_node(void)
{
	t_node	node;

	ft_bzero(&node, sizeof(t_node));
	node.node_kind = NODE_SUBSHELL;
	return (node);
}

static int	test_basic_subshell(void)
{
	t_ctx		ctx;
	t_node		node;
	t_status	status;

	ft_bzero(&ctx, sizeof(t_ctx));
	node = new_subshell_node();
	status = exec_subshell(&node, &ctx, NO_PIPE, NO_PIPE);
	if (status != ST_OK)
		return (printf("[NG] basic subshell: status=%d\n", status), 1);
	if (ctx.already_forked != 1 || ctx.npid != 1)
		return (printf("[NG] basic subshell: already_forked=%d npid=%d\n",
				ctx.already_forked, ctx.npid), 1);
	if (collect_child_result(&ctx) != ST_OK)
		return (printf("[NG] basic subshell: collect failed\n"), 1);
	if (ctx.err.exit_code != 41)
		return (printf("[NG] basic subshell: exit_code=%d expected=41\n",
				ctx.err.exit_code), 1);
	if (ctx.pids != NULL || ctx.npid != 0)
		return (printf("[NG] basic subshell: pid reset failed\n"), 1);
	printf("[OK] basic subshell\n");
	return (0);
}

static int	test_close_pipes_in_parent(void)
{
	t_ctx		ctx;
	t_node		node;
	t_status	status;
	int			in_pipe[2];
	int			out_pipe[2];
	int			is_ng;

	ft_bzero(&ctx, sizeof(t_ctx));
	node = new_subshell_node();
	is_ng = 0;
	if (pipe(in_pipe) < 0 || pipe(out_pipe) < 0)
		return (printf("[NG] close pipes: pipe() failed\n"), 1);
	status = exec_subshell(&node, &ctx, in_pipe[0], out_pipe[1]);
	if (status != ST_OK)
		is_ng = 1;
	if (fcntl(in_pipe[0], F_GETFD) != -1 || errno != EBADF)
		is_ng = 1;
	if (fcntl(out_pipe[1], F_GETFD) != -1 || errno != EBADF)
		is_ng = 1;
	close(in_pipe[1]);
	close(out_pipe[0]);
	if (collect_child_result(&ctx) != ST_OK)
		is_ng = 1;
	if (ctx.err.exit_code != 41)
		is_ng = 1;
	if (is_ng)
		return (printf("[NG] close pipes in parent\n"), 1);
	printf("[OK] close pipes in parent\n");
	return (0);
}

/*
	Executed test cases
	1. basic subshell
		- run exec_subshell with NO_PIPE
		- expect: register child pid and collect exit_code=41
	2. close pipes in parent
		- run exec_subshell with explicit pipe_in/pipe_out
		- expect: parent closes passed fds, collect exit_code=41
*/
int	main(void)
{
	int	ng_count;

	ng_count = 0;
	ng_count += test_basic_subshell();
	ng_count += test_close_pipes_in_parent();
	if (ng_count == 0)
		return (0);
	return (1);
}
