/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 22:09:51 by tafujise          #+#    #+#             */
/*   Updated: 2026/01/18 19:47:18 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTE_H
# define EXECUTE_H

# include "./parser.h"
# include <sys/types.h>
# include "./hashmap.h"

typedef struct s_ctx
{
	int			exit_code;
	t_hashtable	*env_table;//When execve, this is converted to envp.
	t_hashtable	*var_table;//this is not.
}	t_ctx;

typedef struct s_exec
{
	int			input_fd;
	int			output_fd;
	t_hashtable	*temp_table;
}	t_exec;

typedef struct s_entry_view
{
	char	*key;
	int		key_len;
	char	*value;
	int		value_len;
}	t_entry_view;

/* init.c */
int		init_ctx(t_ctx *ctx, char **envp);
/* execute.c */
void	execute(t_node *node, t_exec *executor, t_ctx *ctx);
void	exec_complete(t_node *node, t_exec *executor, t_ctx *ctx);
void	exec_andor(t_node *node, t_exec *executor, t_ctx *ctx);
void	exec_pipe(t_node *node, t_exec *executor, t_ctx *ctx);
void	exec_subshell(t_node *node, t_exec *executor, t_ctx *ctx);
void	exec_simple(t_node *node, t_exec *executor, t_ctx *ctx);

#endif
