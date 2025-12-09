/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   spawn.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 01:36:06 by tafujise          #+#    #+#             */
/*   Updated: 2025/12/08 06:59:09 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

int	spawn_first_child(t_ctx *ctx, int *pipe_fd, int *prev_read_fd)
{
	int	input_fd;

	if (pipe(pipe_fd) < 0)
		return (perror("pipe"), ERROR);
	ctx->cmd_set[0].pid = fork();
	if (ctx->cmd_set[0].pid < 0)
		return (perror("fork"), close(pipe_fd[0]), close(pipe_fd[1]), ERROR);
	if (ctx->cmd_set[0].pid == 0)
	{
		input_fd = open(ctx->input_file, O_RDONLY);
		if (input_fd == -1)
		{
			perror(ctx->input_file);
			input_fd = open("/dev/null", O_RDONLY);
			if (input_fd == -1)
			{
				if (waitpid(ctx->cmd_set[0].pid, NULL, 0) < 0)
					perror("waitpid");
				return (close(pipe_fd[0]), close(pipe_fd[1]), ERROR);
			}
		}
		close(pipe_fd[0]);
		exec_child(ctx, ctx->cmd_set[0].cmd, input_fd, pipe_fd[1]);
	}
	*prev_read_fd = pipe_fd[0];
	return (close(pipe_fd[1]), SUCCESS);
}

int	spawn_middle_child(t_ctx *ctx, int index, int *prev_read_fd, int *pipe_fd)
{
	if (pipe(pipe_fd) < 0)
	{
		perror("pipe");
		if (wait_former_pid(ctx, index) == ERROR)
			perror("waitpid");
		return (close(*prev_read_fd), ERROR);
	}
	ctx->cmd_set[index].pid = fork();
	if (ctx->cmd_set[index].pid < 0)
	{
		perror("fork");
		if (wait_former_pid(ctx, index) < 0)
			perror("waitpid");
		return (close(*prev_read_fd),
			close(pipe_fd[0]), close(pipe_fd[1]), ERROR);
	}
	if (ctx->cmd_set[index].pid == 0)
		exec_child(ctx, ctx->cmd_set[index].cmd, *prev_read_fd, pipe_fd[1]);
	close(*prev_read_fd);
	*prev_read_fd = pipe_fd[0];
	return (close(pipe_fd[1]), SUCCESS);
}

int	spawn_last_child(t_ctx *ctx, int index, int *prev_read_fd)
{
	int	output_fd;

	output_fd = -1;
	ctx->cmd_set[index].pid = fork();
	if (ctx->cmd_set[index].pid < 0)
	{
		perror("fork");
		if (wait_former_pid(ctx, index) == ERROR)
			perror("waitpid");
		return (close(*prev_read_fd), ERROR);
	}
	if (ctx->cmd_set[index].pid == 0)
	{
		output_fd = open(ctx->output_file,
				O_WRONLY | O_TRUNC | O_CREAT, 0644);
		if (output_fd == -1)
		{
			perror(ctx->output_file);
			if (wait_former_pid(ctx, index) == ERROR)
				perror("waitpid");
			return (close(*prev_read_fd), ERROR);
		}
		exec_child(ctx, ctx->cmd_set[index].cmd, *prev_read_fd, output_fd);
	}
	return (close(*prev_read_fd), SUCCESS);
}
