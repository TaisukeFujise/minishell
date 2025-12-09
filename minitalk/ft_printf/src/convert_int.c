/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   convert_int.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/19 11:27:01 by fendo             #+#    #+#             */
/*   Updated: 2025/10/19 11:27:07 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static unsigned int	get_abs_value(int v)
{
	if (v < 0)
		return ((unsigned int)(-v));
	return ((unsigned int)v);
}

void	conv_int(t_ctx *ctx, t_spec *sp, va_list *ap)
{
	int		v;
	char	tmp[64];
	int		len;
	t_emit	e;

	v = va_arg(*ap, int);
	if (sp->prec_set && sp->precision == 0 && v == 0)
		len = 0;
	else
		len = u64_to_str(get_abs_value(v), 10, 0, tmp);
	e.buf = tmp;
	e.len = len;
	e.sign_chr = get_sign_char(sp, (v < 0));
	e.prefix = NULL;
	e.prefix_len = 0;
	if (sp->prec_set)
	{
		sp->f_zero = 0;
		e.prec_zeros = calc_prec_zeros(len, sp->precision);
	}
	else
		e.prec_zeros = 0;
	emit_with_width(ctx, sp, &e);
}
