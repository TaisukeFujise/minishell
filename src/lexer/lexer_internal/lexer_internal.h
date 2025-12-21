/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_internal.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 21:40:40 by fendo             #+#    #+#             */
/*   Updated: 2025/12/21 22:30:22 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_INTERNAL_H
# define LEXER_INTERNAL_H

# include "minishell.h"
# include "lexer.h"

typedef struct s_lexeme
{
	t_token_kind	tk;
	union
	{
		t_word_desc		wd;
		t_op_connect	op_bin;
		t_op_group		op_group;
		t_op_redir		op_redir;
		int				io_num;
		int				err;
	}	u_lxm;
}	t_lexeme;



#endif