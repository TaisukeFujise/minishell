/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   convert_float.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 15:23:50 by fendo             #+#    #+#             */
/*   Updated: 2025/12/01 15:23:51 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static void	get_float_parts(double v, int prec, unsigned long long *i_part,
				double *f_part)
{
	double	rounding;
	int		i;

	rounding = 0.5;
	i = 0;
	while (i < prec)
	{
		rounding /= 10.0;
		i++;
	}
	v += rounding;
	*i_part = (unsigned long long)v;
	*f_part = v - *i_part;
}

static void	fill_fraction(char *buf, double f_part, int prec)
{
	int	i;
	int	digit;

	i = 0;
	while (i < prec)
	{
		f_part *= 10.0;
		digit = (int)f_part;
		buf[i++] = digit + '0';
		f_part -= digit;
	}
	buf[i] = '\0';
}

static int	ft_dtoa(double v, int prec, char *buf)
{
	unsigned long long	i_part;
	double				f_part;
	int					len;

	get_float_parts(v, prec, &i_part, &f_part);
	len = u64_to_str(i_part, 10, 0, buf);
	if (prec > 0)
	{
		buf[len++] = '.';
		fill_fraction(buf + len, f_part, prec);
		len += prec;
	}
	else
		buf[len] = '\0';
	return (len);
}

void	conv_float(t_ctx *ctx, t_spec *sp, va_list *ap)
{
	double	v;
	char	tmp[128];
	int		len;
	t_emit	e;
	int		is_neg;

	v = va_arg(*ap, double);
	is_neg = (v < 0 || (v == 0.0 && 1.0 / v < 0));
	if (is_neg)
		v = -v;
	e.sign_chr = get_sign_char(sp, is_neg);
	if (!sp->prec_set)
		sp->precision = 6;
	len = ft_dtoa(v, sp->precision, tmp);
	e.buf = tmp;
	e.len = len;
	e.prefix = NULL;
	e.prefix_len = 0;
	e.prec_zeros = 0;
	emit_with_width(ctx, sp, &e);
}
