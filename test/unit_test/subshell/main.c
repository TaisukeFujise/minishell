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
	status = exec_subshell(&node, &ctx, false);
	if (status != ST_OK)
		return (printf("[NG] basic subshell: status=%d\n", status), 1);
	if (ctx.err.exit_code != 41)
		return (printf("[NG] basic subshell: exit_code=%d expected=41\n",
				ctx.err.exit_code), 1);
	printf("[OK] basic subshell\n");
	return (0);
}

/*
	own says the process is already dedicated to this node, so the body
	runs here instead of in a new process.
*/
static int	test_own_runs_in_place(void)
{
	t_ctx	ctx;
	t_node	node;

	ft_bzero(&ctx, sizeof(t_ctx));
	node = new_subshell_node();
	g_body_pid = 0;
	if (exec_subshell(&node, &ctx, true) != ST_OK)
		return (printf("[NG] own subshell: status\n"), 1);
	if (g_body_pid != getpid())
		return (printf("[NG] own subshell: body ran in another process\n"), 1);
	if (ctx.err.exit_code != 41)
		return (printf("[NG] own subshell: exit_code=%d\n",
				ctx.err.exit_code), 1);
	printf("[OK] own subshell runs in place\n");
	return (0);
}

/*
	Executed test cases
	1. basic subshell
		- run exec_subshell with NO_PIPE
		- expect: register child pid and collect exit_code=41
	2. own subshell runs in place
		- run exec_subshell with own
		- expect: the body runs in this process, exit_code=41
*/
int	main(void)
{
	int	ng_count;

	ng_count = 0;
	ng_count += test_basic_subshell();
	ng_count += test_own_runs_in_place();
	if (ng_count == 0)
		return (0);
	return (1);
}
