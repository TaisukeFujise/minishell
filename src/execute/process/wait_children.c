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

#include "../../../include/execute.h"
#include "../../../include/minishell.h"
#include <signal.h>

static int	status_to_exitcode(int status)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}

static char	*signal_name(int sig)
{
	if (sig == SIGQUIT)
		return ("Quit");
	if (sig == SIGTERM)
		return ("Terminated");
	if (sig == SIGKILL)
		return ("Killed");
	if (sig == SIGSEGV)
		return ("Segmentation fault");
	if (sig == SIGABRT)
		return ("Aborted");
	if (sig == SIGFPE)
		return ("Floating point exception");
	if (sig == SIGBUS)
		return ("Bus error");
	return (NULL);
}

/*
	What a shell shows for a command that a signal ended. SIGINT only
	needs a new line, the terminal has already echoed ^C, and SIGPIPE is
	the ordinary end of a reader that stopped early. [dash sprint_status]
	Signals with no name here stay silent: the subject does not allow
	strsignal().
*/
static void	report_signal(int status)
{
	char	*name;

	if (!WIFSIGNALED(status))
		return ;
	if (WTERMSIG(status) == SIGINT)
		return ((void)write(STDERR_FILENO, "\n", 1));
	name = signal_name(WTERMSIG(status));
	if (name == NULL)
		return ;
	write(STDERR_FILENO, name, ft_strlen(name));
	if (WCOREDUMP(status))
		write(STDERR_FILENO, " (core dumped)", 14);
	write(STDERR_FILENO, "\n", 1);
}

/*
	Wait for one child and make its status the status of the shell.
	A signal can interrupt the wait; retry the same pid instead of moving
	on and leaving the child unreaped. [review D37-19]
*/
t_status	wait_pid_status(t_ctx *ctx, pid_t pid)
{
	int	status;
	int	waited;

	waited = waitpid(pid, &status, 0);
	while (waited < 0 && errno == EINTR)
		waited = waitpid(pid, &status, 0);
	if (waited < 0)
		return (ST_FATAL);
	report_signal(status);
	ctx->err.exit_code = status_to_exitcode(status);
	return (ST_OK);
}

/*
	Wait for every stage of a pipeline. They are recorded in the order
	they were started, so the last status left in ctx is the status of
	the last stage, which is the status of the pipeline.
*/
t_status	procs_wait(t_procs *procs, t_ctx *ctx)
{
	t_status	result;
	t_status	status;
	int			i;

	result = ST_OK;
	i = 0;
	while (i < procs->count)
	{
		status = wait_pid_status(ctx, procs->pids[i]);
		if (status != ST_OK)
			result = status;
		i++;
	}
	procs_free(procs);
	return (result);
}
