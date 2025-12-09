/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/19 11:27:19 by fendo             #+#    #+#             */
/*   Updated: 2025/10/19 21:21:16 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_printf(const char *fmt, ...)
{
	t_ctx	ctx;
	va_list	ap;
	int		ret;

	ctx.fd = 1;
	ctx.s = NULL;
	ctx.total = 0;
	va_start(ap, fmt);
	ret = ft_vfprintf(&ctx, fmt, ap);
	va_end(ap);
	return (ret);
}

int	ft_dprintf(int fd, const char *fmt, ...)
{
	t_ctx	ctx;
	va_list	ap;
	int		ret;

	ctx.fd = fd;
	ctx.s = NULL;
	ctx.total = 0;
	va_start(ap, fmt);
	ret = ft_vfprintf(&ctx, fmt, ap);
	va_end(ap);
	return (ret);
}

int	ft_sprintf(char *str, const char *fmt, ...)
{
	t_ctx	ctx;
	va_list	ap;
	int		ret;

	ctx.fd = -1;
	ctx.s = str;
	ctx.total = 0;
	va_start(ap, fmt);
	ret = ft_vfprintf(&ctx, fmt, ap);
	va_end(ap);
	if (ctx.total >= 0)
		*ctx.s = '\0';
	return (ret);
}
