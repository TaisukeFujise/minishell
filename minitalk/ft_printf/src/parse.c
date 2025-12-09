/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/19 11:27:22 by fendo             #+#    #+#             */
/*   Updated: 2025/10/19 21:15:06 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	init_spec(t_spec *sp)
{
	sp->f_minus = 0;
	sp->f_plus = 0;
	sp->f_space = 0;
	sp->f_hash = 0;
	sp->f_zero = 0;
	sp->width = 0;
	sp->precision = 0;
	sp->prec_set = 0;
	sp->conv = 0;
}

static const char	*parse_flags(const char *s, t_spec *sp)
{
	while (*s == '-' || *s == '+' || *s == ' ' || *s == '#' || *s == '0')
	{
		if (*s == '-')
			sp->f_minus = 1;
		else if (*s == '+')
			sp->f_plus = 1;
		else if (*s == ' ')
			sp->f_space = 1;
		else if (*s == '#')
			sp->f_hash = 1;
		else if (*s == '0')
			sp->f_zero = 1;
		++s;
	}
	return (s);
}

static const char	*parse_width(const char *s, t_spec *sp)
{
	sp->width = 0;
	while (*s >= '0' && *s <= '9')
	{
		if (sp->width > 2147483647 / 10)
			sp->width = 2147483647;
		else
		{
			sp->width = sp->width * 10 + (*s - '0');
			if (sp->width < 0)
				sp->width = 2147483647;
		}
		++s;
	}
	return (s);
}

static const char	*parse_precision(const char *s, t_spec *sp)
{
	if (*s != '.')
		return (s);
	++s;
	sp->prec_set = 1;
	sp->precision = 0;
	while (*s >= '0' && *s <= '9')
	{
		sp->precision = sp->precision * 10 + (*s - '0');
		++s;
	}
	return (s);
}

const char	*parse_spec(const char *fmt, t_spec *sp, va_list *ap)
{
	(void)ap;
	init_spec(sp);
	fmt = parse_flags(fmt, sp);
	fmt = parse_width(fmt, sp);
	fmt = parse_precision(fmt, sp);
	sp->conv = *fmt;
	if (sp->prec_set)
		sp->f_zero = 0;
	if (*fmt)
		return (fmt + 1);
	return (fmt);
}
