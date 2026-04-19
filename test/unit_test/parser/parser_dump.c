#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct s_buffer
{
	char	*data;
	size_t	len;
	size_t	cap;
}	t_buffer;

static int	buf_grow(t_buffer *buf, size_t add_len)
{
	char	*new_data;
	size_t	new_cap;
	size_t	need_cap;

	need_cap = buf->len + add_len + 1;
	if (need_cap <= buf->cap)
		return (1);
	new_cap = buf->cap;
	while (new_cap < need_cap)
		new_cap *= 2;
	new_data = realloc(buf->data, new_cap);
	if (!new_data)
		return (0);
	buf->data = new_data;
	buf->cap = new_cap;
	return (1);
}

static int	buf_append_mem(t_buffer *buf, const char *str, size_t len)
{
	if (!buf_grow(buf, len))
		return (0);
	memcpy(buf->data + buf->len, str, len);
	buf->len += len;
	buf->data[buf->len] = '\0';
	return (1);
}

static int	buf_append_str(t_buffer *buf, const char *str)
{
	return (buf_append_mem(buf, str, strlen(str)));
}

static int	buf_append_char(t_buffer *buf, char c)
{
	return (buf_append_mem(buf, &c, 1));
}

static int	buf_append_int(t_buffer *buf, int n)
{
	char	num[12];
	int		len;

	len = snprintf(num, sizeof(num), "%d", n);
	if (len < 0 || (size_t)len >= sizeof(num))
		return (0);
	return (buf_append_mem(buf, num, (size_t)len));
}

static int	buf_init(t_buffer *buf)
{
	buf->cap = 256;
	buf->len = 0;
	buf->data = calloc(buf->cap, sizeof(char));
	if (!buf->data)
		return (0);
	return (1);
}

static int	buf_append_escaped_bytes(t_buffer *buf, const char *str, size_t len)
{
	size_t	i;

	i = 0;
	while (i < len)
	{
		if (str[i] == '\n' && !buf_append_str(buf, "\\n"))
			return (0);
		else if (str[i] == '\t' && !buf_append_str(buf, "\\t"))
			return (0);
		else if (str[i] == '\"' && !buf_append_str(buf, "\\\""))
			return (0);
		else if (str[i] == '\\' && !buf_append_str(buf, "\\\\"))
			return (0);
		else if (str[i] != '\n' && str[i] != '\t'
			&& str[i] != '\"' && str[i] != '\\')
		{
			if (!buf_append_char(buf, str[i]))
				return (0);
		}
		i++;
	}
	return (1);
}

static int	buf_append_escaped_word(t_buffer *buf, t_word *word)
{
	if (!buf_append_char(buf, '\"'))
		return (0);
	while (word)
	{
		if (!buf_append_escaped_bytes(buf, word->str, word->len))
			return (0);
		word = word->next;
	}
	return (buf_append_char(buf, '\"'));
}

static const char	*node_kind_name(t_node_kind kind)
{
	if (kind == NODE_COMPLETE)
		return ("NODE_COMPLETE");
	if (kind == NODE_ANDOR)
		return ("NODE_ANDOR");
	if (kind == NODE_PIPE)
		return ("NODE_PIPE");
	if (kind == NODE_SUBSHELL)
		return ("NODE_SUBSHELL");
	if (kind == NODE_SIMPLE)
		return ("NODE_SIMPLE");
	return ("NODE_UNKNOWN");
}

static const char	*connect_name(t_op_connect op)
{
	if (op == CONNECT_AND_IF)
		return ("&&");
	if (op == CONNECT_OR_IF)
		return ("||");
	if (op == CONNECT_PIPE)
		return ("|");
	return ("none");
}

static const char	*redir_name(t_op_redir op)
{
	if (op == REDIR_LESS)
		return ("<");
	if (op == REDIR_GREATER)
		return (">");
	if (op == REDIR_DLESS)
		return ("<<");
	if (op == REDIR_DGREATER)
		return (">>");
	return ("?");
}

static int	count_assigns(t_assign *assigns)
{
	int	count;

	count = 0;
	while (assigns)
	{
		count++;
		assigns = assigns->next;
	}
	return (count);
}

static int	count_args(t_word_list *args)
{
	int	count;

	count = 0;
	while (args)
	{
		count++;
		args = args->next;
	}
	return (count);
}

static int	count_redirects(t_redirect *redirects)
{
	int	count;

	count = 0;
	while (redirects)
	{
		count++;
		redirects = redirects->next;
	}
	return (count);
}

