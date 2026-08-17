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

static t_status	exec_builtin_in_parent(t_simple_cmd *cmd, t_ctx *ctx);

/*
	execute a command the shell runs itself: a builtin, like cd, or a
	command with no name, which is only redirects and assignments.
	Fork if pipe_in or pipe_out is not a NO_PIPE.
	(It means command is connected by pipe)
	- in pipe
		- fork
		- in child : enter_child, apply_redirects then builtin_cmd
		- in parent: close_pipes and dispose_pid
	- single
		- exec_builtin_in_parent
*/
/*
	Todo left
	- restore_signals ???
*/
t_status	exec_builtin(t_simple_cmd *cmd, t_ctx *ctx, t_stage st)
{
	pid_t	pid;

	if (st.pipe_in == NO_PIPE && st.pipe_out == NO_PIPE)
		return (exec_builtin_in_parent(cmd, ctx));
	pid = fork();
	if (pid < 0)
		return (ST_FAILURE);
	if (pid == 0)
	{
		enter_child(st);
		if (apply_redirects(cmd->redirects, false) != ST_OK)
			exit(EXIT_FAILURE);
		builtin_cmd(cmd->args, ctx);
		exit(ctx->err.exit_code);
	}
	close_pipes(st);
	return (dispose_pid(ctx, st, pid));
}

/*
	The redirects of one command must not outlive it, so the shell
	process saves its stdio and puts it back whatever the builtin did.
	A command with no name applies the redirects and runs nothing.
*/
static t_status	exec_builtin_in_parent(t_simple_cmd *cmd, t_ctx *ctx)
{
	t_status	result;

	result = apply_redirects(cmd->redirects, true);
	if (result == ST_OK)
		result = builtin_cmd(cmd->args, ctx);
	if (undo_redirects(cmd->redirects) != ST_OK)
		return (ST_FATAL);
	return (result);
}

/*
	builtin_cmd dispatch builtin command by the first word of args.
*/
t_status	builtin_cmd(t_word_list *args, t_ctx *ctx)
{
	t_builtin	fn;

	if (args == NULL)
		return (set_exit_code(ctx, ST_OK));
	if (args->wd == NULL || args->wd->str == NULL)
		return (ST_FATAL);
	fn = find_builtin(args->wd->str);
	if (fn == NULL)
		return (ST_FATAL);
	return (set_exit_code(ctx, fn(args->next, ctx)));
}

/*
	find_builtin returns the function of a builtin name,
	and NULL when the name is not a builtin.
	exec_simple uses it to choose the execution path.
*/
t_builtin	find_builtin(char *name)
{
	if (ft_strcmp(name, "cd") == 0)
		return (cd_cmd);
	else if (ft_strcmp(name, "echo") == 0)
		return (echo_cmd);
	else if (ft_strcmp(name, "env") == 0)
		return (env_cmd);
	else if (ft_strcmp(name, "exit") == 0)
		return (exit_cmd);
	else if (ft_strcmp(name, "export") == 0)
		return (export_cmd);
	else if (ft_strcmp(name, "pwd") == 0)
		return (pwd_cmd);
	else if (ft_strcmp(name, "unset") == 0)
		return (unset_cmd);
	else
		return (NULL);
}
