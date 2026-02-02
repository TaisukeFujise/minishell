/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_internal.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 21:40:40 by fendo             #+#    #+#             */
/*   Updated: 2026/02/02 14:15:06 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_INTERNAL_H
# define LEXER_INTERNAL_H

# include "lexer.h"

typedef enum e_assign_state
{
	AS_INIT,
	AS_VALID,
	AS_INVALID,
	AS_DONE
}	t_assign_state;

typedef struct s_lex_state
{
	int	paren_depth;
}	t_lex_state;

void			lex_control(char **line, t_token *token);
void			lex_connect(char **line, t_token *token);
void			lex_group(char **line, t_token *token, t_lex_state *st);
void			lex_redirect(char **line, t_token *token);
void			lex_io_number(char **line, t_token *token);
void			lex_word(char **line, t_token *token);

void			skip_blank(char **line);
unsigned int	str2fd(char **line);
char			*strchunk(const char *tbl, const char *str, size_t n);
void			validate_assign(char *cur_ptr, t_token *token,
					t_assign_state *as);
void			set_lex_error(t_token *token, int err);
int				free_word_parts(t_word *head);

#endif
