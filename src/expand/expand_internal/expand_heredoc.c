#include "expand_internal.h"

static bool	add_dollar(t_expand *exp, t_word *raw, char **body)
{
	char	*value;
	t_param	param;

	param.s = *body;
	param.slen = raw->len - (size_t)(*body - raw->str);
	value = expand_param(exp->ctx, &exp->arenas->tmp, &param);
	if (!value || !strbuf_append(&exp->buf, value, param.len))
		return (false);
	*body += param.used;
	return (true);
}

static t_status	store_body(t_expand *exp, t_redirect *redir)
{
	char	*result;

	result = ft_arena_strndup(&exp->arenas->heredoc,
			exp->buf.data, exp->buf.len);
	if (!result)
		return (ST_FATAL);
	redir->hd.raw_str.str = result;
	redir->hd.raw_str.len = exp->buf.len;
	return (ST_OK);
}

t_status	expand_heredoc_body(t_expand *exp, t_redirect *redir)
{
	char	*body;
	char	*next;

	if (!exp || !redir || !redir->hd.raw_str.str)
		return (ST_FATAL);
	if ((redir->target.flag & (W_SQ | W_DQ)) != 0)
		return (ST_OK);
	strbuf_reset(&exp->buf);
	body = redir->hd.raw_str.str;
	while (*body)
	{
		next = ft_strchr(body, '$');
		if (!next)
			next = redir->hd.raw_str.str + redir->hd.raw_str.len;
		if (!strbuf_append(&exp->buf, body, (size_t)(next - body)))
			return (ST_FATAL);
		body = next;
		if (*body && !add_dollar(exp, &redir->hd.raw_str, &body))
			return (ST_FATAL);
	}
	return (store_body(exp, redir));
}
