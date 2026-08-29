/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_disk.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 00:49:57 by tafujise          #+#    #+#             */
/*   Updated: 2026/04/19 22:24:13 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/execute.h"
#include <sys/stat.h>
#include "../../../include/signal_handle.h"
#include "../../../include/minishell.h"
#include "../../../include/parser.h"

/*
	Say why a command did not run, and give the status a shell gives for
	it: 127 when there was nothing to run, 126 when there was something
	but it could not be run. [review D37-07, D37-09]
*/
static int	report_exec_error(char *name, int reason)
{
	struct stat	info;
	char		*msg;

	msg = "command not found";
	if (reason == EACCES && stat(name, &info) == 0 && S_ISDIR(info.st_mode))
		reason = EISDIR;
	if (reason != 0)
		msg = strerror(reason);
	print_error(name, msg);
	if (reason == 0 || reason == ENOENT || reason == ENOTDIR)
		return (127);
	return (126);
}

/*
	Become the command. A name with a slash is the pathname itself, and
	so is any name when PATH is unset. Never returns.
	2.9.1 Simple command
	> Command Search and Execution
*/
static void	disk_command(char **argv, char **envp, t_ctx *ctx)
{
	char	*path;

	if (argv[0][0] == '\0')
		exit(report_exec_error(argv[0], 0));
	path = env_lookup(ctx->tmp_table, ctx->env_table, "PATH");
	if (has_slash(argv[0]) || path == NULL)
	{
		set_underscore(envp, argv[0]);
		execve(argv[0], argv, envp);
		exit(report_exec_error(argv[0], errno));
	}
	exit(report_exec_error(argv[0], search_path(path, argv, envp)));
}

/*
	Apply the redirects of the command and become it. Never returns.
*/
static void	run_in_place(t_simple_cmd *cmd, t_ctx *ctx, t_exec_params params)
{
	reset_signals();
	if (apply_redirects(cmd->redirects, REDIR_KEEP) != ST_OK)
		exit(EXIT_FAILURE);
	disk_command(params.argv, params.envp, ctx);
	exit(EXIT_FAILURE);
}

/*
	execute disk command(external command), like ls.
	- EXEC_NO_FORK: this process is only for this command, so take
	  it over
	- EXEC_MAY_FORK: fork, run it in the child and wait for it
*/
t_status	exec_disk_command(t_simple_cmd *cmd, t_ctx *ctx, t_exec_mode mode)
{
	pid_t			pid;
	t_exec_params	params;

	if (build_exec_params(&params, cmd->args, ctx->tmp_table,
			ctx->env_table) == FAILURE)
		return (ST_FATAL);
	if (mode == EXEC_NO_FORK)
		run_in_place(cmd, ctx, params);
	pid = fork();
	if (pid < 0)
		return (free_exec_params(params.argv, params.envp), ST_FAILURE);
	if (pid == 0)
		run_in_place(cmd, ctx, params);
	free_exec_params(params.argv, params.envp);
	return (wait_pid_status(ctx, pid));
}
