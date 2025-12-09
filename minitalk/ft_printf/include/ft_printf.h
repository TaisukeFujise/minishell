/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/19 11:26:48 by fendo             #+#    #+#             */
/*   Updated: 2025/12/01 15:24:56 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include <stddef.h>
# include <unistd.h>

typedef struct s_spec
{
	int			f_minus;
	int			f_plus;
	int			f_space;
	int			f_hash;
	int			f_zero;
	int			width;
	int			precision;
	int			prec_set;
	char		conv;
	const char	*fmt_start;
	const char	*fmt_end;
}	t_spec;

typedef struct s_ctx
{
	int		fd;
	char	*s;
	long	total;
}	t_ctx;

typedef struct s_emit
{
	const char	*buf;
	int			len;
	int			sign_chr;
	const char	*prefix;
	int			prefix_len;
	int			prec_zeros;
}	t_emit;

void			out_write(t_ctx *ctx, const void *buf, size_t n);
void			out_repeat(t_ctx *ctx, char c, size_t n);

void			emit_with_width(t_ctx *ctx, t_spec *sp, t_emit *e);

const char		*parse_spec(const char *fmt, t_spec *sp, va_list *ap);
void			init_spec(t_spec *sp);

void			conv_char(t_ctx *ctx, t_spec *sp, va_list *ap);
void			conv_str(t_ctx *ctx, t_spec *sp, va_list *ap);
void			conv_percent(t_ctx *ctx, t_spec *sp);
void			conv_int(t_ctx *ctx, t_spec *sp, va_list *ap);
void			conv_uint(t_ctx *ctx, t_spec *sp, va_list *ap);
void			conv_hex(t_ctx *ctx, t_spec *sp, va_list *ap, int upper);
void			conv_ptr(t_ctx *ctx, t_spec *sp, va_list *ap);
void			conv_float(t_ctx *ctx, t_spec *sp, va_list *ap);

int				numlen_u64(unsigned long long v, int base);
int				u64_to_str(unsigned long long v, int base,
					int upper, char *dst);
int				calc_prec_zeros(int len, int prec);
int				get_sign_char(t_spec *sp, int is_negative);

int				ft_printf(const char *fmt, ...);
int				ft_dprintf(int fd, const char *fmt, ...);
int				ft_sprintf(char *str, const char *fmt, ...);
int				ft_vfprintf(t_ctx *ctx, const char *fmt, va_list ap);

#endif