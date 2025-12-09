/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   convert_char.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/19 11:26:51 by fendo             #+#    #+#             */
/*   Updated: 2025/10/19 21:15:06 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	conv_char(t_ctx *ctx, t_spec *sp, va_list *ap)
{
	char	c;
	t_emit	e;

	c = (char)va_arg(*ap, int);
	e.buf = &c;
	e.len = 1;
	e.sign_chr = 0;
	e.prefix = NULL;
	e.prefix_len = 0;
	e.prec_zeros = 0;
	emit_with_width(ctx, sp, &e);
}

void	conv_percent(t_ctx *ctx, t_spec *sp)
{
	char	c;

	(void)sp;
	c = '%';
	out_write(ctx, &c, 1);
}
