#include "parser.h"
#include "libft.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char	*parser_dump_ast_to_string(t_node *node);

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

static void	print_escaped_segment(const char *begin, const char *end)
{
	while (begin < end)
	{
		if (*begin == '\n')
			printf("\\n");
		else if (*begin == '\t')
			printf("\\t");
		else if (*begin == '\\')
			printf("\\\\");
		else if (*begin == '\"')
			printf("\\\"");
		else
			putchar(*begin);
		begin++;
	}
}

static void	dump_input(char *line)
{
	t_ctx		ctx;
	t_node		ast;
	t_arenas	arenas;
	t_status	status;
	char		*begin;
	char		*cursor;
	char		*dump;
	int			i;

	memset(&ctx, 0, sizeof(ctx));
	cursor = line;
	i = 0;
	while (*cursor)
	{
		begin = cursor;
		init_arenas(&arenas);
		status = parse(&cursor, &ast, &ctx, &arenas);
		if (cursor == begin)
		{
			destroy_arenas(&arenas);
			break ;
		}
		if (status == ST_OK && ast.left == NULL)
		{
			destroy_arenas(&arenas);
			continue ;
		}
		printf("[%d] ", i++);
		print_status(status);
		if (status == ST_OK)
		{
			printf(" command=\"");
			print_escaped_segment(begin, cursor);
			printf("\"\n");
			printf("AST:\n");
			dump = parser_dump_ast_to_string(&ast);
			if (!dump)
				printf("<dump-oom>\n");
			else
			{
				printf("%s", dump);
				free(dump);
			}
			close_heredocs(ast.left);
			ast.left = NULL;
		}
		else
		{
			printf(" command=\"");
			print_escaped_segment(begin, cursor);
			printf("\" err=\"%s\"\n", ctx.err.err_msg);
			if (status != ST_FAILURE)
			{
				destroy_arenas(&arenas);
				break ;
			}
		}
		destroy_arenas(&arenas);
	}
}

int	main(void)
{
	char	*line;

	while (1)
	{
		line = readline("parser dump> ");
		if (!line)
			break ;
		if (*line)
			add_history(line);
		dump_input(line);
		free(line);
	}
	return (0);
}
