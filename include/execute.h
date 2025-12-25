/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 22:09:51 by tafujise          #+#    #+#             */
/*   Updated: 2025/12/26 03:06:11 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTE_H
# define EXECUTE_H

#include "./parser.h"
#include <sys/types.h>
#include "./hashmap.h"

typedef	struct s_exec
{
	int			exit_code;
	t_hashtable	*env_table;
	t_hashtable	*temp_table;
}	t_exec;

/* init.c */
int		init_executor(t_exec *executor, char **envp);
/* execute.c */
void	execute(t_node *node, t_exec *executor);
void	exec_complete(t_node *node, t_exec *executor);
void	exec_andor(t_node *node, t_exec *executor);
void	exec_pipe(t_node *node, t_exec *executor);
void	exec_subshell(t_node *node, t_exec *executor);
void	exec_simple(t_node *node, t_exec *executor);

#endif
