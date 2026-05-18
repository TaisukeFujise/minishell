#include "expand.h"
#include "expand_dump.h"
#include "hashmap.h"
#include "libft.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void	print_status(t_status status)
{
	if (status == ST_OK)
		printf("ST_OK");
	else if (status == ST_FAILURE)
		printf("ST_FAILURE");
	else if (status == ST_FATAL)
		printf("ST_FATAL");
	else if (status == ST_EXIT)
		printf("ST_EXIT");
}

static void	print_escaped(const char *s, size_t len)
{
	size_t	i;

	i = 0;
	while (i < len)
	{
		if (s[i] == '\n')
			printf("\\n");
		else if (s[i] == '\t')
			printf("\\t");
		else if (s[i] == '\\')
			printf("\\\\");
		else if (s[i] == '\"')
			printf("\\\"");
		else
			putchar(s[i]);
		i++;
	}
}

static void	print_flags(uint8_t flag)
{
	int	first;

	first = 1;
	if (flag == W_NONE)
		return ((void)printf("NONE"));
	if ((flag & W_SQ) && !first++)
		printf("|");
	if (flag & W_SQ)
		printf("SQ");
	first = (flag & W_SQ) ? 0 : first;
	if ((flag & W_DQ) && !first)
		printf("|");
	if (flag & W_DQ)
		printf("DQ");
	first = (flag & W_DQ) ? 0 : first;
	if ((flag & W_DOLL) && !first)
		printf("|");
	if (flag & W_DOLL)
		printf("DOLL");
	first = (flag & W_DOLL) ? 0 : first;
	if ((flag & W_WILD) && !first)
		printf("|");
	if (flag & W_WILD)
		printf("WILD");
	first = (flag & W_WILD) ? 0 : first;
	if ((flag & W_ASSIGN) && !first)
		printf("|");
	if (flag & W_ASSIGN)
		printf("ASSIGN");
	first = (flag & W_ASSIGN) ? 0 : first;
	if ((flag & W_APPEND) && !first)
		printf("|");
	if (flag & W_APPEND)
		printf("APPEND");
	first = (flag & W_APPEND) ? 0 : first;
	if ((flag & W_ID) && !first)
		printf("|");
	if (flag & W_ID)
		printf("ID");
}

static void	print_indent(int depth)
{
	while (depth-- > 0)
		printf("  ");
}

static void	dump_word(t_word *word)
{
	printf("\"");
	while (word)
	{
		print_escaped(word->str, (size_t)word->len);
		word = word->next;
	}
	printf("\"");
}

static void	dump_word_parts(t_word *word)
{
	int	i;

	i = 0;
	printf(" parts=[");
	while (word)
	{
		if (i != 0)
			printf(", ");
		printf("{%d:\"", i);
		print_escaped(word->str, (size_t)word->len);
		printf("\" flags=");
		print_flags(word->flag);
		printf("}");
		word = word->next;
		i++;
	}
	printf("]");
}

