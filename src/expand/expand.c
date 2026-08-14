#include "expand_internal/expand_internal.h"

static const char	*get_ifs(t_ctx *ctx)
{
	t_bucket_contents	*entry;

	if (!ctx || !ctx->env_table)
		return (IFS_DEFAULT);
	entry = hash_search("IFS", ctx->env_table);
	if (!entry)
		return (IFS_DEFAULT);
	if (!entry->data.value)
		return ("");
	return (entry->data.value);
}

static t_status	expand_node(t_expand *exp, t_node *node)
{
	t_status	status;

	if (node->node_kind == NODE_SIMPLE)
	{
		status = expand_assigns(exp, node->u_node.simple_command.assigns);
		if (status == ST_OK)
			status = expand_args(exp, &node->u_node.simple_command);
		if (status == ST_OK)
			status = expand_redirects(exp,
					node->u_node.simple_command.redirects);
		return (status);
	}
	if (node->node_kind == NODE_SUBSHELL)
		return (expand_redirects(exp, node->u_node.subshell.redirects));
	return (ST_OK);
}

t_status	expand_command(t_node *node, t_ctx *ctx, t_arenas *arenas)
{
	t_expand	exp;
	t_status	status;

	if (!ctx || !arenas)
		return (ST_FATAL);
	if (!node)
		return (ST_OK);
	exp.ctx = ctx;
	exp.arenas = arenas;
	exp.ifs = get_ifs(ctx);
	if (!strbuf_init(&exp.buf))
		return (ST_FATAL);
	status = expand_node(&exp, node);
	free(exp.buf.data);
	return (status);
}
