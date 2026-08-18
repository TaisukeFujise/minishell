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
#include "../../../include/minishell.h"
#include "../../../include/parser.h"

void		disk_command(char **argv, char **envp, t_ctx *ctx);
bool		has_slash(char *str);
int			run_path_search_command(char *path_value, char **argv, char **envp);

/*
	execute disk command(external command), like ls.
	Fork regardless of whether pipe_in or pipe_out are not a NO_PIPE.
	(It means whether command is connected by pipe or not doesn't matter.)
	- fork
	- in child : enter_child then disk_command
	- in parent: adopt_child
*/
/*
	Todo left
	- restore signals
*/
t_status	exec_disk_command(t_simple_cmd *cmd, t_ctx *ctx, int pipe_in,
		int pipe_out)
{
	pid_t			pid;
	t_exec_params	params;

	if (build_exec_params(&params, cmd->args, ctx->tmp_table,
			ctx->env_table) == FAILURE)
		return (ST_FATAL);
	pid = fork();
	if (pid < 0)
		return (free_exec_params(params.argv, params.envp), ST_FAILURE);
	if (pid == 0)
	{
		enter_child(ctx, pipe_in, pipe_out);
		if (apply_redirects(cmd->redirects) != ST_OK)
			exit(EXIT_FAILURE);
		disk_command(params.argv, params.envp, ctx);
		exit(EXIT_FAILURE);
	}
	free_exec_params(params.argv, params.envp);
	close_pipes(pipe_in, pipe_out);
	return (register_pid(ctx, pid));
}

/*
	Todo
	- disk_command find disk command and execute it.
	- It returns error, like "command not found", if there is no command.
	2.9.1 Simple command
	> Command Search and Execution
*/
void	disk_command(char **argv, char **envp, t_ctx *ctx)
{
	char	*path_value;
	int		exit_code;

	if (has_slash(argv[0]))
	{
		execve(argv[0], argv, envp);
		if (errno == ENOENT)
			exit(127);
		exit(126);
	}
	else
	{
		path_value = extract_path_value(ctx->tmp_table, ctx->env_table);
		if (path_value == NULL)
			exit(1);
		exit_code = run_path_search_command(path_value, argv, envp);
		exit(exit_code);
	}
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

/*
	127 means errno ENOENT("command not found")
*/
int	run_path_search_command(char *path_value, char **argv, char **envp)
{
	char	*dir;
	char	*pathname;
	int		exit_code;

	exit_code = 127;
	while (*path_value)
	{
		dir = extract_path_entry(path_value);
		if (dir == NULL)
			return (1);
		pathname = ft_strjoin(dir, argv[0]);
		if (pathname == NULL)
			return (free(dir), 1);
		execve(pathname, argv, envp);
		if (errno != ENOENT)
			exit_code = 126;
		path_value += ft_strlen(dir);
		if (*path_value == ':')
			path_value++;
	}
	return (exit_code);
}
