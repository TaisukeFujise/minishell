/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 22:09:51 by tafujise          #+#    #+#             */
/*   Updated: 2026/02/16 02:52:57 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTE_H
# define EXECUTE_H

# define FD_BITMAP_SIZE 32
/*
	t_redirect.saved holds the backup of its io number while the redirect
	is applied: 0 when there is none, FD_WAS_CLOSED when the io number was
	closed before. A backup never lands on fd 0, it is above every io
	number of the command, so the zeroed AST node means "none".
*/
# define FD_WAS_CLOSED -2

# include "./hashmap.h"
# include "./minishell.h"
# include "./parser.h"
# include <errno.h>
# include <fcntl.h>
# include <sys/types.h>

/*
	What one step of the execution holds. It is passed by value, so what
	a stage sets stays inside the subtree it runs.
	- in/out: the fds to become stdin and stdout, NO_PIPE when there are
	  none. The pipeline that made them owns them.
	- close: the fds a forked child must not keep, the read ends of the
	  pipelines around it.
*/
typedef struct s_stage
{
	int			in;
	int			out;
	t_fd_bitmap	*close;
}				t_stage;

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
t_status	execute_internal(t_node *node, t_ctx *ctx, t_stage st);

// <dispatch>
/* exec_builtin.c */
t_status	exec_builtin(t_simple_cmd *cmd, t_ctx *ctx, t_stage st);
/* exec_connection.c */
t_status	exec_connection(t_node *node, t_ctx *ctx, t_stage st);
t_status	exec_complete(t_node *node, t_ctx *ctx, t_stage st);
t_status	exec_andor(t_node *node, t_ctx *ctx, t_stage st);
t_status	exec_pipeline(t_node *node, t_ctx *ctx, t_stage st);
/* exec_disk.c */
t_status	exec_disk_command(t_simple_cmd *cmd, t_ctx *ctx, t_stage st);
/* exec_simple.c */
t_status	exec_simple(t_node *node, t_ctx *ctx, t_stage st);
t_status	set_exit_code(t_ctx *ctx, t_status status);
/* exec_subshell.c */
t_status	exec_subshell(t_node *node, t_ctx *ctx, t_stage st);

// <expansion>
/* assigns.c */
t_status	apply_assign(t_assign *assign, t_hashtable *table, t_ctx *ctx,
				t_tabletype type);
/* expand.c */

// <process>
/* fd_bitmap.c */
t_fd_bitmap	*new_fd_bitmap(int size);
t_fd_bitmap	*grow_fd_bitmap(t_fd_bitmap *src, int fd);
void		close_fd_bitmap(t_fd_bitmap *fd_bitmap);
void		dispose_fd_bitmap(t_fd_bitmap *fd_bitmap);
/* pipe_utils.c */
t_stage		new_stage(int in, int out);
t_status	move_fd(int source, int target);
void		enter_child(t_stage st);
void		close_pipes(t_stage st);
/* register_pid.c */
t_status	register_pid(t_ctx *ctx, pid_t pid);
/* wait_children.c */
t_status	collect_child_result(t_ctx *ctx);

// <redirect>
/* apply_redirect.c */
t_status	apply_redirects(t_redirect *redirects, bool undoable);
t_status	undo_redirects(t_redirect *redirects);
/* heredoc_tmpfile.c */
char		*create_tmp_filename(void);
int			open_heredoc_fd(t_redirect *redirect);

// <utils>
/* args_utils.c */
int			count_args(t_word_list *args);
int			build_exec_params(t_exec_params *exec_params, t_word_list *args,
				t_hashtable *tmp_table, t_hashtable *env_table);
void		free_exec_params(char **argv, char **envp);
/* path_utils.c */
char		*extract_path_value(t_hashtable *tmp_table, t_hashtable *env_table);
char		*extract_path_entry(char *path_value);
/* envp_utils.c */
char		*env_lookup(t_hashtable *tmp_table, t_hashtable *env_table,
				char *name);
char		*make_env_entry(char *key, char *value);
char		**build_envp(t_hashtable *tmp_table, t_hashtable *env_table);
void		free_envp(char **envp);

#endif
