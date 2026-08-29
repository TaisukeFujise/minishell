/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_internal.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/25 19:36:48 by fendo             #+#    #+#             */
/*   Updated: 2026/08/25 19:36:49 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPAND_INTERNAL_H
# define EXPAND_INTERNAL_H

# include "execute.h"
# include "expand.h"
# include <dirent.h>

# define IFS_DEFAULT " \t\n"
# define EXPAND_MSG_AMBIG_REDIR "ambiguous redirect"

typedef struct s_expand_buf
{
	t_strbuf	text;
	t_strbuf	mask;
}	t_expand_buf;

typedef struct s_expand
{
	t_ctx			*ctx;
	t_arenas		*arenas;
	const char		*ifs;
	t_expand_buf	buf;
}	t_expand;

typedef struct s_fields
{
	t_word_list	*head;
	t_word_list	**tail;
	bool		glob;
	bool		pending_ws;
	bool		keep_empty;
}	t_fields;

typedef struct s_param
{
	const char	*s;
	size_t		slen;
	size_t		len;
	size_t		used;
}	t_param;

t_status	expand_args(t_expand *exp, t_simple_cmd *cmd);
t_status	expand_redirects(t_expand *exp, t_redirect *redir);
t_status	expand_assigns(t_expand *exp, t_assign *assign,
				t_hashtable *table, t_tabletype type);
t_status	expand_heredoc_body(t_expand *exp, t_redirect *redir);
bool		expand_buf_init(t_expand_buf *buf);
void		expand_buf_free(t_expand_buf *buf);
void		expand_buf_reset(t_expand_buf *buf);
bool		expand_buf_append(t_expand_buf *buf, const char *str, size_t len,
				bool glob);
void		fields_init(t_fields *fields);
t_word_list	*field_insert(t_expand *exp, t_word_list **link,
				const char *s, size_t len);
t_status	fields_emit(t_expand *exp, t_fields *fields);
t_status	expand_word(t_expand *exp, t_word *wd, t_fields *fields);
char		*expand_param(t_ctx *ctx, t_arena *arena, t_param *param);
t_word_list	*expand_glob(t_expand *exp);

#endif
