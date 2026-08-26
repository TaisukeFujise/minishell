/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handle.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 21:36:30 by tafujise          #+#    #+#             */
/*   Updated: 2025/12/19 00:06:08 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/signal_handle.h"

/*
	The one global the subject allows: the number of a signal that
	arrived. Nothing else is stored here and nothing reads through it.
*/

volatile sig_atomic_t	g_signum = 0;

int	handle_readline_signal(void)
{
	if (g_signum == SIGINT)
	{
		write(1, "^C", 2);
		rl_replace_line("", 0);
		rl_done = 1;
	}
	return (0);
}

/*
	A command runs with the dispositions it would have had if the shell
	had not touched them: the shell ignores SIGQUIT and catches SIGINT
	for its prompt, and neither belongs to the command it starts. An
	ignored signal survives execve, so only the child can undo it.
	[review D37-08]
*/
void	reset_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

static void	signal_handler(int signum)
{
	g_signum = signum;
}

/*
	rl_catch_signals: a non-zero value means using the default signal
	handler, so I set 0 to use the original handler.
*/
int	set_signal(void)
{
	struct sigaction	sa_sigint;
	struct sigaction	sa_ignore;

	rl_catch_signals = 0;
	rl_event_hook = handle_readline_signal;
	ft_bzero(&sa_sigint, sizeof(sa_sigint));
	ft_bzero(&sa_ignore, sizeof(sa_ignore));
	if (sigemptyset(&sa_sigint.sa_mask) == -1)
		return (FAILURE);
	if (sigemptyset(&sa_ignore.sa_mask) == -1)
		return (FAILURE);
	sa_sigint.sa_handler = signal_handler;
	sa_ignore.sa_handler = SIG_IGN;
	if (sigaction(SIGINT, &sa_sigint, NULL) == -1)
		return (FAILURE);
	if (sigaction(SIGQUIT, &sa_ignore, NULL) == -1)
		return (FAILURE);
	return (SUCCESS);
}
