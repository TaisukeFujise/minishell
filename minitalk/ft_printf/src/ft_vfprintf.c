/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_vfprintf.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 15:24:16 by fendo             #+#    #+#             */
/*   Updated: 2025/12/01 15:24:17 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static void	dispatch(t_ctx *ctx, t_spec *sp, va_list *ap)
{
	if (sp->conv == 'c')
		conv_char(ctx, sp, ap);
	else if (sp->conv == 's')
		conv_str(ctx, sp, ap);
	else if (sp->conv == '%')
		conv_percent(ctx, sp);
	else if (sp->conv == 'd' || sp->conv == 'i')
		conv_int(ctx, sp, ap);
	else if (sp->conv == 'u')
		conv_uint(ctx, sp, ap);
	else if (sp->conv == 'x')
		conv_hex(ctx, sp, ap, 0);
	else if (sp->conv == 'X')
		conv_hex(ctx, sp, ap, 1);
	else if (sp->conv == 'p')
		conv_ptr(ctx, sp, ap);
	else if (sp->conv == 'f')
		conv_float(ctx, sp, ap);
	else
		out_write(ctx, sp->fmt_start - 1,
			(size_t)(sp->fmt_end - (sp->fmt_start - 1)));
}

static void	process_literal(t_ctx *ctx, const char **p)
{
	const char	*b;

	b = *p;
	while (**p && **p != '%')
		++(*p);
	out_write(ctx, b, (size_t)(*p - b));
}

static void	process_format(t_ctx *ctx, const char **p, va_list *ap)
{
	t_spec	sp;

	++(*p);
	sp.fmt_start = *p;
	*p = parse_spec(*p, &sp, ap);
	sp.fmt_end = *p;
	dispatch(ctx, &sp, ap);
}

int	ft_vfprintf(t_ctx *ctx, const char *fmt, va_list ap)
{
	const char	*p;
	int			ret;
	va_list		ap_copy;

	va_copy(ap_copy, ap);
	p = fmt;
	while (*p && ctx->total >= 0)
	{
		if (*p != '%')
			process_literal(ctx, &p);
		else
			process_format(ctx, &p, &ap_copy);
	}
	va_end(ap_copy);
	if (ctx->total < 0)
		ret = -1;
	else
		ret = (int)ctx->total;
	return (ret);
}
