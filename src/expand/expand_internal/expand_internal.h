#ifndef EXPAND_INTERNAL_H
# define EXPAND_INTERNAL_H

# include "expand.h"
# include "strbuf.h"
# include <dirent.h>

# define IFS_DEFAULT " \t\n"
# define EXPAND_MSG_AMBIG_REDIR "ambiguous redirect"

typedef struct s_expand
{
	t_ctx		*ctx;
	t_arenas	*arenas;
	const char	*ifs;
	t_strbuf	buf;
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
t_status	expand_assigns(t_expand *exp, t_assign *assign);
t_status	expand_heredoc_body(t_expand *exp, t_redirect *redir);
void		fields_init(t_fields *fields);
t_word_list	*field_insert(t_expand *exp, t_word_list **link,
				const char *s, size_t len);
t_status	fields_emit(t_expand *exp, t_fields *fields);
t_status	expand_word(t_expand *exp, t_word *wd, t_fields *fields);
char		*expand_param(t_ctx *ctx, t_arena *arena, t_param *param);
t_word_list	*expand_glob(t_expand *exp, const char *pat);

#endif
