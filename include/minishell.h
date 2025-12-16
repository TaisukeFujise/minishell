/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 20:40:52 by tafujise          #+#    #+#             */
/*   Updated: 2025/12/17 03:07:11 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>

typedef enum e_op
{
	OP_AND_IF,
	OP_OR_IF,
	OP_LESS,
	OP_GRAET,
	OP_DLESS,
	OP_DGREAT,
	OP_PIPE,
	OP_RPAREN,
	OP_LPAREN,
}	t_op;

typedef enum e_flag
{
	W_SQ	= 1u << 0,
	W_DQ	= 1u << 1,
	W_DOLL	= 1u << 2,
	W_WILD	= 1u << 3,
}	t_flag;

typedef struct s_word
{
	char	*str;
	size_t	len;
}	t_word;

typedef struct s_word_desc
{
	t_word	word;
	size_t	flag;
}	t_word_desc;

#endif
