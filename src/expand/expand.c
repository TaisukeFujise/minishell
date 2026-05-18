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

t_status	expand_command(void *cmd, t_ctx *ctx, t_arenas *arenas,
				bool is_simple)
{
	t_expand	ex;
	t_status	status;
	t_redirect	*redirects;

	if (!cmd || !ctx || !arenas)
		return (ST_FATAL);
	ex.ctx = ctx;
	ex.arenas = arenas;
	ex.ifs = get_ifs(ctx);
	if (is_simple)
	{
		status = expand_assigns(&ex, ((t_simple_cmd *)cmd)->assigns);
		if (status != ST_OK)
			return (status);
		status = expand_args(&ex, cmd);
		if (status != ST_OK)
			return (status);
		redirects = ((t_simple_cmd *)cmd)->redirects;
	}
	else
		redirects = ((t_subshell *)cmd)->redirects;
	return (expand_redirects(&ex, redirects));
}
