/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgeslin <fgeslin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 12:57:35 by fgeslin           #+#    #+#             */
/*   Updated: 2023/02/07 16:59:57 by fgeslin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

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
	usleep(40);
	if (size <= 1)
		return ;
	send_bit(c, size / 2, pid);
}

static void	inputreceived(int signum)
{
	if (signum == SIGUSR1)
		ft_printf(KYEL"-"KNRM"message received"KYEL"-\n"KNRM);
}

static void	init_sigs(struct sigaction *sa)
{
	sa->sa_handler = inputreceived;
	sigemptyset(&sa->sa_mask);
	sa->sa_flags = SA_RESTART;
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
	send_bit(getpid(), 2147483648, server_pid);
	send_bit(ft_strlen(argv[2]), 2147483648, server_pid);
	send_bit(0, 2147483648, server_pid);
	to_send = (char *)argv[2];
	while (*to_send)
	{
		send_bit((unsigned char)*to_send, 128, server_pid);
		to_send++;
	}
	send_bit(*to_send, 128, server_pid);
	return (0);
}
