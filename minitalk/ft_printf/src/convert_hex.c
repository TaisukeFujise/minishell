/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   convert_hex.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/19 11:26:55 by fendo             #+#    #+#             */
/*   Updated: 2025/10/19 11:26:56 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	get_prefix_len(int alt, int is_zero)
{
	if (alt && !is_zero)
		return (2);
	return (0);
}

static const char	*get_hex_prefix(int upper)
{
	if (upper)
		return ("0X");
	return ("0x");
}

void	conv_hex(t_ctx *ctx, t_spec *sp, va_list *ap, int upper)
{
	unsigned int	v;
	char			tmp[64];
	int				len;
	t_emit			e;

	v = va_arg(*ap, unsigned int);
	if (sp->prec_set && sp->precision == 0 && v == 0)
		len = 0;
	else
		len = u64_to_str(v, 16, upper, tmp);
	e.buf = tmp;
	e.len = len;
	e.sign_chr = 0;
	e.prefix = get_hex_prefix(upper);
	e.prefix_len = get_prefix_len(sp->f_hash, (v == 0));
	if (sp->prec_set)
	{
		sp->f_zero = 0;
		e.prec_zeros = calc_prec_zeros(len, sp->precision);
	}
	else
		e.prec_zeros = 0;
	emit_with_width(ctx, sp, &e);
}
