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
#include "../../../include/signal_handle.h"
#include "../../../include/minishell.h"
#include "../../../include/parser.h"

void		run_in_place(t_simple_cmd *cmd, t_ctx *ctx, t_exec_params params);
void		disk_command(char **argv, char **envp, t_ctx *ctx);
bool		has_slash(char *str);
int			report_exec_error(char *name, int reason);

/*
	execute disk command(external command), like ls.
	Fork regardless of whether pipe_in or pipe_out are not a NO_PIPE.
	(It means whether command is connected by pipe or not doesn't matter.)
	- own: this process is only for this command, so take it over
	- otherwise: fork, run it in the child and wait for it
*/
t_status	exec_disk_command(t_simple_cmd *cmd, t_ctx *ctx, bool own)
{
	pid_t			pid;
	t_exec_params	params;

	if (build_exec_params(&params, cmd->args, ctx->tmp_table,
			ctx->env_table) == FAILURE)
		return (ST_FATAL);
	if (own)
		run_in_place(cmd, ctx, params);
	pid = fork();
	if (pid < 0)
		return (free_exec_params(params.argv, params.envp), ST_FAILURE);
	if (pid == 0)
		run_in_place(cmd, ctx, params);
	free_exec_params(params.argv, params.envp);
	return (wait_pid_status(ctx, pid));
}

/*
	Apply the redirects of the command and become it. Never returns.
*/
void	run_in_place(t_simple_cmd *cmd, t_ctx *ctx, t_exec_params params)
{
	reset_signals();
	if (apply_redirects(cmd->redirects, false) != ST_OK)
		exit(EXIT_FAILURE);
	disk_command(params.argv, params.envp, ctx);
	exit(EXIT_FAILURE);
}

/*
	Say why a command did not run, and give the status a shell gives for
	it: 127 when there was nothing to run, 126 when there was something
	but it could not be run. [review D37-07, D37-09]
*/
int	report_exec_error(char *name, int reason)
{
	char	*msg;

	write(STDERR_FILENO, "minishell: ", 11);
	write(STDERR_FILENO, name, ft_strlen(name));
	write(STDERR_FILENO, ": ", 2);
	msg = "command not found";
	if (reason != 0)
		msg = strerror(reason);
	write(STDERR_FILENO, msg, ft_strlen(msg));
	write(STDERR_FILENO, "\n", 1);
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
void	disk_command(char **argv, char **envp, t_ctx *ctx)
{
	char	*path;

	if (argv[0][0] == '\0')
		exit(report_exec_error(argv[0], 0));
	path = extract_path_value(ctx->tmp_table, ctx->env_table);
	if (has_slash(argv[0]) || path == NULL)
	{
		set_underscore(envp, argv[0]);
		execve(argv[0], argv, envp);
		exit(report_exec_error(argv[0], errno));
	}
	exit(report_exec_error(argv[0], search_path(path, argv, envp)));
}

bool	has_slash(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '/')
			return (true);
		i++;
	}
	return (false);
}
