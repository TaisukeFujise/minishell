/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_internal.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 21:40:40 by fendo             #+#    #+#             */
/*   Updated: 2026/03/02 23:39:09 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_INTERNAL_H
# define LEXER_INTERNAL_H

# include "../../../include/lexer.h"

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

typedef struct s_word_builder
{
	t_arena	*arena;
	t_word	**tail;
}	t_word_builder;

t_token_kind	lex_control(char **line, t_token *token);
t_token_kind	lex_connect(char **line, t_token *token);
t_token_kind	lex_group(char **line, t_token *token, t_lex_state *st);
t_token_kind	lex_redirect(char **line, t_token *token);
t_token_kind	lex_io_number(char **line, t_token *token);
t_token_kind	lex_word(char **line, t_token *token, t_arena *arena);

int				str2fd(char **line);
char			*strchunk(const char *tbl, const char *str, size_t n);
void			validate_assign(char *cur_ptr, t_assign_info *as);
void			set_lexer_error(t_token *token, t_lexer_err err);

int				is_tk_bound(char *ch);
t_lexer_err		append_part(t_word_builder *wb, char *str,
					int len, uint8_t flag);
t_lexer_err		commit_part(t_word_builder *wb, char **begin, char *end,
					uint8_t flag);
t_lexer_err		close_quote_part(t_word_builder *wb, char **line, char *begin,
					uint8_t flag);
void			lex_dollar(char **cur_ptr, uint8_t *flag);
void			apply_assign_info(t_word *head, t_assign_info *as);
void			set_assign_info(t_assign_info *as, t_assign_state state,
					char *eq_ptr, uint8_t flag);

#endif
