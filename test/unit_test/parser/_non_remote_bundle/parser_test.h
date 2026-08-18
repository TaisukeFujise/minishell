#ifndef PARSER_TEST_H
# define PARSER_TEST_H

# include "parser.h"
# include "libft.h"
# include <stdio.h>

void	parser_mock_set_lines(const char **lines);
void	parser_dump_ast(t_node *node);
char	*parser_dump_ast_to_string(t_node *node);
void	parser_dump_free_string(char *s);
t_status	parser_test_parse(char **cursor, t_node *ast, t_ctx *ctx);
void	parser_test_free(t_node *node);

# define parse(cursor, ast, ctx) parser_test_parse((cursor), (ast), (ctx))
# define free_node(node) parser_test_free((node))

#endif
