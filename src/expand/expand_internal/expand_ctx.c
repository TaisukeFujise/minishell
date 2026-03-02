#include "expand_internal.h"

static const char	*get_ifs(t_ctx *ctx)
{
	t_bucket_contents	*entry;

	entry = hash_search("IFS", ctx->env_table);
	if (!entry)
		return (IFS_DEFAULT);
	return (entry->data.value);
}

void	init_expand_ctx(t_expand_ctx *ex, t_ctx *ctx, t_arenas *arenas)
{
	ex->ast = &arenas->ast;
	ex->tmp = &arenas->tmp;
	ex->ctx = ctx;
	ex->ifs = get_ifs(ctx);
}
