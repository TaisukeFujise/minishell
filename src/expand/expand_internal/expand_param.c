#include "expand_internal.h"

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

static size_t	name_len(const char *s, size_t len)
{
	size_t	i;

	if (len < 2 || (!ft_isalpha(s[1]) && s[1] != '_'))
		return (0);
	i = 2;
	while (i < len && (ft_isalnum(s[i]) || s[i] == '_'))
		i++;
	return (i - 1);
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
		return (param->len = 0, param->used = 0, "");
	if (param->slen < 2)
		return (param->len = param->slen, param->used = param->slen, "$");
	if (param->s[1] == '?')
		return (param->used = 2, expand_status(ctx, arena, &param->len));
	namelen = name_len(param->s, param->slen);
	if (namelen == 0)
		return (param->len = 1, param->used = 1, "$");
	param->used = namelen + 1;
	return (lookup_var(ctx, arena, param));
}
