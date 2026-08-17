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
	The processes one operation started. A pipeline owns one set for all
	its stages and waits for them together; a command outside a pipeline
	has none and waits for its own child. Capacity is taken before the
	first fork, so a child always has a place to be recorded.
*/
typedef struct s_procs
{
	pid_t	*pids;
	int		count;
	int		capacity;
}			t_procs;

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
t_status	execute_internal(t_node *node, t_ctx *ctx, bool own);
int			count_stages(t_node *node);
t_node		**collect_stages(t_node *node, t_node **out);

// <dispatch>
/* exec_builtin.c */
t_status	exec_builtin(t_simple_cmd *cmd, t_ctx *ctx);
/* exec_connection.c */
t_status	exec_connection(t_node *node, t_ctx *ctx);
t_status	exec_complete(t_node *node, t_ctx *ctx);
t_status	exec_andor(t_node *node, t_ctx *ctx);
/* exec_pipeline.c */
t_status	exec_pipeline(t_node *node, t_ctx *ctx);
/* exec_disk.c */
t_status	exec_disk_command(t_simple_cmd *cmd, t_ctx *ctx, bool own);
/* exec_simple.c */
t_status	exec_simple(t_node *node, t_ctx *ctx, bool own);
t_status	set_exit_code(t_ctx *ctx, t_status status);
/* exec_subshell.c */
t_status	exec_subshell(t_node *node, t_ctx *ctx, bool own);

// <expansion>
/* assigns.c */
t_status	apply_assign(t_assign *assign, t_hashtable *table, t_ctx *ctx,
				t_tabletype type);
/* expand.c */

// <process>
/* pipe_utils.c */
t_status	move_fd(int source, int target);
/* procs.c */
bool		procs_init(t_procs *procs, int capacity);
t_status	procs_add(t_procs *procs, pid_t pid);
void		procs_free(t_procs *procs);
/* wait_children.c */
t_status	wait_pid_status(t_ctx *ctx, pid_t pid);
t_status	procs_wait(t_procs *procs, t_ctx *ctx);

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
