#ifndef EXPAND_INTERNAL_H
# define EXPAND_INTERNAL_H

# include "expand.h"
# include <dirent.h>

# define IFS_DEFAULT " \t\n"
# define EXP_BUF_INIT 64

/*
** Expansion options (bitflags)
*/
typedef enum e_expand_opt
{
	EXP_PARAM = 1 << 0,
	EXP_SPLIT = 1 << 1,
	EXP_GLOB = 1 << 2
}	t_expand_opt;

# define EXP_ARGV   (EXP_PARAM | EXP_SPLIT | EXP_GLOB)
# define EXP_REDIR  (EXP_PARAM)
# define EXP_ASSIGN (EXP_PARAM)

/*
** Expansion context - passed to all internal functions
*/
typedef struct s_expand_ctx
{
	t_arena			*ast;
	t_arena			*tmp;
	t_ctx			*ctx;
	const char		*ifs;
}	t_expand_ctx;

/*
** Dynamic buffer for building expanded strings
*/
typedef struct s_expbuf
{
	char	*data;
	bool	*smap;
	bool	*gmap;
	int		len;
	int		cap;
}	t_expbuf;

/* expand_ctx.c */
void		init_expand_ctx(t_expand_ctx *ex, t_ctx *ctx, t_arenas *arenas);

/* expand_buf.c */
bool		expbuf_init(t_expbuf *buf, t_arena *arena);
bool		expbuf_append(t_expbuf *buf, const char *s, int n,
				bool splittable, bool globbable, t_arena *arena);
char		*expbuf_finish(t_expbuf *buf, t_arena *arena);

/* expand_param.c */
void		expand_param(t_expand_ctx *ex, t_word *part, t_expbuf *buf);

/* expand_word.c */
t_word_list	*expand_word(t_expand_ctx *ex, t_word *wd, int opts);
char		*word_to_str(t_expand_ctx *ex, t_word *wd);
bool		word_has_quote(t_word *wd);

/* expand_split.c */
char		**split_fields(t_expand_ctx *ex, t_expbuf *buf, int *count);

/* expand_glob.c */
char		**expand_glob(t_expand_ctx *ex, const char *pattern, int *count);
bool		pattern_has_glob(const char *s);

/* expand_utils.c */
t_word_list	*make_word_node(t_expand_ctx *ex, const char *s);
t_word_list	*fields_to_wordlist(t_expand_ctx *ex, char **fields, int count);

#endif
