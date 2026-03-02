#include "expand_internal.h"

static char	*lookup_var(t_expand_ctx *ex, const char *name, int namelen)
{
	char				*key;
	t_bucket_contents	*entry;

	key = ft_arena_strndup(ex->tmp, name, namelen);
	if (!key)
		return (NULL);
	entry = hash_search(key, ex->ctx->env_table);
	if (!entry || !entry->data.value)
		return ("");
	return (entry->data.value);
}

static void	expand_exit_status(t_expand_ctx *ex, t_expbuf *buf,
								bool splittable, bool globbable)
{
	char	num[12];
	int		n;
	int		i;
	int		code;

	code = ex->ctx->err.exit_code;
	if (code == 0)
	{
		expbuf_append(buf, "0", 1, splittable, globbable, ex->tmp);
		return ;
	}
	i = 0;
	n = code;
	while (n > 0)
	{
		num[i++] = '0' + (n % 10);
		n /= 10;
	}
	while (i > 0)
		expbuf_append(buf, &num[--i], 1, splittable, globbable, ex->tmp);
}

static void	expand_var(t_expand_ctx *ex, t_word *part, t_expbuf *buf,
						bool splittable, bool globbable)
{
	const char	*name;
	int			namelen;
	char		*value;

	name = part->str + 1;
	namelen = part->len - 1;
	value = lookup_var(ex, name, namelen);
	if (value)
		expbuf_append(buf, value, ft_strlen(value),
			splittable, globbable, ex->tmp);
}

/*
** Expand a W_DOLL part:
**   $? -> exit code
**   $NAME -> environment variable value
** 
** splittable/globbable depend on whether this is in quotes (W_DQ flag)
*/
void	expand_param(t_expand_ctx *ex, t_word *part, t_expbuf *buf)
{
	bool	in_dq;
	bool	splittable;
	bool	globbable;

	in_dq = (part->flag & W_DQ) != 0;
	splittable = !in_dq;
	globbable = !in_dq;
	if (part->len >= 2 && part->str[1] == '?')
		expand_exit_status(ex, buf, splittable, globbable);
	else if (part->len > 1)
		expand_var(ex, part, buf, splittable, globbable);
}
