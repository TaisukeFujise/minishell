#include "builtin_test.h"

t_word_list	*make_args(char **strs)
{
	t_word_list	*head;
	t_word_list	**cur;
	int			i;

	if (strs == NULL || strs[0] == NULL)
		return (NULL);
	head = NULL;
	cur = &head;
	i = 0;
	while (strs[i] != NULL)
	{
		*cur = ft_calloc(1, sizeof(t_word_list));
		if (*cur == NULL)
			return (free_args(head), NULL);
		(*cur)->wd = ft_calloc(1, sizeof(t_word));
		if ((*cur)->wd == NULL)
			return (free_args(head), NULL);
		(*cur)->wd->str = ft_strdup(strs[i]);
		if ((*cur)->wd->str == NULL)
			return (free_args(head), NULL);
		(*cur)->wd->len = ft_strlen(strs[i]);
		cur = &(*cur)->next;
		i++;
	}
	return (head);
}

void	free_args(t_word_list *args)
{
	t_word_list	*next;

	while (args)
	{
		next = args->next;
		if (args->wd != NULL)
		{
			free(args->wd->str);
			free(args->wd);
		}
		free(args);
		args = next;
	}
}

t_status	capture(char *buf, size_t size, t_builtin_fn fn,
				t_word_list *args, t_ctx *ctx)
{
	int			fds[2];
	int			saved;
	t_status	st;
	ssize_t		n;

	if (pipe(fds) < 0)
		return (ST_FATAL);
	fflush(stdout);
	saved = dup(STDOUT_FILENO);
	dup2(fds[1], STDOUT_FILENO);
	close(fds[1]);
	st = fn(args, ctx);
	fflush(stdout);
	dup2(saved, STDOUT_FILENO);
	close(saved);
	n = read(fds[0], buf, size - 1);
	buf[n < 0 ? 0 : n] = '\0';
	close(fds[0]);
	return (st);
}

int	setup_ctx(t_ctx *ctx, char **envp)
{
	if (init_ctx(ctx, envp) == FAILURE)
		return (1);
	ctx->bitmap = new_fd_bitmap(FD_BITMAP_SIZE);
	if (ctx->bitmap == NULL)
		return (1);
	return (0);
}

void	teardown_ctx(t_ctx *ctx)
{
	hash_flush(ctx->env_table, NULL);
	hash_dispose(ctx->env_table);
	ctx->env_table = NULL;
	if (ctx->tmp_table != NULL)
	{
		hash_flush(ctx->tmp_table, NULL);
		hash_dispose(ctx->tmp_table);
		ctx->tmp_table = NULL;
	}
	dispose_fd_bitmap(ctx->bitmap);
	ctx->bitmap = NULL;
}
