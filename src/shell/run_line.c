/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_line.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/28 18:20:00 by fendo             #+#    #+#             */
/*   Updated: 2026/08/28 18:20:00 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/execute.h"
#include "../../include/parser.h"
#include "../../include/shell.h"
#include "../../include/signal_handle.h"

void	dispose_shell(char *user_input, t_ctx *ctx)
{
	free(user_input);
	free(ctx->cwd);
	ctx->cwd = NULL;
	if (ctx->env_table != NULL)
	{
		hash_flush(ctx->env_table, NULL);
		hash_dispose(ctx->env_table);
		ctx->env_table = NULL;
	}
	if (ctx->tmp_table != NULL)
	{
		hash_flush(ctx->tmp_table, NULL);
		hash_dispose(ctx->tmp_table);
		ctx->tmp_table = NULL;
	}
	rl_clear_history();
}

/*
	One parse unit: parse it, run what came out, then give back the
	arenas it was built in. The here-documents are closed here too,
	because the tree that names them is about to go.
*/
static t_status	run_command(char **cursor, t_node *ast, t_ctx *ctx,
		t_arenas *arenas)
{
	t_status	status;

	status = parse(cursor, ast, ctx, arenas);
	if (status == ST_OK)
		status = execute(ast, ctx);
	close_heredocs(ast->left);
	ast->left = NULL;
	ctx->arenas = NULL;
	ft_arena_destroy(&arenas->tmp);
	ft_arena_destroy(&arenas->ast);
	return (status);
}

/*
	A line can hold more than one parse unit, so the cursor walks it
	until nothing is left. ST_EXIT and ST_FATAL end the session: the
	first because the shell was asked to leave, the second because it
	cannot go on.
*/
void	parse_and_execute(char *input, t_node *ast, t_ctx *ctx)
{
	char		*cursor;
	t_status	status;
	t_arenas	arenas;

	cursor = input;
	while (*cursor != '\0')
	{
		ft_arena_init(&arenas.ast, ARENA_DEFAULT_CHUNK_SIZE);
		ft_arena_init(&arenas.tmp, ARENA_DEFAULT_CHUNK_SIZE);
		ctx->arenas = &arenas;
		status = run_command(&cursor, ast, ctx, &arenas);
		if (status == ST_EXIT || status == ST_FATAL)
		{
			if (status == ST_FATAL && ctx->err.exit_code == 0)
				ctx->err.exit_code = 1;
			dispose_shell(input, ctx);
			exit(ctx->err.exit_code);
		}
	}
}
