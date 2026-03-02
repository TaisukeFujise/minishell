#include "expand_internal.h"

static bool	is_ifs_char(char c, const char *ifs)
{
	while (*ifs)
	{
		if (*ifs == c)
			return (true);
		ifs++;
	}
	return (false);
}

static int	count_fields(t_expbuf *buf, const char *ifs)
{
	int		count;
	int		i;
	bool	in_field;

	if (!ifs || !*ifs)
		return (1);
	count = 0;
	in_field = false;
	i = 0;
	while (i < buf->len)
	{
		if (buf->smap[i] && is_ifs_char(buf->data[i], ifs))
		{
			if (in_field)
				count++;
			in_field = false;
		}
		else
			in_field = true;
		i++;
	}
	if (in_field)
		count++;
	return (count);
}

static char	*extract_field(t_expand_ctx *ex, t_expbuf *buf, int *pos)
{
	int		start;
	int		end;
	int		i;
	char	*field;

	i = *pos;
	while (i < buf->len && buf->smap[i] && is_ifs_char(buf->data[i], ex->ifs))
		i++;
	start = i;
	while (i < buf->len)
	{
		if (buf->smap[i] && is_ifs_char(buf->data[i], ex->ifs))
			break ;
		i++;
	}
	end = i;
	*pos = i;
	if (start == end)
		return (NULL);
	field = ft_arena_strndup(ex->ast, buf->data + start, end - start);
	return (field);
}

/*
** Split buf->data by IFS characters, but only at positions where smap[i]=true.
** Returns array of field strings allocated in ex->ast.
** Sets *count to the number of fields.
** If IFS is empty or NULL, returns the whole string as one field.
*/
char	**split_fields(t_expand_ctx *ex, t_expbuf *buf, int *count)
{
	char	**fields;
	int		nfields;
	int		pos;
	int		i;

	if (!ex->ifs || !*ex->ifs || buf->len == 0)
	{
		fields = ft_arena_alloc(ex->ast, sizeof(char *));
		if (!fields)
			return (NULL);
		fields[0] = expbuf_finish(buf, ex->ast);
		*count = (buf->len > 0) ? 1 : 0;
		return (fields);
	}
	nfields = count_fields(buf, ex->ifs);
	fields = ft_arena_alloc(ex->ast, sizeof(char *) * (nfields + 1));
	if (!fields)
		return (NULL);
	pos = 0;
	i = 0;
	while (i < nfields)
	{
		fields[i] = extract_field(ex, buf, &pos);
		if (fields[i])
			i++;
		else
			break ;
	}
	*count = i;
	return (fields);
}
