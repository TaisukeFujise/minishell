/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_buffer.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/25 19:37:05 by fendo             #+#    #+#             */
/*   Updated: 2026/08/25 19:37:06 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand_internal.h"

bool	expand_buf_init(t_expand_buf *buf)
{
	if (!strbuf_init(&buf->text))
		return (false);
	if (strbuf_init(&buf->mask))
		return (true);
	strbuf_free(&buf->text);
	return (false);
}

void	expand_buf_free(t_expand_buf *buf)
{
	strbuf_free(&buf->text);
	strbuf_free(&buf->mask);
}

void	expand_buf_reset(t_expand_buf *buf)
{
	buf->text.len = 0;
	buf->text.data[0] = '\0';
	buf->mask.len = 0;
	buf->mask.data[0] = '\0';
}

bool	expand_buf_append(t_expand_buf *buf, const char *str, size_t len,
					bool glob)
{
	size_t	mark;

	mark = buf->text.len;
	if (!strbuf_append(&buf->text, str, len))
		return (false);
	if (!strbuf_append(&buf->mask, str, len))
	{
		buf->text.len = mark;
		buf->text.data[mark] = '\0';
		return (false);
	}
	ft_memset(buf->mask.data + mark, glob, len);
	return (true);
}
