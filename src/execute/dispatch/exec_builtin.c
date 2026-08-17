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
		- in child : enter_child then builtin_cmd
		- in parent: adopt_child
	- single
		- exec_builtin_in_parent
*/
/*
	Todo left
	- restore_signals ???
*/
t_status	exec_builtin(t_simple_cmd *cmd, t_ctx *ctx, int pipe_in,
		int pipe_out)
{
	pid_t	pid;

	if (pipe_in == NO_PIPE && pipe_out == NO_PIPE)
		return (exec_builtin_in_parent(cmd, ctx));
	pid = fork();
	if (pid < 0)
		return (ST_FAILURE);
	if (pid == 0)
	{
		enter_child(cmd, ctx, pipe_in, pipe_out);
		set_exit_code(ctx, builtin_cmd(cmd->args, ctx));
		exit(ctx->err.exit_code);
	}
	return (adopt_child(ctx, pid, pipe_in, pipe_out));
}

/*
	The redirects of one command must not outlive it, so the shell
	process saves its stdio and puts it back whatever the builtin did.
	A command with no name applies the redirects and runs nothing.
*/
static t_status	exec_builtin_in_parent(t_simple_cmd *cmd, t_ctx *ctx)
{
	t_status	result;
	t_savedfd	saved;

	if (save_stdio(&saved) != ST_OK)
		return (ST_FAILURE);
	result = apply_redirects(cmd->redirects);
	if (result == ST_OK)
		result = builtin_cmd(cmd->args, ctx);
	if (undo_stdio(saved) == ST_FATAL)
		result = ST_FATAL;
	close_savedfd(saved);
	return (result);
}

/*
	builtin_cmd dispatch builtin command by the first word of args.
*/
t_status	builtin_cmd(t_word_list *args, t_ctx *ctx)
{
	t_builtin	fn;

	if (args == NULL)
		return (ST_OK);
	if (args->wd == NULL || args->wd->str == NULL)
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
