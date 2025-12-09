/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 15:25:18 by fendo             #+#    #+#             */
/*   Updated: 2025/12/02 01:47:51 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

t_client	g_client;

static void	ack_handler(int signum, siginfo_t *info, void *context)
{
	(void)signum;
	(void)context;
	if (info->si_pid == g_client.server_pid)
		g_client.ack_received = 1;
}

static int	wait_for_ack(unsigned int timeout_limit)
{
	unsigned int	elapsed_time;

	elapsed_time = 0;
	while (elapsed_time < timeout_limit)
	{
		if (g_client.ack_received == 1)
		{
			g_client.ack_received = 0;
			return (1);
		}
		if (usleep(TICK) == -1 && errno != EINTR)
			error_exit("Error: Failed to sleep");
		elapsed_time += TICK;
	}
	return (0);
}

static void	send_signal(pid_t server_pid, int signal_type)
{
	int				retry;
	unsigned int	timeout_limit;

	retry = 0;
	timeout_limit = BASE_TIMEOUT;
	g_client.ack_received = 0;
	while (retry <= MAX_RETRIES)
	{
		if (kill(server_pid, signal_type) == -1)
			handle_kill_error("Signal send");
		if (wait_for_ack(timeout_limit) == 1)
			return ;
		if (timeout_limit > MAX_TIMEOUT / TIMEOUT_MULTIPLIER_NUM
			* TIMEOUT_MULTIPLIER_DEN)
			timeout_limit = MAX_TIMEOUT;
		else
			timeout_limit = (unsigned int)(timeout_limit
					* TIMEOUT_MULTIPLIER_NUM / TIMEOUT_MULTIPLIER_DEN);
		retry++;
	}
	error_exit("Error: Failed to connect to server");
}

static void	send_message(pid_t server_pid, unsigned char *message)
{
	size_t			i;
	size_t			n;
	int				bit;

	i = 0;
	n = ft_strlen((char *)message);
	while (i <= n)
	{
		bit = CHAR_BIT;
		while (bit--)
		{
			if ((message[i] >> bit) & 1)
				send_signal(server_pid, SIGUSR2);
			else
				send_signal(server_pid, SIGUSR1);
		}
		i++;
	}
}

int	main(int argc, char **argv)
{
	struct sigaction	sa;

	if (argc != 3)
		error_exit("Usage: ./client <server_pid> <message>");
	g_client.ack_received = 0;
	g_client.server_pid = parse_pid(argv[1]);
	ft_memset(&sa, 0, sizeof(sa));
	if (kill(g_client.server_pid, 0) == -1)
		handle_kill_error("PID validation");
	if (sigemptyset(&sa.sa_mask) == -1)
		error_exit("Error: Failed to setup signal handler");
	if (sigaddset(&sa.sa_mask, SIGUSR1) == -1)
		error_exit("Error: Failed to setup signal handler");
	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = ack_handler;
	if (sigaction(SIGUSR1, &sa, NULL) == -1)
		error_exit("Error: Failed to setup signal handler");
	send_message(g_client.server_pid, (unsigned char *)argv[2]);
	return (0);
}
