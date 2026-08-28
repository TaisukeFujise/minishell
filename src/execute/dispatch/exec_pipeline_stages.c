/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline_stages.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/28 19:00:00 by fendo             #+#    #+#             */
/*   Updated: 2026/08/28 19:00:00 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/execute.h"
#include "../../../include/parser.h"

/*
	How many processes a pipeline starts, counted before the first fork
	so that the process set is allocated once. [review PROC-01]
*/
int	count_stages(t_node *node)
{
	if (node != NULL && node->node_kind == NODE_PIPE)
		return (count_stages(node->left) + count_stages(node->right));
	return (1);
}

/*
	The stages of a pipeline in the order they are written. The parse
	tree nests them to the left; the coordinator wants a flat list, the
	way dash keeps them in the node itself.
*/
t_node	**collect_stages(t_node *node, t_node **out)
{
	if (node != NULL && node->node_kind == NODE_PIPE)
	{
		out = collect_stages(node->left, out);
		return (collect_stages(node->right, out));
	}
	*out = node;
	return (out + 1);
}
