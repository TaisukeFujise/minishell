#include "expand_internal/expand_internal.h"

static const char	*get_ifs(t_ctx *ctx)
{
	t_bucket_contents	*entry;

	if (!ctx->env_table)
		return (IFS_DEFAULT);
	entry = hash_search("IFS", ctx->env_table);
	if (!entry)
		return (IFS_DEFAULT);
	if (!entry->data.value)
		return ("");
	return (entry->data.value);
}

t_status	expand_command(t_node *node, t_ctx *ctx, t_arenas *arenas)
{
	t_expand	exp;
	t_status	status;

	exp.ctx = ctx;
	exp.arenas = arenas;
	exp.ifs = get_ifs(ctx);
	if (!strbuf_init(&exp.buf))
		return (ST_FATAL);
	status = ST_OK;
	if (node->node_kind == NODE_SIMPLE)
	{
		status = expand_assigns(&exp, node->u_node.simple_command.assigns);
		if (status == ST_OK)
			status = expand_args(&exp, &node->u_node.simple_command);
		if (status == ST_OK)
			status = expand_redirects(&exp,
					node->u_node.simple_command.redirects);
	}
	else if (node->node_kind == NODE_SUBSHELL)
		status = expand_redirects(&exp, node->u_node.subshell.redirects);
	free(exp.buf.data);
	return (status);
}
