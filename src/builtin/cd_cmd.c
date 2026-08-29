/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_cmd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 19:33:34 by tafujise          #+#    #+#             */
/*   Updated: 2026/02/24 23:07:54 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/builtin.h"
#include "../../include/execute.h"
#include "../../include/parser.h"

static t_status	set_var(t_hashtable *table, char *key, char *value);
static t_status	update_pwd(t_ctx *ctx);
static t_status	cd_move(char *target, t_ctx *ctx);

/*
	Where cd is asked to go. Without an operand it is HOME, and an empty
	destination is the directory the shell is in already, which is what
	bash does with cd "" and with an empty HOME. NULL when there is
	nowhere to go; the reason is reported here.
*/
static char	*cd_destination(t_word_list *args, t_ctx *ctx)
{
	char	*target;

	if (args != NULL)
		target = args->wd->str;
	else
	{
		target = env_lookup(ctx->tmp_table, ctx->env_table, "HOME");
		if (target == NULL)
		{
			print_error("cd", "HOME not set");
			return (NULL);
		}
	}
	if (*target == '\0')
		return (".");
	return (target);
}

/*
	The move, and the name the shell will know the place by. "." and
	".." are resolved in the name before chdir() sees it, so "cd
	link/.." goes back to where the name was written and not to the
	physical parent of the link, as bash, dash and zsh all do. The
	shell's own path is the base: a PWD a command overwrote does not
	steer it. [bash-5.3 builtins/cd.def change_to_directory()]
*/
static t_status	cd_move(char *target, t_ctx *ctx)
{
	char	*path;
	int		moved;

	path = NULL;
	if (ctx->cwd != NULL || *target == '/')
	{
		path = path_canon(path_absolute(ctx->cwd, target));
		if (path == NULL)
			return (ST_FATAL);
		moved = chdir(path);
	}
	else
		moved = chdir(target);
	if (moved < 0)
	{
		print_error_at("cd", target, strerror(errno));
		return (free(path), ST_FAILURE);
	}
	if (path != NULL)
	{
		free(ctx->cwd);
		ctx->cwd = path;
	}
	return (ST_OK);
}

/*
	cd [directory]
	"cd" change current working directory, and update the ctx->exit_code.
	If no parameters, change the current working directory to $HOME.
	- 1. no directory operand with no HOME environment
	- 2. no directory operand with HOME environment
	- 3. directory operand begins with a <slash> character
	- 4. the first component of the directory operand is dot or dot-dot
	<reference>
	[ https://pubs.opengroup.org/onlinepubs/9699919799/utilities/cd.html ]
*/
t_status	cd_cmd(t_word_list *args, t_ctx *ctx)
{
	char		*target;
	t_status	status;

	if (args != NULL && args->next != NULL)
	{
		print_error("cd", "too many arguments");
		return (ST_FAILURE);
	}
	target = cd_destination(args, ctx);
	if (target == NULL)
		return (ST_FAILURE);
	status = cd_move(target, ctx);
	if (status != ST_OK)
		return (status);
	return (update_pwd(ctx));
}

/*
	OLDPWD becomes the value PWD had, and empty when it had none, as
	bash leaves it. PWD is the name the move settled on, so it tells
	the place the way the operand did. Neither string is owned here,
	the table copies what it is given. [review D37-24]
*/
static t_status	update_pwd(t_ctx *ctx)
{
	t_status	status;

	status = set_var(ctx->env_table, "OLDPWD",
			env_lookup(ctx->tmp_table, ctx->env_table, "PWD"));
	if (status != ST_OK)
		return (status);
	return (set_var(ctx->env_table, "PWD", ctx->cwd));
}

static t_status	set_var(t_hashtable *table, char *key, char *value)
{
	t_bucket_contents	*item;

	item = hash_insert(key, table);
	if (item == NULL)
		return (ST_FATAL);
	if (!hash_set_value(item, value))
		return (ST_FATAL);
	return (ST_OK);
}
