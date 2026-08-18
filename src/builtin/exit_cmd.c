/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 20:40:04 by tafujise          #+#    #+#             */
/*   Updated: 2026/02/24 23:49:28 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/builtin.h"
#include "../../include/execute.h"
#include "../../include/parser.h"

/*
	The number of "exit n", as bash reads it: the space around it is
	skipped, a sign is allowed, and the whole of the rest has to be
	digits. [bash-5.3 general.c valid_number()]
*/
static bool	parse_exit_status(char *str, long *out)
{
	char	*scan;
	bool	has_digit;

	scan = str;
	while (ft_isspace(*scan))
		scan++;
	if (*scan == '+' || *scan == '-')
		scan++;
	has_digit = false;
	while (ft_isdigit(*scan))
	{
		has_digit = true;
		scan++;
	}
	while (ft_isspace(*scan))
		scan++;
	if (!has_digit || *scan != '\0')
		return (false);
	errno = 0;
	*out = ft_atol(str);
	return (errno != ERANGE);
}

/*
	exit [n]
	The argument is read before it is counted: "exit abc 1" is a numeric
	error, not too many arguments, and leaves the shell with 2. Too many
	arguments is the one case that does not exit at all. With no argument
	the status of the last command stands.
	An interactive shell says that it is leaving before it looks at the
	argument at all, on stderr, so that "exit 0 > file" does not put it
	in the file.
	[bash-5.3 builtins/common.c get_exitstat(), builtins/exit.def]
*/
t_status	exit_cmd(t_word_list *args, t_ctx *ctx)
{
	long	value;

	if (ctx->interactive)
		write_all(STDERR_FILENO, "exit\n", 5);
	if (args == NULL)
		return (ST_EXIT);
	if (!parse_exit_status(args->wd->str, &value))
	{
		print_error_at("exit", args->wd->str, "numeric argument required");
		ctx->err.exit_code = 2;
		return (ST_EXIT);
	}
	if (args->next != NULL)
	{
		print_error("exit", "too many arguments");
		ctx->err.exit_code = 1;
		return (ST_FAILURE);
	}
	ctx->err.exit_code = (int)(((value % 256) + 256) % 256);
	return (ST_EXIT);
}
