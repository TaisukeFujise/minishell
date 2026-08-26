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
	The process boundary of a pipeline stage. It closes the end of the
	new pipe it does not use, takes the end it reads from as its stdin
	and the one it writes into as its stdout, then runs the node it was
	forked for. The node has this process to itself, so it never forks
	again. It never returns: the status of that node is its own.
*/
static void	stage_child(t_node *node, t_ctx *ctx, int prevfd, int *pipefd)
{
	reset_signals();
	ctx->interactive = false;
	if (pipefd[0] != NO_PIPE)
		close(pipefd[0]);
	if (prevfd != NO_PIPE && move_fd(prevfd, STDIN_FILENO) != ST_OK)
		exit(EXIT_FAILURE);
	if (pipefd[1] != NO_PIPE && move_fd(pipefd[1], STDOUT_FILENO) != ST_OK)
		exit(EXIT_FAILURE);
	execute_internal(node, ctx, EXEC_OWN_PROCESS);
	exit(ctx->err.exit_code);
}

/*
	Every stage of a pipeline runs in its own process, so the coordinator
	forks them itself instead of letting a leaf guess that it is one.
	An endpoint is closed as soon as the stage that needs it has been
	started, so a child inherits at most the end it reads from and the
	pipe it writes into, and it closes the one it does not want by name.
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
		stage_child(*stages, ctx, prevfd, pipefd);
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
