#include "expand_internal.h"

static t_status	append_split(t_expand *exp, t_fields *fields,
					char *s, size_t len)
{
	size_t	i;
	size_t	start;

	i = 0;
	while (i < len)
	{
		while (i < len && ft_strchr(exp->ifs, s[i]))
			i++;
		if (i != 0 && fields->buf.len > 0
			&& fields_emit(exp, fields) != ST_OK)
			return (ST_FATAL);
		start = i;
		while (i < len && !ft_strchr(exp->ifs, s[i]))
			i++;
		if (i == start)
			continue ;
		if (!strbuf_add(&fields->buf, s + start, i - start))
			return (ST_FATAL);
		if (ft_memchr(s + start, '*', i - start))
			fields->glob = true;
	}
	return (ST_OK);
}

static char	*part_value(t_expand *exp, t_word *part, t_param *param)
{
	if ((part->flag & W_DOLL) == 0 || (part->flag & W_SQ) != 0)
	{
		param->len = (size_t)part->len;
		return (part->str);
	}
	param->s = part->str;
	param->slen = (size_t)part->len;
	return (expand_param(exp->ctx, &exp->arenas->tmp, param));
}

static t_status	append_part(t_expand *exp, t_word *part,
					t_exp_mode mode, t_fields *fields)
{
	char	*value;
	t_param	param;

	value = part_value(exp, part, &param);
	if (!value)
		return (ST_FATAL);
	if (mode == EXP_FIELDS && (part->flag & W_DOLL)
		&& (part->flag & (W_SQ | W_DQ)) == 0)
		return (append_split(exp, fields, value, param.len));
	if (!strbuf_add(&fields->buf, value, param.len))
		return (ST_FATAL);
	if (mode == EXP_FIELDS && (part->flag & W_WILD) != 0)
		fields->glob = true;
	return (ST_OK);
}

t_status	expand_word(t_expand *exp, t_word *wd, t_exp_mode mode,
				t_fields *fields)
{
	fields->emitted = false;
	fields->keep_empty = false;
	fields->glob = false;
	fields->buf.len = 0;
	fields->buf.data[0] = '\0';
	while (wd)
	{
		if ((wd->flag & (W_SQ | W_DQ)) != 0)
			fields->keep_empty = true;
		if (append_part(exp, wd, mode, fields) != ST_OK)
			return (ST_FATAL);
		wd = wd->next;
	}
	if (mode == EXP_FIELDS && (fields->buf.len > 0
			|| (!fields->emitted && fields->keep_empty)))
		return (fields_emit(exp, fields));
	return (ST_OK);
}

char	*expand_word_str(t_expand *exp, t_word *wd)
{
	t_fields	fields;
	char		*str;

	if (!fields_init(&fields, &exp->arenas->tmp))
		return (NULL);
	if (expand_word(exp, wd, EXP_JOIN, &fields) != ST_OK)
		return (NULL);
	str = ft_arena_strndup(&exp->arenas->ast,
			fields.buf.data, fields.buf.len);
	return (str);
}
