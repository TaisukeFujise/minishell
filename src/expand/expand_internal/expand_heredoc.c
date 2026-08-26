/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/25 19:36:53 by fendo             #+#    #+#             */
/*   Updated: 2026/08/25 19:36:54 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand_internal.h"

static bool	add_dollar(t_expand *exp, t_word *raw, char **body)
{
	char	*value;
	t_param	param;

	param.s = *body;
	param.slen = raw->len - (size_t)(*body - raw->str);
	value = expand_param(exp->ctx, &exp->arenas->tmp, &param);
	if (!value || !expand_buf_append(&exp->buf, value, param.len, false))
		return (false);
	*body += param.used;
	return (true);
}

static t_status	store_body(t_expand *exp, t_redirect *redir)
{
	char	*old;

	old = redir->hd.raw_str.str;
	redir->hd.raw_str.str = strbuf_detach(&exp->buf.text,
			&redir->hd.raw_str.len);
	free(old);
	if (!strbuf_init(&exp->buf.text))
		return (ST_FATAL);
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
	expand_buf_reset(&exp->buf);
	body = redir->hd.raw_str.str;
	while (*body)
	{
		next = ft_strchr(body, '$');
		if (!next)
			next = redir->hd.raw_str.str + redir->hd.raw_str.len;
		if (!expand_buf_append(&exp->buf, body,
				(size_t)(next - body), false))
			return (ST_FATAL);
		body = next;
		if (*body && !add_dollar(exp, &redir->hd.raw_str, &body))
			return (ST_FATAL);
	}
	return (store_body(exp, redir));
}
