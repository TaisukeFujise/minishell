/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   convert_uint.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/19 11:27:11 by fendo             #+#    #+#             */
/*   Updated: 2025/10/19 11:27:12 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	conv_uint(t_ctx *ctx, t_spec *sp, va_list *ap)
{
	unsigned int	v;
	char			tmp[64];
	int				len;
	t_emit			e;

	v = va_arg(*ap, unsigned int);
	if (sp->prec_set && sp->precision == 0 && v == 0)
		len = 0;
	else
		len = u64_to_str(v, 10, 0, tmp);
	e.buf = tmp;
	e.len = len;
	e.sign_chr = 0;
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
