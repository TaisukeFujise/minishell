/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 20:40:52 by tafujise          #+#    #+#             */
/*   Updated: 2025/12/18 22:46:02 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <signal.h>
# include "../libft/libft.h"
# define SUCCESS 0
# define FAILURE -1

extern volatile sig_atomic_t	g_signum;

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
