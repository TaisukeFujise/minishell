/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   out.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/19 11:27:21 by fendo             #+#    #+#             */
/*   Updated: 2025/10/19 11:27:22 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	out_write(t_ctx *ctx, const void *buf, size_t n)
{
	ssize_t		w;
	size_t		i;
	const char	*p;

	if (ctx->total < 0)
		return ;
	if (ctx->fd >= 0)
	{
		w = write(ctx->fd, buf, n);
		if (w < 0)
			ctx->total = -1;
		else
			ctx->total += (long)w;
	}
	else
	{
		p = buf;
		i = 0;
		while (i < n)
			ctx->s[i++] = *p++;
		ctx->s += n;
		ctx->total += n;
	}
}

void	out_repeat(t_ctx *ctx, char c, size_t n)
{
	while (n--)
		out_write(ctx, &c, 1);
}
