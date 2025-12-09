/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/19 11:27:24 by fendo             #+#    #+#             */
/*   Updated: 2025/10/19 13:37:28 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	numlen_u64(unsigned long long v, int base)
{
	int	len;

	len = 1;
	while (v >= (unsigned long long)base)
	{
		v /= (unsigned long long)base;
		++len;
	}
	return (len);
}

static void	reverse_str(char *dst, int len)
{
	int		i;
	char	t;

	i = 0;
	while (i < len / 2)
	{
		t = dst[i];
		dst[i] = dst[len - 1 - i];
		dst[len - 1 - i] = t;
		i++;
	}
}

int	u64_to_str(unsigned long long v, int base, int upper, char *dst)
{
	static const char	*l = "0123456789abcdef";
	static const char	*u = "0123456789ABCDEF";
	const char			*digit_set;
	int					i;

	if (upper)
		digit_set = u;
	else
		digit_set = l;
	if (base < 2 || base > 16)
		base = 10;
	i = 0;
	if (v == 0)
	{
		dst[0] = '0';
		return (1);
	}
	while (v)
	{
		dst[i++] = digit_set[v % (unsigned long long)base];
		v /= (unsigned long long)base;
	}
	reverse_str(dst, i);
	return (i);
}

int	calc_prec_zeros(int len, int prec)
{
	int	zeros;

	zeros = prec - len;
	if (zeros <= 0)
		return (0);
	return (zeros);
}

int	get_sign_char(t_spec *sp, int is_negative)
{
	if (is_negative)
		return ('-');
	if (sp->f_plus)
		return ('+');
	if (sp->f_space)
		return (' ');
	return (0);
}