static const char	*node_name(t_node_kind kind)
{
	if (kind == NODE_COMPLETE)
		return ("COMPLETE");
	if (kind == NODE_ANDOR)
		return ("ANDOR");
	if (kind == NODE_PIPE)
		return ("PIPE");
	if (kind == NODE_SUBSHELL)
		return ("SUBSHELL");
	if (kind == NODE_SIMPLE)
		return ("SIMPLE");
	return ("UNKNOWN");
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

static void	dump_assigns(t_assign *assign, int depth)
{
	int	i;

	i = 0;
	while (assign)
	{
		print_indent(depth);
		printf("assign[%d] key=", i);
		dump_word(assign->key);
		printf(" value=");
		dump_word(assign->value);
		printf(" key_flags=");
		print_flags(assign->key->flag);
		printf("\n");
		assign = assign->next;
		i++;
	}
}

static void	dump_args(t_word_list *arg, int depth)
{
	int	i;

	i = 0;
	while (arg)
	{
		print_indent(depth);
		printf("arg[%d] word=", i);
		dump_word(arg->wd);
		dump_word_parts(arg->wd);
		printf("\n");
		arg = arg->next;
		i++;
	}
}

static void	dump_redirects(t_redirect *redir, int depth)
{
	int	i;

	i = 0;
	while (redir)
	{
		print_indent(depth);
		printf("redir[%d] op=%s io=%d target=", i,
			redir_name(redir->op), redir->io_number);
		dump_word(&redir->target);
		dump_word_parts(&redir->target);
		if (redir->op == REDIR_DLESS)
		{
			printf(" hd=\"");
			if (redir->hd.raw_str.str)
				print_escaped(redir->hd.raw_str.str,
					(size_t)redir->hd.raw_str.len);
			printf("\"");
		}
		printf("\n");
		redir = redir->next;
		i++;
	}
}

static void	dump_node(t_node *node, int depth)
{
	if (!node)
		return ;
	print_indent(depth);
	printf("%s\n", node_name(node->node_kind));
	if (node->node_kind == NODE_SIMPLE)
	{
		dump_assigns(node->u_node.simple_command.assigns, depth + 1);
		dump_args(node->u_node.simple_command.args, depth + 1);
		dump_redirects(node->u_node.simple_command.redirects, depth + 1);
	}
	else if (node->node_kind == NODE_SUBSHELL)
		dump_redirects(node->u_node.subshell.redirects, depth + 1);
	dump_node(node->left, depth + 1);
	dump_node(node->right, depth + 1);
}

static t_status	expand_tree(t_node *node, t_ctx *ctx, t_arenas *arenas)
{
	t_status	status;

	if (!node)
		return (ST_OK);
	status = expand_command(node, ctx, arenas);
	if (status != ST_OK)
		return (status);
	status = expand_tree(node->left, ctx, arenas);
	if (status != ST_OK)
		return (status);
	return (expand_tree(node->right, ctx, arenas));
}

static void	add_env(t_ctx *ctx, const char *key, const char *value)
{
	t_bucket_contents	*entry;

	entry = hash_insert(ft_strdup(key), ctx->env_table);
	if (!entry)
		return ;
	entry->data.value = ft_strdup(value);
	entry->data.exported = true;
}

static void	init_ctx(t_ctx *ctx)
{
	memset(ctx, 0, sizeof(*ctx));
	ctx->env_table = hash_create(BUCKET_SIZE);
	/* Fixed environment for deterministic expansion dump tests. */
	ctx->err.exit_code = 42;
	add_env(ctx, "FOO", "alpha beta");
	add_env(ctx, "BAR", "z");
	add_env(ctx, "EMPTY", "");
	add_env(ctx, "STAR", "*.c");
	add_env(ctx, "HOME", "/home/tester");
}

static void	init_arenas(t_arenas *arenas)
{
	ft_arena_init(&arenas->ast, ARENA_DEFAULT_CHUNK_SIZE);
	ft_arena_init(&arenas->tmp, ARENA_DEFAULT_CHUNK_SIZE);
	ft_arena_init(&arenas->heredoc, ARENA_DEFAULT_CHUNK_SIZE);
}

static void	destroy_arenas(t_arenas *arenas)
{
	ft_arena_destroy(&arenas->tmp);
	ft_arena_destroy(&arenas->heredoc);
	ft_arena_destroy(&arenas->ast);
}

static void	dump_command(char *begin, char *cursor, t_ctx *ctx,
	t_node *ast, t_arenas *arenas)
{
	t_status	status;

	printf("command=\"");
	print_escaped(begin, (size_t)(cursor - begin));
	printf("\"\nBEFORE_EXPAND:\n");
	dump_node(ast, 0);
	status = expand_tree(ast, ctx, arenas);
	printf("EXPAND_STATUS=");
	print_status(status);
	if (status != ST_OK)
		printf(" err=\"%s\"", ctx->err.err_msg);
	printf("\nAFTER_EXPAND:\n");
	dump_node(ast, 0);
}

void	expand_dump_input(char *line)
{
	t_ctx		ctx;
	t_node		ast;
	t_arenas	arenas;
	t_status	status;
	char		*begin;
	char		*cursor;
	int			i;

	init_ctx(&ctx);
	cursor = line;
	i = 0;
	while (*cursor)
	{
		begin = cursor;
		init_arenas(&arenas);
		memset(&ast, 0, sizeof(ast));
		status = parse(&cursor, &ast, &ctx, &arenas);
		printf("[%d] PARSE_STATUS=", i++);
		print_status(status);
		printf("\n");
		if (status == ST_OK)
			dump_command(begin, cursor, &ctx, &ast, &arenas);
		else
			printf("err=\"%s\"\n", ctx.err.err_msg);
		if (status == ST_OK)
			close_heredocs(ast.left);
		destroy_arenas(&arenas);
		if (status != ST_OK)
			break ;
	}
	hash_flush(ctx.env_table, NULL);
	hash_dispose(ctx.env_table);
}
