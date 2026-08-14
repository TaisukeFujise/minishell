#include "parser_test.h"

static const char	**g_mock_lines = NULL;
static int			g_mock_idx = 0;
static t_arenas		g_parser_arenas;
static int			g_arena_ready = 0;

void	parser_mock_set_lines(const char **lines)
{
	if (g_arena_ready)
	{
		ft_arena_destroy(&g_parser_arenas.tmp);
		ft_arena_destroy(&g_parser_arenas.heredoc);
		ft_arena_destroy(&g_parser_arenas.ast);
	}
	ft_arena_init(&g_parser_arenas.ast, ARENA_DEFAULT_CHUNK_SIZE);
	ft_arena_init(&g_parser_arenas.tmp, ARENA_DEFAULT_CHUNK_SIZE);
	ft_arena_init(&g_parser_arenas.heredoc, ARENA_DEFAULT_CHUNK_SIZE);
	g_arena_ready = 1;
	g_mock_lines = lines;
	g_mock_idx = 0;
}

char	*readline(const char *prompt)
{
	(void)prompt;
	if (!g_mock_lines || !g_mock_lines[g_mock_idx])
		return (NULL);
	return (ft_strdup(g_mock_lines[g_mock_idx++]));
}

#undef parse
#undef free_node

t_status	parser_test_parse(char **cursor, t_node *ast, t_ctx *ctx)
{
	if (!g_arena_ready)
	{
		ft_arena_init(&g_parser_arenas.ast, ARENA_DEFAULT_CHUNK_SIZE);
		ft_arena_init(&g_parser_arenas.tmp, ARENA_DEFAULT_CHUNK_SIZE);
		ft_arena_init(&g_parser_arenas.heredoc, ARENA_DEFAULT_CHUNK_SIZE);
		g_arena_ready = 1;
	}
	return (parse(cursor, ast, ctx, &g_parser_arenas));
}

void	parser_test_free(t_node *node)
{
	close_heredocs(node);
	if (g_arena_ready)
	{
		ft_arena_destroy(&g_parser_arenas.tmp);
		ft_arena_destroy(&g_parser_arenas.heredoc);
		ft_arena_destroy(&g_parser_arenas.ast);
	}
	g_arena_ready = 0;
}
