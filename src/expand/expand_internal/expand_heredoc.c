#include "expand_internal.h"

static bool	add_dollar(t_expand *exp, t_word *raw, char **body)
{
	char	*value;
	t_param	param;

	param.s = *body;
	param.slen = raw->len - (*body - raw->str);
	value = expand_param(exp->ctx, &exp->arenas->tmp, &param);
	if (!value || !strbuf_add(&exp->buf, value, param.len))
		return (false);
	*body += param.used;
	return (true);
}

t_status	expand_heredoc_body(t_expand *exp, t_redirect *redir)
{
	char	*body;
	char	*next;
	char	*result;

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
		if (!strbuf_add(&exp->buf, body, next - body))
			return (ST_FATAL);
		body = next;
		if (*body && !add_dollar(exp, &redir->hd.raw_str, &body))
			return (ST_FATAL);
	}
	if (exp->buf.len > INT_MAX)
		return (ST_FATAL);
	result = ft_arena_strndup(&exp->arenas->heredoc,
			exp->buf.data, exp->buf.len);
	if (!result)
		return (ST_FATAL);
	redir->hd.raw_str.str = result;
	redir->hd.raw_str.len = (int)exp->buf.len;
	return (ST_OK);
}
