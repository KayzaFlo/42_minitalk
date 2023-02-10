/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgeslin <fgeslin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 12:57:35 by fgeslin           #+#    #+#             */
/*   Updated: 2023/02/10 16:43:01 by fgeslin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

int	g_sig;

void	send_bit(uint32_t c, size_t size, int pid)
{
	if (c >= size)
	{
		kill(pid, SIGUSR1);
		c -= size;
	}
	else
	{
		kill(pid, SIGUSR2);
	}
	usleep(400);
	// while (!g_sig)
	// 	pause();
	// g_sig = 0;
	if (size <= 1)
		return ;
	send_bit(c, size / 2, pid);
}

static void	handler(int signum)
{
	if (signum == SIGUSR1)
		ft_printf(KYEL"-"KNRM"message received"KYEL"-\n"KNRM);
	//if (signum == SIGUSR2)
		g_sig = 1;
}

static void	init_sigs(struct sigaction *sa)
{
	sa->sa_handler = handler;
	sigemptyset(&sa->sa_mask);
	sa->sa_flags = SA_SIGINFO;
	if (sigaction(SIGUSR1, sa, NULL) == -1)
		exit (-1);
	if (sigaction(SIGUSR2, sa, NULL) == -1)
		exit (-1);
}

int	main(int argc, char const *argv[])
{
	struct sigaction	sa;
	pid_t				server_pid;
	char				*to_send;

	if (argc != 3 || !argv)
		return (0);
	server_pid = atoi(argv[1]);
	init_sigs(&sa);
	to_send = (char *)argv[2];
	while (*to_send)
	{
		send_bit((unsigned char)*to_send, 128, server_pid);
		to_send++;
	}
	send_bit('\0', 128, server_pid);
	return (0);
}
