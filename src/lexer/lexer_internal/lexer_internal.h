/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_internal.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 21:40:40 by fendo             #+#    #+#             */
/*   Updated: 2025/12/26 22:45:06 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_INTERNAL_H
# define LEXER_INTERNAL_H

# include "lexer.h"

typedef enum e_asgn_state
{
	AS_INIT,
	AS_VALID,
	AS_INVALID,
	AS_DONE
}	t_asgn_state;

void			lex_control(char **line, t_token *token);
void			lex_connect(char **line, t_token *token);
void			lex_group(char **line, t_token *token);
void			lex_redirect(char **line, t_token *token);
void			lex_io_number(char **line, t_token *token);
void			lex_word(char **line, t_token *token);

void			skip_blank(char **line);
unsigned int	str2fd(char **line);
char			*strchunk(const char *tbl, const char *str, size_t n);
void			scan_asgn(char c, t_token *token, uint8_t *as);

#endif