/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 00:35:09 by tafujise          #+#    #+#             */
/*   Updated: 2025/12/08 06:48:46 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

bool	has_slash(char *cmd)
{
	int	i;

	i = 0;
	while (cmd[i] != '\0')
	{
		if (cmd[i] == '/')
			return (true);
		i++;
	}
	return (false);
}

void	exit_code_handle(void)
{
	if (errno == EACCES || errno == ENOEXEC)
		exit(126);
	if (errno == ENOENT)
		exit(127);
	exit(1);
}

int	status_to_exitcode(int status)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}

int	apply_redirect(int input_fd, int output_fd)
{
	if (output_fd == -1)
		return (ERROR);
	if (input_fd != -1 && input_fd != STDIN_FILENO)
	{
		if (dup2(input_fd, STDIN_FILENO) != STDIN_FILENO)
			return (perror("dup2(stdin)"), ERROR);
	}
	if (output_fd != -1 && output_fd != STDOUT_FILENO)
	{
		if (dup2(output_fd, STDOUT_FILENO) != STDOUT_FILENO)
			return (perror("dup2(stdout)"), ERROR);
	}
	return (SUCCESS);
}

void	exec_direct_path(t_ctx *ctx, char *path, char **cmd, char **envp)
{
	if (execve(path, cmd, envp) < 0)
	{
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": command not found\n", 2);
		free_ctx(ctx);
		exit_code_handle();
	}
}
