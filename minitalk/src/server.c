/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 15:25:37 by fendo             #+#    #+#             */
/*   Updated: 2025/12/02 01:15:23 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

t_server	g_server;

static void	set_new_character(void)
{
	g_server.c = 0;
	g_server.bit_count = 0;
}

static void	reset_server_state(void)
{
	set_new_character();
	g_server.client_pid = 0;
}

static int	handle_new_client(siginfo_t *info)
{
	if (g_server.client_pid == 0)
		g_server.client_pid = info->si_pid;
	else if (g_server.client_pid != info->si_pid)
	{
		if (kill(g_server.client_pid, 0) == -1 && errno == ESRCH)
		{
			reset_server_state();
			g_server.client_pid = info->si_pid;
		}
		else
			return (-1);
	}
	return (0);
}

static void	handle_signal(int signum, siginfo_t *info, void *context)
{
	(void)context;
	if (handle_new_client(info) == -1)
		return ;
	if (signum == SIGUSR2)
		g_server.c |= (1 << (CHAR_BIT - 1 - g_server.bit_count));
	g_server.bit_count++;
	if (g_server.bit_count == CHAR_BIT)
	{
		if (g_server.c == '\0')
		{
			write(1, "\n", 1);
			reset_server_state();
		}
		else
		{
			write(1, &g_server.c, 1);
			set_new_character();
		}
	}
	if (kill(info->si_pid, SIGUSR1) == -1)
		reset_server_state();
}

int	main(void)
{
	struct sigaction	sa;

	reset_server_state();
	ft_printf("Server PID: %d\n", getpid());
	ft_memset(&sa, 0, sizeof(sa));
	if (sigemptyset(&sa.sa_mask) == -1)
		error_exit("Error: Failed to setup signal handler");
	if (sigaddset(&sa.sa_mask, SIGUSR1) == -1)
		error_exit("Error: Failed to setup signal handler");
	if (sigaddset(&sa.sa_mask, SIGUSR2) == -1)
		error_exit("Error: Failed to setup signal handler");
	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = handle_signal;
	if (sigaction(SIGUSR1, &sa, NULL) == -1
		|| sigaction(SIGUSR2, &sa, NULL) == -1)
		error_exit("Error: Failed to setup signal handler");
	while (1)
	{
		if (pause() == -1 && errno != EINTR)
			error_exit("Error: Failed to pause");
	}
	return (0);
}
