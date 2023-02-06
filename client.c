/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgeslin <fgeslin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 12:57:35 by fgeslin           #+#    #+#             */
/*   Updated: 2023/02/06 17:38:00 by fgeslin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <unistd.h>
#include <stdio.h>

#include <stdlib.h>

void	send_bit(int c, int b, int pid)
{
	if (c >= b)
	{
		kill(pid, SIGUSR1);
		c -= b;
	printf("1");
	}
	else
	{
		kill(pid, SIGUSR2);
	printf("0");
	}
	usleep(10);
	if (b <= 1)
		return ;
	send_bit(c, b / 2, pid);
}

int	main(int argc, char const *argv[])
{
	pid_t	server_pid;
	pid_t	pid;
	char	*to_send;
	int		len;

	if (argc != 3 || !argv)
		return (0);
	server_pid = atoi(argv[1]);
	pid = getpid();
	len = 0;
	while (argv[2][len])
		len++;
	send_bit(len, 128, server_pid);
	send_bit(0, 128, server_pid);
	to_send = (char *)argv[2];
	while (*to_send)
	{
		send_bit(*to_send, 128, server_pid);
		to_send++;
	}
		send_bit(*to_send, 128, server_pid);
	return (0);
}
