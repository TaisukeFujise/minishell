/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   emit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/19 11:27:13 by fendo             #+#    #+#             */
/*   Updated: 2025/10/19 11:27:14 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	calc_padding(t_spec *sp, t_emit *e)
{
	int	sign_width;
	int	pad_len;

	sign_width = 0;
	if (e->sign_chr)
		sign_width = 1;
	pad_len = sp->width - (e->prefix_len + sign_width + e->prec_zeros + e->len);
	if (pad_len < 0)
		pad_len = 0;
	return (pad_len);
}

static void	emit_padding(t_ctx *ctx, t_spec *sp, int pad_len, t_emit *e)
{
	if (sp->f_zero && pad_len > 0)
	{
		if (e->sign_chr)
			out_write(ctx, &e->sign_chr, 1);
		if (e->prefix_len)
			out_write(ctx, e->prefix, (size_t)e->prefix_len);
		out_repeat(ctx, '0', pad_len);
	}
	else
	{
		out_repeat(ctx, ' ', pad_len);
		if (e->sign_chr)
			out_write(ctx, &e->sign_chr, 1);
		if (e->prefix_len)
			out_write(ctx, e->prefix, (size_t)e->prefix_len);
	}
}

static void	emit_prefix_only(t_ctx *ctx, t_emit *e)
{
	if (e->sign_chr)
		out_write(ctx, &e->sign_chr, 1);
	if (e->prefix_len)
		out_write(ctx, e->prefix, (size_t)e->prefix_len);
}

void	emit_with_width(t_ctx *ctx, t_spec *sp, t_emit *e)
{
	int	pad_len;

	pad_len = calc_padding(sp, e);
	if (!sp->f_minus)
		emit_padding(ctx, sp, pad_len, e);
	else
		emit_prefix_only(ctx, e);
	if (e->prec_zeros > 0)
		out_repeat(ctx, '0', e->prec_zeros);
	if (e->len > 0)
		out_write(ctx, e->buf, (size_t)e->len);
	if (sp->f_minus && pad_len > 0)
		out_repeat(ctx, ' ', pad_len);
}