static int	buf_append_flags(t_buffer *buf, uint8_t flag)
{
	int	first;

	first = 1;
	if (flag == W_NONE)
		return (buf_append_str(buf, "NONE"));
	if ((flag & W_SQ) && !first && !buf_append_char(buf, '|'))
		return (0);
	if ((flag & W_SQ) && !buf_append_str(buf, "SQ"))
		return (0);
	first = (flag & W_SQ) ? 0 : first;
	if ((flag & W_DQ) && !first && !buf_append_char(buf, '|'))
		return (0);
	if ((flag & W_DQ) && !buf_append_str(buf, "DQ"))
		return (0);
	first = (flag & W_DQ) ? 0 : first;
	if ((flag & W_DOLL) && !first && !buf_append_char(buf, '|'))
		return (0);
	if ((flag & W_DOLL) && !buf_append_str(buf, "DOLL"))
		return (0);
	first = (flag & W_DOLL) ? 0 : first;
	if ((flag & W_WILD) && !first && !buf_append_char(buf, '|'))
		return (0);
	if ((flag & W_WILD) && !buf_append_str(buf, "WILD"))
		return (0);
	first = (flag & W_WILD) ? 0 : first;
	if ((flag & W_ASSIGN) && !first && !buf_append_char(buf, '|'))
		return (0);
	if ((flag & W_ASSIGN) && !buf_append_str(buf, "ASSIGN"))
		return (0);
	first = (flag & W_ASSIGN) ? 0 : first;
	if ((flag & W_APPEND) && !first && !buf_append_char(buf, '|'))
		return (0);
	if ((flag & W_APPEND) && !buf_append_str(buf, "APPEND"))
		return (0);
	first = (flag & W_APPEND) ? 0 : first;
	if ((flag & W_ID) && !first && !buf_append_char(buf, '|'))
		return (0);
	if ((flag & W_ID) && !buf_append_str(buf, "ID"))
		return (0);
	return (1);
}

static int	buf_append_node_header(t_buffer *buf, t_node *node)
{
	if (!buf_append_str(buf, node_kind_name(node->node_kind)))
		return (0);
	if (node->node_kind == NODE_ANDOR || node->node_kind == NODE_PIPE)
	{
		if (!buf_append_str(buf, " op="))
			return (0);
		if (!buf_append_str(buf, connect_name(node->u_node.and_or.op)))
			return (0);
	}
	else if (node->node_kind == NODE_SUBSHELL)
	{
		if (!buf_append_str(buf, " redirects="))
			return (0);
		if (!buf_append_int(buf, count_redirects(node->u_node.subshell.redirects)))
			return (0);
	}
	else if (node->node_kind == NODE_SIMPLE)
	{
		if (!buf_append_str(buf, " assigns="))
			return (0);
		if (!buf_append_int(buf, count_assigns(node->u_node.simple_command.assigns)))
			return (0);
		if (!buf_append_str(buf, " args="))
			return (0);
		if (!buf_append_int(buf, count_args(node->u_node.simple_command.args)))
			return (0);
		if (!buf_append_str(buf, " redirects="))
			return (0);
		if (!buf_append_int(buf, count_redirects(node->u_node.simple_command.redirects)))
			return (0);
	}
	return (1);
}

static int	buf_append_line_begin(t_buffer *buf, const char *prefix, int is_last)
{
	if (!buf_append_str(buf, prefix))
		return (0);
	if (is_last)
		return (buf_append_str(buf, "└─ "));
	return (buf_append_str(buf, "├─ "));
}

static char	*make_child_prefix(const char *prefix, int is_last)
{
	char		*next_prefix;
	const char	*pad;
	size_t		len;
	size_t		prefix_len;
	size_t		pad_len;

	if (is_last)
		pad = "   ";
	else
		pad = "│  ";
	prefix_len = strlen(prefix);
	pad_len = strlen(pad);
	len = prefix_len + pad_len;
	next_prefix = calloc(len + 1, sizeof(char));
	if (!next_prefix)
		return (NULL);
	memcpy(next_prefix, prefix, prefix_len);
	memcpy(next_prefix + prefix_len, pad, pad_len);
	return (next_prefix);
}

static int	dump_assign_item(t_buffer *buf, const char *prefix, int is_last,
	int idx, t_assign *assign)
{
	if (!buf_append_line_begin(buf, prefix, is_last))
		return (0);
	if (!buf_append_str(buf, "assigns["))
		return (0);
	if (!buf_append_int(buf, idx))
		return (0);
	if (!buf_append_str(buf, "]: key="))
		return (0);
	if (!buf_append_escaped_word(buf, assign->key))
		return (0);
	if (!buf_append_str(buf, " value="))
		return (0);
	if (!buf_append_escaped_word(buf, assign->value))
		return (0);
	if (!buf_append_str(buf, " key_flags="))
		return (0);
	if (!buf_append_flags(buf, assign->key->flag))
		return (0);
	return (buf_append_char(buf, '\n'));
}

static int	dump_arg_item(t_buffer *buf, const char *prefix, int is_last,
	int idx, t_word_list *arg)
{
	if (!buf_append_line_begin(buf, prefix, is_last))
		return (0);
	if (!buf_append_str(buf, "args["))
		return (0);
	if (!buf_append_int(buf, idx))
		return (0);
	if (!buf_append_str(buf, "]: word="))
		return (0);
	if (!buf_append_escaped_word(buf, arg->wd))
		return (0);
	if (!buf_append_str(buf, " flags="))
		return (0);
	if (!buf_append_flags(buf, arg->wd->flag))
		return (0);
	return (buf_append_char(buf, '\n'));
}

