/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   convert_str.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/19 11:27:10 by fendo             #+#    #+#             */
/*   Updated: 2025/12/01 15:50:14 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "libft.h"

static int	ft_strnlen(const char *str, int n)
{
	int	i;

	if (str == NULL)
		return (0);
	i = 0;
	while (i < n && str[i] != '\0')
		i++;
	return (i);
}

static void	handle_null_str(t_spec *sp, t_emit *e)
{
	if (sp->prec_set && sp->precision < 6)
	{
		e->buf = "";
		e->len = 0;
	}
	else
	{
		e->buf = "(null)";
		e->len = 6;
	}
}

static int	calc_str_len(const char *s, t_spec *sp)
{
	int	len;

	if (sp->prec_set)
		len = ft_strnlen(s, sp->precision);
	else
		len = ft_strlen(s);
	if (sp->prec_set && sp->precision < len)
		len = sp->precision;
	return (len);
}

void	conv_str(t_ctx *ctx, t_spec *sp, va_list *ap)
{
	const char	*s;
	t_emit		e;

	s = va_arg(*ap, const char *);
	if (!s)
		handle_null_str(sp, &e);
	else
	{
		e.buf = s;
		e.len = calc_str_len(s, sp);
	}
	e.sign_chr = 0;
	e.prefix = NULL;
	e.prefix_len = 0;
	e.prec_zeros = 0;
	emit_with_width(ctx, sp, &e);
}
