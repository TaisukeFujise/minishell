#include "parser_internal.h"

void	ilist_append(void *headp, void *item, size_t next_off)
{
	void	*head;
	void	*next;
	char	*cur;

	ft_memcpy(&head, headp, sizeof(void *));
	if (!head)
	{
		ft_memcpy(headp, &item, sizeof(void *));
		return ;
	}
	cur = (char *)head;
	while (1)
	{
		ft_memcpy(&next, cur + next_off, sizeof(void *));
		if (!next)
			break ;
		cur = (char *)next;
	}
	ft_memcpy(cur + next_off, &item, sizeof(void *));
}

t_node	*new_node(t_parser_state *ps, t_node_kind kind)
{
	t_node	*node;

	node = ft_arena_calloc(&ps->arenas->ast, 1, sizeof(t_node));
	if (!node)
	{
		parser_fail(ps, ST_FATAL, NULL);
		return (NULL);
	}
	node->node_kind = kind;
	return (node);
}

t_node	*make_binary(t_parser_state *ps, t_op_connect op, t_node *left,
	t_node *right)
{
	t_node		*node;
	t_node_kind	kind;

	if (ps->status != ST_OK || !left || !right)
		return (NULL);
	if (op == CONNECT_PIPE)
		kind = NODE_PIPE;
	else
		kind = NODE_ANDOR;
	node = new_node(ps, kind);
	if (!node)
		return (NULL);
	node->u_node.and_or.op = op;
	node->left = left;
	node->right = right;
	return (node);
}
