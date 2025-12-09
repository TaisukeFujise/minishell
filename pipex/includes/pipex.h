/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 14:33:06 by tafujise          #+#    #+#             */
/*   Updated: 2025/12/08 07:14:41 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "../libft/libft.h"
# include <sys/types.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <errno.h>
# include <stdio.h>
# include <stdbool.h>

# define SUCCESS 0
# define ERROR -1

typedef struct s_cmd
{
	char	**cmd;
	pid_t	pid;
}	t_cmd;

typedef struct s_ctx
{
	char	*input_file;
	char	*output_file;
	t_cmd	*cmd_set;
	int		cmd_sum;
	char	**envp;
	int		status;
}	t_ctx;

/* parse.c */
int		parse_args(int argc, char **argv, char **envp, t_ctx *ctx);
char	**parse_path(char **envp);
/* pipeline.c */
int		run_pipeline(t_ctx *ctx);
void	exec_child(t_ctx *ctx,
			char **cmd, int input_fd, int output_fd);
void	search_and_exec(t_ctx *ctx, char **cmd);
/* spawn.c */
int		spawn_first_child(t_ctx *ctx, int *pipe_fd, int *prev_read_fd);
int		spawn_middle_child(t_ctx *ctx, int index, int *prev_read_fd,
			int *pipe_fd);
int		spawn_last_child(t_ctx *ctx, int index, int *prev_read_fd);
/* free.c */
void	free_ctx(t_ctx *ctx);
void	free_pathset(char ***path_set);
/* utils.c */
bool	has_slash(char *cmd);
void	exit_code_handle(void);
int		status_to_exitcode(int status);
int		apply_redirect(int input_fd, int output_fd);
void	exec_direct_path(t_ctx *ctx, char *path, char **cmd, char **envp);
/* wait.c */
int		wait_former_pid(t_ctx *ctx, int index);

#endif
