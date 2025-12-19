/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 19:27:55 by tafujise          #+#    #+#             */
/*   Updated: 2025/12/20 00:05:06 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parser.h"
#include "../../include/execute.h"

void	exec_complete(t_node *node, t_exec *executor)
{
	while (node != NULL)
	{
		execute(node->child, executor);
		node = node->next;	
	}
}	

void	exec_andor(t_node *node, t_exec *executor)
{
	while (node != NULL)
	{
		execute(node->child, executor);
		if (node->u_cmd.and_or.op == CONNECT_AND_IF)
			if (executor->exit_code != 0)// If the last exit status is not 0, end the process.
				break; 
		if (node->u_cmd.and_or.op == CONNECT_OR_IF)	
			if (executor->exit_code == 0) // If the last exit status is 0, end the process.
				break;		
		node = node->next;	
	}	
}	
//
//  void	exec_pipe(t_node *node, t_exec *executor)
// {
// 	while (node != NULL)
// 	{
// 			
// 	}	
// }	
//
// void	exec_subshell(t_node *node, t_exec *executor)
// {
//
// }	
//
void	execute(t_node *node, t_exec *executor)
{
	if (node == NULL) // when user_input is empty, ast root node is NULL.
		return ;  // it's NULL guard for thath
	if (node->node_kind == NODE_SIMPLE)
	{
		exec_cmd(node, executor); // execute commands connected by pipe 
	}
	execute(node->children, executor);
	// if (node->node_kind == NODE_PROGRAM)
		// exec_program(node, executor); //  
	if (node->node_kind == NODE_COMPS) 
		exec_complete(node, executor);
	else if (node->node_kind == NODE_ANDOR)
		exec_andor(node, executor); 
	else if (node->node_kind == NODE_PIPE) // 中でnext
		exec_pipe(node, executor);
	else if (node->node_kind == NODE_SUBSHELL)
		exec_subshell(node, executor);
	else
		exit(1);	
}


