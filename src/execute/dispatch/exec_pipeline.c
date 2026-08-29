/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/25 19:38:03 by fendo             #+#    #+#             */
/*   Updated: 2026/08/25 19:38:04 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/execute.h"
#include "../../../include/signal_handle.h"
#include "../../../include/minishell.h"
#include "../../../include/parser.h"

/*
	Start one stage. The pipe it writes into is made first, then the fork;
	in the child everything below "pid == 0" runs on a process that exists
	only for this node, so it wires the ends it was given onto stdin and
	stdout, closes the one it does not want by name, and never comes back.
	Nothing has to track a set of inherited fds. [dash evalpipe()]
*/
static pid_t	fork_stage(t_node **stages, t_ctx *ctx, int prevfd, int *pipefd)
{
	pid_t	pid;

	pipefd[0] = NO_PIPE;
	pipefd[1] = NO_PIPE;
	if (stages[1] != NULL && pipe(pipefd) < 0)
		return (-1);
	pid = fork();
	if (pid == 0)
	{
		reset_signals();
		ctx->interactive = false;
		if (pipefd[0] != NO_PIPE)
			close(pipefd[0]);
		if (prevfd != NO_PIPE && move_fd(prevfd, STDIN_FILENO) != ST_OK)
			exit(EXIT_FAILURE);
		if (pipefd[1] != NO_PIPE && move_fd(pipefd[1], STDOUT_FILENO) != ST_OK)
			exit(EXIT_FAILURE);
		execute_internal(*stages, ctx, EXEC_NO_FORK);
		exit(ctx->err.exit_code);
	}
	return (pid);
}

static t_status	start_stages(t_node **stages, t_ctx *ctx, t_procs *procs)
{
	int		pipefd[2];
	int		prevfd;
	pid_t	pid;

	prevfd = NO_PIPE;
	while (*stages != NULL)
	{
		pid = fork_stage(stages, ctx, prevfd, pipefd);
		close_fd(prevfd);
		close_fd(pipefd[1]);
		prevfd = pipefd[0];
		if (pid < 0)
			return (close_fd(prevfd), ctx->err.exit_code = 1, ST_FAILURE);
		if (procs_add(procs, pid) != ST_OK)
			return (close_fd(prevfd), ST_FATAL);
		stages++;
	}
	return (ST_OK);
}

/*
	How many processes a pipeline starts, counted before the first fork
	so that the process set is allocated once. [review PROC-01]
*/
static int	count_stages(t_node *node)
{
	if (node != NULL && node->node_kind == NODE_PIPE)
		return (count_stages(node->left) + count_stages(node->right));
	return (1);
}

/*
	The stages of a pipeline in the order they are written. The parse
	tree nests them to the left; the coordinator wants a flat list, the
	way dash keeps them in the node itself.
*/
static t_node	**collect_stages(t_node *node, t_node **out)
{
	if (node != NULL && node->node_kind == NODE_PIPE)
	{
		out = collect_stages(node->left, out);
		return (collect_stages(node->right, out));
	}
	*out = node;
	return (out + 1);
}

/*
	The set of processes is sized and taken before the first fork, and
	every stage that was started is waited for, even if starting a later
	one failed. [review D37-18, P41-05, P41-07]
*/
t_status	exec_pipeline(t_node *node, t_ctx *ctx)
{
	t_node		**stages;
	t_procs		procs;
	t_status	result;
	int			count;

	count = count_stages(node);
	stages = ft_calloc(count + 1, sizeof(t_node *));
	if (stages == NULL)
		return (ST_FATAL);
	if (!procs_init(&procs, count))
		return (free(stages), ST_FATAL);
	collect_stages(node, stages);
	result = start_stages(stages, ctx, &procs);
	free(stages);
	if (result == ST_OK || procs.count > 0)
		return (procs_wait(&procs, ctx));
	return (procs_free(&procs), result);
}
