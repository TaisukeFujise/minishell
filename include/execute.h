/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 22:09:51 by tafujise          #+#    #+#             */
/*   Updated: 2026/02/15 03:05:43 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTE_H
# define EXECUTE_H

# define FD_BITMAP_SIZE 32

# include "./hashmap.h"
# include "./minishell.h"
# include "./parser.h"
# include <errno.h>
# include <fcntl.h>
# include <sys/types.h>

typedef struct s_savedfd
{
	int		stdin;
	int		stdout;
}			t_savedfd;

typedef enum s_tabletype
{
	TMP,
	VARS,
}			t_tabletype;

typedef struct s_exec_params
{
	char	**argv;
	char	**envp;
}			t_exec_params;

/* init.c */
int			init_ctx(t_ctx *ctx, char **envp);
/* execute.c */
t_status	execute(t_node *node, t_ctx *ctx);
t_status	execute_internal(t_node *node, t_ctx *ctx, int pipe_in,
				int pipe_out);

// <dispatch>
/* exec_builtin.c */
t_status	exec_builtin(t_simple_cmd *cmd, t_ctx *ctx, int pipe_in,
				int pipe_out);
/* exec_connection.c */
t_status	exec_connection(t_node *node, t_ctx *ctx, int pipe_in,
				int pipe_out);
t_status	exec_complete(t_node *node, t_ctx *ctx, int pipe_in, int pipe_out);
t_status	exec_andor(t_node *node, t_ctx *ctx, int pipe_in, int pipe_out);
t_status	exec_pipeline(t_node *node, t_ctx *ctx, int pipe_in, int pipe_out);
/* exec_disk.c */
t_status	exec_disk_command(t_simple_cmd *cmd, t_ctx *ctx, int pipe_in,
				int pipe_out);
/* exec_null.c */
t_status	exec_null_command(t_simple_cmd *cmd, t_ctx *ctx, int pipe_in,
				int pipe_out);
/* exec_simple.c */
t_status	exec_simple(t_node *node, t_ctx *ctx, int pipe_in, int pipe_out);
/* exec_subshell.c */
t_status	exec_subshell(t_node *node, t_ctx *ctx, int pipe_in, int pipe_out);

// <expansion>
/* assigns.c */
t_status	apply_assigns(t_hashtable *table, t_assign *assign,
				t_tabletype type);
/* expand.c */

// <process>
/* fd_bitmap.c */
t_fd_bitmap	*new_fd_bitmap(int size);
void		close_fd_bitmap(t_fd_bitmap *fd_bitmap);
void		dispose_fd_bitmap(t_fd_bitmap *fd_bitmap);
/* pipe_utils.c */
t_status	attach_pipe_to_stdio(int pipe_in, int pipe_out);
void		close_pipes(int pipe_in, int pipe_out);
/* register_pid.c */
t_status	register_pid(t_ctx *ctx, pid_t pid);
/* wait_children.c */
t_status	collect_child_result(t_ctx *ctx);

// <redirect>
/* apply_redirect.c */
t_status	apply_redirects(t_redirect *redirects);
/* heredoc_tmpfile.c */
char		*create_tmp_filename(void);
/* stdio_guard.c */
t_status	save_stdio(t_savedfd *saved);
void		close_savedfd(t_savedfd saved);
t_status	undo_stdio(t_savedfd saved);

// <utils>
/* args_utils.c */
int			count_args(t_word_list *args);
int			build_exec_params(t_exec_params *exec_params, t_word_list *args,
				t_hashtable *tmp_table, t_hashtable *env_table);
void		free_exec_params(char **argv, char **envp);
/* path_utils.c */
char		*extract_path_value(t_hashtable *tmp_table, t_hashtable *env_table);
char		*extract_path_entry(char *path_value);

#endif
