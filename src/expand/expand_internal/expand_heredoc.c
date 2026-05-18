#include "expand_internal.h"

static bool	expand_dollar(t_strbuf *buf, t_ctx *ctx, char **body)
{
	char	*value;
	t_param	param;

	param.s = *body;
	param.slen = ft_strlen(*body);
	value = expand_param(ctx, buf->arena, &param);
	if (!value || !strbuf_add(buf, value, param.len))
		return (false);
	*body += param.used;
	return (true);
}

t_status	expand_heredoc_body(t_redirect *redir, t_ctx *ctx, t_arena *arena)
{
	t_strbuf	buf;
	char		*body;
	char		*next;

	if (!redir || !ctx || !arena || !redir->hd.raw_str.str)
		return (ST_FATAL);
	if ((redir->target.flag & (W_SQ | W_DQ)) != 0)
		return (ST_OK);
	if (!strbuf_init(&buf, arena))
		return (ST_FATAL);
	body = redir->hd.raw_str.str;
	while (*body)
	{
		next = ft_strchr(body, '$');
		if (!next)
			next = body + ft_strlen(body);
		if (!strbuf_add(&buf, body, next - body))
			return (ST_FATAL);
		body = next;
		if (*body && !expand_dollar(&buf, ctx, &body))
			return (ST_FATAL);
	}
	redir->hd.raw_str.str = buf.data;
	redir->hd.raw_str.len = (int)buf.len;
	return (ST_OK);
}
