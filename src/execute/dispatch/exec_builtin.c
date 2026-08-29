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

/*
	Run a builtin, or a command with no name, in this process. The
	redirects of one command must not outlive it, so they are undone
	whatever the builtin did. A pipeline stage is already a process of
	its own, so nothing forks here.
*/
t_status	exec_builtin(t_simple_cmd *cmd, t_ctx *ctx)
{
	t_status	result;

	result = apply_redirects(cmd->redirects, REDIR_RESTORE);
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
