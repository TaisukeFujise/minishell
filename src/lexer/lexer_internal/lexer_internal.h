/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_internal.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 21:40:40 by fendo             #+#    #+#             */
/*   Updated: 2025/12/24 17:28:07 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_INTERNAL_H
# define LEXER_INTERNAL_H

# include "lexer.h"

void	lex_control(char **line, t_token *token);
void	lex_connect(char **line, t_token *token);
void	lex_group(char **line, t_token *token);
void	lex_redirect(char **line, t_token *token);
void	lex_io_number(char **line, t_token *token);
void	lex_word(char **line, t_token *token);

#endif