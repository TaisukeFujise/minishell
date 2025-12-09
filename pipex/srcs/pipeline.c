/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 15:44:10 by tafujise          #+#    #+#             */
/*   Updated: 2025/12/08 07:12:23 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

int	run_pipeline(t_ctx *ctx)
{
	int	pipe_fd[2];
	int	prev_read_fd;
	int	index;

	index = 0;
	prev_read_fd = -1;
	while (index < ctx->cmd_sum)
	{
		if (index == 0)
			if (spawn_first_child(ctx, pipe_fd, &prev_read_fd) == ERROR)
				return (1);
		if (0 < index && index < ctx->cmd_sum - 1)
			if (spawn_middle_child(ctx, index, &prev_read_fd, pipe_fd) == ERROR)
				return (1);
		if (index == ctx->cmd_sum - 1)
			if (spawn_last_child(ctx, ctx->cmd_sum -1, &prev_read_fd) == ERROR)
				return (1);
		index++;
	}
	if (wait_former_pid(ctx, ctx->cmd_sum - 1) == ERROR)
		return (1);
	if (waitpid(ctx->cmd_set[ctx->cmd_sum - 1].pid, &ctx->status, 0) < 0)
		return (1);
	return (status_to_exitcode(ctx->status));
}

void	exec_child(t_ctx *ctx,
								char **cmd, int input_fd, int output_fd)
{
	if (apply_redirect(input_fd, output_fd) == ERROR)
	{
		close(input_fd);
		close(output_fd);
		free_ctx(ctx);
		exit(1);
	}
	close(input_fd);
	close(output_fd);
	if (cmd[0] == NULL)
	{
		ft_putstr_fd("command not found\n", 2);
		free_ctx(ctx);
		exit(127);
	}
	search_and_exec(ctx, cmd);
}

char	*create_path(char *raw_path, char *cmd_top)
{
	char	*path_base;
	char	*path;

	path_base = ft_strjoin(raw_path, "/");
	if (path_base == NULL)
		return (NULL);
	path = ft_strjoin(path_base, cmd_top);
	free(path_base);
	return (path);
}

void	search_and_exec(t_ctx *ctx, char **cmd)
{
	int		i;
	char	**path_set;
	char	*path;

	if (has_slash(cmd[0]) == true)
		exec_direct_path(ctx, cmd[0], cmd, ctx->envp);
	path_set = parse_path(ctx->envp);
	if (path_set == NULL)
		exit(1);
	i = 0;
	while (path_set[i] != NULL)
	{
		path = create_path(path_set[i], cmd[0]);
		free(path_set[i]);
		if (path == NULL)
		{
			free_pathset(&path_set);
			exit(1);
		}
		execve(path, cmd, ctx->envp);
		free(path);
		i++;
	}
	ft_putstr_fd(cmd[0], 2);
	ft_putstr_fd(": command not found\n", 2);
	free_ctx(ctx);
	exit_code_handle();
}
