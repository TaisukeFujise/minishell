#include "expand_internal.h"
#include "strutil.h"

static char	*expand_status(t_ctx *ctx, t_arena *arena, size_t *len)
{
	char	*num;
	char	*copy;

	num = ft_itoa(ctx->err.exit_code);
	if (!num)
		return (NULL);
	*len = ft_strlen(num);
	copy = ft_arena_strdup(arena, num);
	free(num);
	return (copy);
}

static char	*set_param(t_param *param, size_t len, size_t used, char *value)
{
	param->len = len;
	param->used = used;
	return (value);
}

static char	*lookup_var(t_ctx *ctx, t_arena *arena, t_param *param)
{
	char				*key;
	t_bucket_contents	*entry;

	key = ft_arena_strndup(arena, param->s + 1, param->used - 1);
	if (!key)
		return (NULL);
	entry = NULL;
	if (ctx && ctx->env_table)
		entry = hash_search(key, ctx->env_table);
	if (!entry || !entry->data.value)
	{
		param->len = 0;
		return ("");
	}
	param->len = ft_strlen(entry->data.value);
	return (entry->data.value);
}

char	*expand_param(t_ctx *ctx, t_arena *arena, t_param *param)
{
	size_t	namelen;

	if (!param->s)
		return (set_param(param, 0, 0, ""));
	if (param->slen < 2)
		return (set_param(param, param->slen, param->slen, "$"));
	if (param->s[1] == '?')
	{
		param->used = 2;
		return (expand_status(ctx, arena, &param->len));
	}
	namelen = str_name_len(param->s + 1);
	if (namelen == 0)
		return (set_param(param, 1, 1, "$"));
	param->used = namelen + 1;
	return (lookup_var(ctx, arena, param));
}
