#include "../../../include/execute.h"
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

static char	*lookup_var(t_ctx *ctx, t_arena *arena, t_param *param);

static char	*set_param(t_param *param, size_t len, size_t used, char *value)
{
	param->len = len;
	param->used = used;
	return (value);
}

/*
	${name} names the same variable as $name; the braces only say where
	the name ends. Without a name and a closing brace the text stays.
*/
static char	*expand_braced(t_ctx *ctx, t_arena *arena, t_param *param)
{
	size_t	namelen;
	char	*value;

	namelen = str_name_len(param->s + 2);
	if (namelen == 0 || param->s[namelen + 2] != '}')
		return (set_param(param, 1, 1, "$"));
	param->s++;
	param->used = namelen + 1;
	value = lookup_var(ctx, arena, param);
	param->s--;
	param->used = namelen + 3;
	return (value);
}

static char	*lookup_var(t_ctx *ctx, t_arena *arena, t_param *param)
{
	char	*key;
	char	*value;

	key = ft_arena_strndup(arena, param->s + 1, param->used - 1);
	if (!key)
		return (NULL);
	value = NULL;
	if (ctx && ctx->env_table)
		value = env_lookup(ctx->tmp_table, ctx->env_table, key);
	if (!value)
	{
		param->len = 0;
		return ("");
	}
	param->len = ft_strlen(value);
	return (value);
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
	if (ft_isdigit(param->s[1]))
		return (set_param(param, 0, 2, ""));
	if (param->s[1] == '{')
		return (expand_braced(ctx, arena, param));
	namelen = str_name_len(param->s + 1);
	if (namelen == 0)
		return (set_param(param, 1, 1, "$"));
	param->used = namelen + 1;
	return (lookup_var(ctx, arena, param));
}
