#ifndef EXPAND_INTERNAL_H
# define EXPAND_INTERNAL_H

# include "expand.h"
# include <dirent.h>

# define IFS_DEFAULT " \t\n"
# define EXP_BUF_INIT 64
# define EXPAND_MSG_AMBIG_REDIR "ambiguous redirect"

/*
	`mask` runs parallel to `data` and lives in the same allocation
	(data[0..cap) then mask[0..cap)), so the two can never fall out of step.
	mask[i] is non-zero when data[i] came from unquoted text and may act as a
	glob operator; a quoted '*' has a zero byte and is matched literally.
*/
typedef struct s_strbuf
{
	char		*data;
	char		*mask;
	size_t		len;
	size_t		cap;
}	t_strbuf;

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
bool		strbuf_init(t_strbuf *buf);
void		strbuf_reset(t_strbuf *buf);
bool		strbuf_add(t_strbuf *buf, const char *s, size_t len,
				bool glob);
void		fields_init(t_fields *fields);
t_word_list	*field_insert(t_expand *exp, t_word_list **link,
				const char *s, size_t len);
t_status	fields_emit(t_expand *exp, t_fields *fields);
t_status	expand_word(t_expand *exp, t_word *wd, t_fields *fields);
char		*expand_param(t_ctx *ctx, t_arena *arena, t_param *param);
t_word_list	*expand_glob(t_expand *exp);

#endif
