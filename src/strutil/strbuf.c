/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strbuf.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/25 19:35:28 by fendo             #+#    #+#             */
/*   Updated: 2026/08/26 17:13:05 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

#define STRBUF_INIT_CAP 64

static bool	strbuf_reserve(t_strbuf *buf, size_t required)
{
	char	*new_data;
	size_t	new_cap;

	if (required <= buf->cap)
		return (true);
	new_cap = required;
	if (buf->cap <= SIZE_MAX / 2)
	{
		new_cap = buf->cap * 2;
		if (new_cap < required)
			new_cap = required;
	}
	new_data = ft_realloc(buf->data, buf->cap, new_cap);
	if (!new_data)
		return (false);
	buf->data = new_data;
	buf->cap = new_cap;
	return (true);
}

bool	strbuf_init(t_strbuf *buf)
{
	buf->data = malloc(STRBUF_INIT_CAP);
	if (!buf->data)
		return (false);
	buf->data[0] = '\0';
	buf->len = 0;
	buf->cap = STRBUF_INIT_CAP;
	return (true);
}

bool	strbuf_append(t_strbuf *buf, const char *str, size_t len)
{
	if (len == 0)
		return (true);
	if (len > SIZE_MAX - buf->len - 1)
		return (false);
	if (!strbuf_reserve(buf, buf->len + len + 1))
		return (false);
	ft_memcpy(buf->data + buf->len, str, len);
	buf->len += len;
	buf->data[buf->len] = '\0';
	return (true);
}

char	*strbuf_detach(t_strbuf *buf, size_t *len)
{
	char	*data;

	data = buf->data;
	if (len)
		*len = buf->len;
	buf->data = NULL;
	buf->len = 0;
	buf->cap = 0;
	return (data);
}

void	strbuf_free(t_strbuf *buf)
{
	free(buf->data);
	buf->data = NULL;
	buf->len = 0;
	buf->cap = 0;
}
