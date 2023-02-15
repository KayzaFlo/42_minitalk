/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgeslin <fgeslin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 12:57:35 by fgeslin           #+#    #+#             */
/*   Updated: 2023/02/15 17:08:08 by fgeslin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

static void	ft_kill(int server_pid, char *str)
{
	char	c;
	int		i;

	if (!str)
		return ;
	while (*str)
	{
		i = 8;
		c = *str++;
		while (i--)
		{
			if (c >> i & 1)
				kill(server_pid, SIGUSR1);
			else
				kill(server_pid, SIGUSR2);
			if (!usleep(400))
				printf(KYEL "🟡 Warning:" KWHT " No back sig received\n");
		}
	}
	while (++i != 8)
	{
		kill(server_pid, SIGUSR2);
		if (!usleep(400))
			printf(KYEL "🟡 Warning:" KWHT " No back sig received\n");
	}
}

static void	handler(int signum)
{
	if (signum == SIGUSR1)
	{
		printf(KGRN "🟢 Message received\n" KNRM);
		exit (0);
	}
}

static void	init_sigs(struct sigaction *sa)
{
	sigemptyset(&sa->sa_mask);
	sigaddset(&sa->sa_mask, SIGUSR1);
	sigaddset(&sa->sa_mask, SIGUSR2);
	sa->sa_flags = SA_SIGINFO | SA_RESTART;
	sa->sa_handler = handler;
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
	{
		printf(KYEL "🟡 Usage:" KWHT KBLD " ./client " KNRM KUND "server_pid"
			KNRM " " KUND "message" KNRM "\n");
		return (0);
	}
	server_pid = atoi(argv[1]);
	init_sigs(&sa);
	to_send = (char *)argv[2];
	ft_kill(server_pid, to_send);
	if (!usleep(40000))
	{
		printf(KRED "🔴 Error:" KWHT " Timeout, No response from Server "
			"for more than 40ms (is your " KUND "server_pid" KNRM " valid ?)\n");
		exit (-1);
	}
	return (0);
}