static int	dump_redirect_item(t_buffer *buf, const char *prefix, int is_last,
	int idx, t_redirect *redir)
{
	size_t	preview_len;

	preview_len = 24;
	if (!buf_append_line_begin(buf, prefix, is_last))
		return (0);
	if (!buf_append_str(buf, "redirects["))
		return (0);
	if (!buf_append_int(buf, idx))
		return (0);
	if (!buf_append_str(buf, "]: op="))
		return (0);
	if (!buf_append_str(buf, redir_name(redir->op)))
		return (0);
	if (!buf_append_str(buf, " io="))
		return (0);
	if (!buf_append_int(buf, redir->io_number))
		return (0);
	if (!buf_append_str(buf, " target="))
		return (0);
	if (!buf_append_escaped_word(buf, &redir->target))
		return (0);
	if (redir->op == REDIR_DLESS)
	{
		if (!buf_append_str(buf, " hd_len="))
			return (0);
		if (!buf_append_int(buf, redir->hd.raw_str.len))
			return (0);
		if (!buf_append_str(buf, " hd_preview=\""))
			return (0);
		if (redir->hd.raw_str.str)
		{
			if ((size_t)redir->hd.raw_str.len < preview_len)
				preview_len = redir->hd.raw_str.len;
			if (!buf_append_escaped_bytes(buf, redir->hd.raw_str.str, preview_len))
				return (0);
			if ((size_t)redir->hd.raw_str.len > preview_len
				&& !buf_append_str(buf, "..."))
				return (0);
		}
		if (!buf_append_char(buf, '\"'))
			return (0);
	}
	return (buf_append_char(buf, '\n'));
}

static int	dump_node_ref(t_buffer *buf, const char *prefix, int is_last,
	const char *edge, t_node *node);

static int	dump_node_children(t_buffer *buf, const char *prefix, t_node *node)
{
	t_assign	*assign;
	t_word_list	*arg;
	t_redirect	*redir;
	int			idx;
	int			total;
	int			assign_idx;
	int			arg_idx;
	int			redir_idx;

	total = 2;
	if (node->node_kind == NODE_SIMPLE)
		total += count_assigns(node->u_node.simple_command.assigns)
			+ count_args(node->u_node.simple_command.args)
			+ count_redirects(node->u_node.simple_command.redirects);
	else if (node->node_kind == NODE_SUBSHELL)
		total += count_redirects(node->u_node.subshell.redirects);
	idx = 0;
	if (!dump_node_ref(buf, prefix, idx == total - 1, "left: ", node->left))
		return (0);
	idx++;
	if (!dump_node_ref(buf, prefix, idx == total - 1, "right: ", node->right))
		return (0);
	idx++;
	assign = NULL;
	arg = NULL;
	redir = NULL;
	assign_idx = 0;
	arg_idx = 0;
	redir_idx = 0;
	if (node->node_kind == NODE_SIMPLE)
	{
		assign = node->u_node.simple_command.assigns;
		arg = node->u_node.simple_command.args;
		redir = node->u_node.simple_command.redirects;
	}
	else if (node->node_kind == NODE_SUBSHELL)
		redir = node->u_node.subshell.redirects;
	while (assign && ++idx)
	{
		if (!dump_assign_item(buf, prefix, idx == total, assign_idx, assign))
			return (0);
		assign_idx++;
		assign = assign->next;
	}
	while (arg && ++idx)
	{
		if (!dump_arg_item(buf, prefix, idx == total, arg_idx, arg))
			return (0);
		arg_idx++;
		arg = arg->next;
	}
	while (redir && ++idx)
	{
		if (!dump_redirect_item(buf, prefix, idx == total, redir_idx, redir))
			return (0);
		redir_idx++;
		redir = redir->next;
	}
	return (1);
}

static int	dump_node_ref(t_buffer *buf, const char *prefix, int is_last,
	const char *edge, t_node *node)
{
	char	*child_prefix;

	if (!buf_append_line_begin(buf, prefix, is_last))
		return (0);
	if (!buf_append_str(buf, edge))
		return (0);
	if (!node)
		return (buf_append_str(buf, "NULL\n"));
	if (!buf_append_node_header(buf, node))
		return (0);
	if (!buf_append_char(buf, '\n'))
		return (0);
	child_prefix = make_child_prefix(prefix, is_last);
	if (!child_prefix)
		return (0);
	if (!dump_node_children(buf, child_prefix, node))
		return (free(child_prefix), 0);
	free(child_prefix);
	return (1);
}

char	*parser_dump_ast_to_string(t_node *node)
{
	t_buffer	buf;

	if (!buf_init(&buf))
		return (NULL);
	if (!node)
	{
		if (!buf_append_str(&buf, "NULL\n"))
			return (free(buf.data), NULL);
		return (buf.data);
	}
	if (!dump_node_ref(&buf, "", 1, "", node))
		return (free(buf.data), NULL);
	return (buf.data);
}

void	parser_dump_ast(t_node *node)
{
	char	*dump;

	dump = parser_dump_ast_to_string(node);
	if (!dump)
		return ;
	printf("%s", dump);
	free(dump);
}
