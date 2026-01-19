/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 22:09:51 by tafujise          #+#    #+#             */
/*   Updated: 2026/01/18 21:26:31 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTE_H
# define EXECUTE_H

# include "./parser.h"
# include <sys/types.h>
# include "./hashmap.h"
# include "./minishell.h"

typedef struct s_exec
{
	int			input_fd;
	int			output_fd;
	t_hashtable	*tmp_table;
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
t_status	execute(t_node *node, t_exec *executor, t_ctx *ctx);
t_status	exec_complete(t_node *node, t_exec *executor, t_ctx *ctx);
t_status	exec_andor(t_node *node, t_exec *executor, t_ctx *ctx);
t_status	exec_pipe(t_node *node, t_exec *executor, t_ctx *ctx);
t_status	exec_subshell(t_node *node, t_exec *executor, t_ctx *ctx);
t_status	exec_simple(t_simple_cmd *cmd, t_exec *executor, t_ctx *ctx);

#endif
