/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 00:45:59 by tafujise          #+#    #+#             */
/*   Updated: 2026/04/19 22:24:00 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/builtin.h"
#include "../../../include/execute.h"
#include "../../../include/minishell.h"
#include "../../../include/parser.h"

void		exec_builtin_in_pipe(t_simple_cmd *cmd, t_ctx *ctx, int pipe_in,
				int pipe_out);
t_status	exec_builtin_in_parent(t_simple_cmd *cmd, t_ctx *ctx);

/*
	execute builtin command, like cd.
	Fork if pipe_in or pipe_out is not a NO_PIPE.
	(It means command is connected by pipe)
	- in pipe
		- fork
		- in child : exec_builtin_command_in_pipe
		- in parent:
			- set ctx->already_forked flag
			- close_pipes(pipe_in, pipe_out)
			- register_pid(ctx, pid)
	- single
		- exec_builtin_in_parent
*/
t_status	exec_builtin(t_simple_cmd *cmd, t_ctx *ctx, int pipe_in,
		int pipe_out)
{
	pid_t	pid;

	if (pipe_in != NO_PIPE || pipe_out != NO_PIPE)
	{
		pid = fork();
		if (pid < 0)
			return (ST_FAILURE);
		else if (pid == 0)
			exec_builtin_in_pipe(cmd, ctx, pipe_in, pipe_out);
		else
		{
			ctx->already_forked = 1;
			close_pipes(pipe_in, pipe_out);
			return (register_pid(ctx, pid));
		}
		return (ST_FATAL);
	}
	else
		return (exec_builtin_in_parent(cmd, ctx));
}

/*
	exec_builtin_in_pipe is called in pipe
	- close_fd_bitmap
	- apply_redirects
	- apply_assigns_to_tmp_env
	- builtin_cmd
*/
/*
	Todo left
	- restore_signals ???
*/
void	exec_builtin_in_pipe(t_simple_cmd *cmd, t_ctx *ctx, int pipe_in,
		int pipe_out)
{
	close_fd_bitmap(ctx->bitmap);
	if (attach_pipe_to_stdio(pipe_in, pipe_out) != ST_OK)
		exit(EXIT_FAILURE);
	close_fd_bitmap(ctx->bitmap);
	pipe_in = NO_PIPE;
	pipe_out = NO_PIPE;
	if (apply_redirects(cmd->redirects) != ST_OK)
		exit(EXIT_FAILURE);
	builtin_cmd(cmd->args, ctx);
	exit(ctx->err.exit_code);
}

/*
	exec_builtin_in_parent
	- save_stdio
	- apply_redirects
	- apply_assigns_to_tmp_env
	- builtin_cmd
	- undo_stdio
*/
t_status	exec_builtin_in_parent(t_simple_cmd *cmd, t_ctx *ctx)
{
	t_status	result;
	t_savedfd	saved;

	if (save_stdio(&saved) != ST_OK)
		return (ST_FAILURE);
	if (apply_redirects(cmd->redirects) != ST_OK)
	{
		result = undo_stdio(saved);
		return (close_savedfd(saved), result);
	}
	result = builtin_cmd(cmd->args, ctx);
	if (result != ST_OK)
	{
		if (undo_stdio(saved) == ST_FATAL)
			return (close_savedfd(saved), ST_FATAL);
		return (close_savedfd(saved), result);
	}
	result = undo_stdio(saved);
	return (close_savedfd(saved), result);
}

/*
	builtin_cmd dispatch builtin command by the first word of args.
*/
t_status	builtin_cmd(t_word_list *args, t_ctx *ctx)
{
	t_builtin	fn;

	if (args == NULL || args->wd == NULL || args->wd->str == NULL)
		return (ST_FATAL);
	fn = find_builtin(args->wd->str);
	if (fn == NULL)
		return (ST_FATAL);
	return (fn(args->next, ctx));
}

/*
	find_builtin returns the function of a builtin name,
	and NULL when the name is not a builtin.
	exec_simple uses it to choose the execution path.
*/
t_builtin	find_builtin(char *name)
{
	static const char		*names[] = {"cd", "echo", "env", "exit", "export",
		"pwd", "unset"};
	static const t_builtin	fns[] = {cd_cmd, echo_cmd, env_cmd, exit_cmd,
		export_cmd, pwd_cmd, unset_cmd};
	size_t					i;

	i = 0;
	while (i < sizeof(fns) / sizeof(fns[0]))
	{
		if (ft_strcmp(name, names[i]) == 0)
			return (fns[i]);
		i++;
	}
	return (NULL);
}
