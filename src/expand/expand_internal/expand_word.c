#include "expand_internal.h"

static void	process_part(t_expand_ctx *ex, t_word *part, t_expbuf *buf, int opt)
{
	bool	globbable;

	if (part->flag & W_SQ)
		expbuf_append(buf, part->str, part->len, false, false, ex->tmp);
	else if (part->flag & W_DOLL)
	{
		if (opt & EXP_PARAM)
			expand_param(ex, part, buf);
		else
			expbuf_append(buf, part->str, part->len, false, false, ex->tmp);
	}
	else if (part->flag & W_WILD)
	{
		globbable = (opt & EXP_GLOB) != 0;
		expbuf_append(buf, part->str, part->len, false, globbable, ex->tmp);
	}
	else if (part->flag & W_DQ)
		expbuf_append(buf, part->str, part->len, false, false, ex->tmp);
	else
		expbuf_append(buf, part->str, part->len, false, false, ex->tmp);
}

static bool	buf_has_glob(t_expbuf *buf)
{
	int	i;

	i = 0;
	while (i < buf->len)
	{
		if (buf->gmap[i] && buf->data[i] == '*')
			return (true);
		i++;
	}
	return (false);
}

static t_word_list	*apply_glob_to_fields(t_expand_ctx *ex, char **fields,
										int count, bool do_glob)
{
	t_word_list	*head;
	t_word_list	*tail;
	t_word_list	*nodes;
	char		**expanded;
	int			exp_count;
	int			i;

	head = NULL;
	tail = NULL;
	i = 0;
	while (i < count)
	{
		if (do_glob && pattern_has_glob(fields[i]))
			expanded = expand_glob(ex, fields[i], &exp_count);
		else
		{
			expanded = ft_arena_alloc(ex->ast, sizeof(char *));
			expanded[0] = ft_arena_strdup(ex->ast, fields[i]);
			exp_count = 1;
		}
		nodes = fields_to_wordlist(ex, expanded, exp_count);
		if (nodes)
		{
			if (!head)
				head = nodes;
			else
				tail->next = nodes;
			while (nodes->next)
				nodes = nodes->next;
			tail = nodes;
		}
		i++;
	}
	return (head);
}

/*
** Expand a single word to a list of words (may be 0, 1, or more).
** Returns NULL if the word should be deleted (empty after expansion).
** The opts parameter controls which expansion steps to apply.
*/
t_word_list	*expand_word(t_expand_ctx *ex, t_word *wd, int opts)
{
	t_expbuf	buf;
	t_word		*part;
	char		**fields;
	int			count;
	bool		keep_empty;
	bool		do_glob;

	if (!expbuf_init(&buf, ex->tmp))
		return (NULL);
	keep_empty = word_has_quote(wd);
	do_glob = (opts & EXP_GLOB) && buf_has_glob(&buf);
	part = wd;
	while (part)
	{
		process_part(ex, part, &buf, opts);
		part = part->next;
	}
	do_glob = (opts & EXP_GLOB) && buf_has_glob(&buf);
	if (opts & EXP_SPLIT)
		fields = split_fields(ex, &buf, &count);
	else
	{
		fields = ft_arena_alloc(ex->ast, sizeof(char *));
		fields[0] = expbuf_finish(&buf, ex->ast);
		count = (buf.len > 0 || keep_empty) ? 1 : 0;
	}
	if (count == 0 && !keep_empty)
		return (NULL);
	if (count == 0 && keep_empty)
	{
		fields = ft_arena_alloc(ex->ast, sizeof(char *));
		fields[0] = ft_arena_strdup(ex->ast, "");
		count = 1;
		return (fields_to_wordlist(ex, fields, count));
	}
	return (apply_glob_to_fields(ex, fields, count, do_glob));
}
