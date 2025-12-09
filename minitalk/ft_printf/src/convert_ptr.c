/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   convert_ptr.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/19 11:27:04 by fendo             #+#    #+#             */
/*   Updated: 2025/10/19 21:21:16 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static void	conv_ptr_null(t_ctx *ctx, t_spec *sp)
{
	t_emit	e;

	e.buf = "(nil)";
	e.len = 5;
	e.sign_chr = 0;
	e.prefix = NULL;
	e.prefix_len = 0;
	e.prec_zeros = 0;
	emit_with_width(ctx, sp, &e);
}

void	conv_ptr(t_ctx *ctx, t_spec *sp, va_list *ap)
{
	unsigned long long	v;
	char				tmp[18];
	int					len;
	t_emit				e;

	v = (unsigned long long)va_arg(*ap, void *);
	if (v == 0)
	{
		conv_ptr_null(ctx, sp);
		return ;
	}
	len = u64_to_str(v, 16, 0, tmp);
	e.buf = tmp;
	e.len = len;
	e.sign_chr = 0;
	e.prefix = "0x";
	e.prefix_len = 2;
	e.prec_zeros = 0;
	emit_with_width(ctx, sp, &e);
}
