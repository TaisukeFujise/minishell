/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 22:09:51 by tafujise          #+#    #+#             */
/*   Updated: 2025/12/19 22:53:53 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTE_H
# define EXECUTE_H

#include "./parser.h"

// hash table 
typedef struct s_entry t_entry;

struct	s_entry {
	char	*key;
	char	*value;
	t_entry	*next;
};

typedef struct s_hashmap t_hashmap;

struct	s_hashmap {
	t_entry	**entries;
	int		size;	
};

typedef	struct s_exec
{
	int			exit_code;
	t_hashmap	*env;	
}	t_exec;

void	execute(t_node *node, t_exec *executor);
void	exec_complete(t_node *node, t_exec *executor);
void	exec_andor(t_node *node, t_exec *executor);
void	exec_pipe(t_node *node, t_exec *executor);
void	exec_subshell(t_node *node, t_exec *executor);
void	exec_cmd(t_node *node, t_exec *executor);

#endif
