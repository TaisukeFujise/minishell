#include "expand_internal.h"

static t_status	split_boundary(t_expand *exp, t_fields *fields, char delim)
{
	if (delim && ft_strchr(IFS_DEFAULT, delim))
		fields->pending_ws = true;
	else if (delim || fields->pending_ws)
	{
		fields->pending_ws = false;
		if (delim || exp->buf.len > 0 || fields->keep_empty)
			return (fields_emit(exp, fields));
	}
	return (ST_OK);
}

static t_status	append_split(t_expand *exp, t_fields *fields,
					char *s, size_t len)
{
	size_t	i;
	size_t	start;

	i = 0;
	while (i < len)
	{
		start = i;
		while (i < len && !ft_strchr(exp->ifs, s[i]))
			i++;
		if (i > start && split_boundary(exp, fields, 0) != ST_OK)
			return (ST_FATAL);
		if (i > start && !strbuf_add(&exp->buf, s + start, i - start, true))
			return (ST_FATAL);
		if (i > start && ft_memchr(s + start, '*', i - start))
			fields->glob = true;
		if (i < len && split_boundary(exp, fields, s[i++]) != ST_OK)
			return (ST_FATAL);
	}
	return (ST_OK);
}

static char	*part_value(t_expand *exp, t_word *part, t_param *param)
{
	if ((part->flag & W_DOLL) == 0 || (part->flag & W_SQ) != 0)
	{
		param->len = part->len;
		return (part->str);
	}
	param->s = part->str;
	param->slen = part->len;
	return (expand_param(exp->ctx, &exp->arenas->tmp, param));
}

static t_status	append_part(t_expand *exp, t_word *part, t_fields *fields)
{
	char	*value;
	t_param	param;

	value = part_value(exp, part, &param);
	if (!value)
		return (ST_FATAL);
	if (fields && (part->flag & W_DOLL)
		&& (part->flag & (W_SQ | W_DQ)) == 0)
		return (append_split(exp, fields, value, param.len));
	if (fields && split_boundary(exp, fields, 0) != ST_OK)
		return (ST_FATAL);
	if (fields && (part->flag & (W_SQ | W_DQ)) != 0)
		fields->keep_empty = true;
	if (!strbuf_add(&exp->buf, value, param.len,
			(part->flag & W_WILD) != 0))
		return (ST_FATAL);
	if (fields && (part->flag & W_WILD) != 0)
		fields->glob = true;
	return (ST_OK);
}

t_status	expand_word(t_expand *exp, t_word *wd, t_fields *fields)
{
	strbuf_reset(&exp->buf);
	if (fields)
	{
		fields->pending_ws = false;
		fields->keep_empty = false;
		fields->glob = false;
	}
	while (wd)
	{
		if (append_part(exp, wd, fields) != ST_OK)
			return (ST_FATAL);
		wd = wd->next;
	}
	if (fields && split_boundary(exp, fields, 0) != ST_OK)
		return (ST_FATAL);
	if (fields && (exp->buf.len > 0 || fields->keep_empty))
		return (fields_emit(exp, fields));
	return (ST_OK);
}
