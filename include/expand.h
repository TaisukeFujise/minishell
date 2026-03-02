/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 19:46:17 by fendo             #+#    #+#             */
/*   Updated: 2026/03/01 19:28:04 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPAND_H
# define EXPAND_H

# include "lexer.h"
# include "parser.h"

/*
** Public API
** - expand_simple_cmd: Expand args and redirects of a simple command
** - expand_heredoc_body: Expand heredoc body if delimiter is unquoted
*/

t_status	expand_simple_cmd(t_simple_cmd *cmd, t_ctx *ctx, t_arenas *arenas);
char		*expand_heredoc_body(const char *body, size_t len, bool quoted,
				t_ctx *ctx, t_arena *arena);

#endif