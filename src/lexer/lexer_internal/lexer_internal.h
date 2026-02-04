/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_internal.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 21:40:40 by fendo             #+#    #+#             */
/*   Updated: 2026/02/04 18:19:34 by fendo            ###   ########.fr       */
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

typedef struct s_assign_info
{
	t_assign_state	state;
	char			*eq_ptr;
	uint8_t			flag;
}	t_assign_info;

typedef struct s_lex_state
{
	int		paren_depth;
}	t_lex_state;

t_token_kind	lex_control(char **line, t_token *token);
t_token_kind	lex_connect(char **line, t_token *token);
t_token_kind	lex_group(char **line, t_token *token, t_lex_state *st);
t_token_kind	lex_redirect(char **line, t_token *token);
t_token_kind	lex_io_number(char **line, t_token *token);
t_token_kind	lex_word(char **line, t_token *token);

void			skip_blank(char **line);
unsigned int	str2fd(char **line);
char			*strchunk(const char *tbl, const char *str, size_t n);
void			validate_assign(char *cur_ptr, t_assign_info *as);
void			set_lexer_error(t_token *token, int err);
int				free_word_parts(t_word *head);
int				is_tk_bound(char *ch);
int				append_part(t_word ***tail, char *str, int len, uint8_t flag);
int				finish_quote(char **line, t_word ***tail,
					char *begin, uint8_t flag);
void			lex_dollar(char **cur_ptr, uint8_t *flag);

#endif
