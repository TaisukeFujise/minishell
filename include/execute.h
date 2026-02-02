/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 22:09:51 by tafujise          #+#    #+#             */
/*   Updated: 2026/02/03 00:50:11 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTE_H
# define EXECUTE_H

# define FD_BITMAP_SIZE 32

# include "./parser.h"
# include <sys/types.h>
# include "./hashmap.h"
# include "./minishell.h"
# include <fcntl.h>
# include <errno.h>

typedef struct s_savedfd
{
	int	stdin;
	int	stdout;
}	t_savedfd;

/* init.c */
int			init_ctx(t_ctx *ctx, char **envp);
/* execute.c */
t_status	execute(t_node *node, t_ctx *ctx);
t_status	execute_internal(t_node *node, t_ctx *ctx, int pipe_in, int pipe_out);
/* exec_connection.c */
t_status	exec_connection(t_node *node, t_ctx *ctx, int pipe_in, int pipe_out);
t_status	exec_complete(t_node *node, t_ctx *ctx, int pipe_in, int pipe_out);
t_status	exec_andor(t_node *node, t_ctx *ctx, int pipe_in, int pipe_out);
t_status	exec_pipeline(t_node *node, t_ctx *ctx, int pipe_in, int pipe_out);
/* exec_simple.c */
t_status	exec_simple(t_node *node, t_ctx *ctx, int pipe_in, int pipe_out);
/* exec_subshell.c */
t_status	exec_subshell(t_node *node, t_ctx *ctx, int pipe_in, int pipe_out);
/* exec_null.c */
t_status	exec_null_command(t_simple_cmd *cmd, t_ctx *ctx, int pipe_in, int pipe_out);
/* exec_builtin.c */
void		exec_subshell_builtin(t_simple_cmd *cmd, t_ctx *ctx, int pipe_in, int pipe_out);
t_status	exec_builtin(t_simple_cmd *cmd, t_ctx *ctx, int pipe_in, int pipe_out);
/* exec_disk.c */
t_status	exec_disk_command(t_simple_cmd *cmd, t_ctx *ctx, int pipe_in, int pipe_out);

/* bitmap.c */
t_fd_bitmap	*new_fd_bitmap(int size);
void		close_fd_bitmap(t_fd_bitmap *fd_bitmap);
void		dispose_fd_bitmap(t_fd_bitmap *fd_bitmap);

/* assigns.c */
t_status	apply_assigns_to_vars(t_hashtable *env_table, t_assign *assign);
t_status	apply_assigns_to_tmp_env(t_hashtable *tmp_table, t_assign *assign);

/* apply_redirect.c */
t_status	apply_redirects(t_redirect *redirects, t_ctx *ctx);

/* stdio_backup.c */
t_status	save_stdio(t_savedfd *saved);
t_status	undo_stdio(t_redirect *redirects, t_savedfd saved);

/* pipe_utils.c */
t_status	attach_pipe_to_stdio(t_ctx *ctx, int pipe_in, int pipe_out);
void		close_pipes(int pipe_in, int pipe_out);

/* register_pid.c */
t_status	register_pid(t_ctx *ctx, pid_t pid);

/* wait_pids.c */
t_status	collect_child_result(t_ctx *ctx);

#endif
