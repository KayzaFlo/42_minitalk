/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgeslin <fgeslin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 12:57:33 by fgeslin           #+#    #+#             */
/*   Updated: 2023/01/31 13:34:04 by fgeslin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <unistd.h>
#include <stdio.h>

void	oi(int signum)
{
	printf("signal received\n");
	if (signum == SIGUSR1)
		printf("SIGUSR1\n");
}

int	main(int argc, char const *argv[])
{
	pid_t	pid;

	if (argc != 1 || !argv)
		return (0);
	signal(SIGUSR1, oi);
	signal(SIGUSR2, oi);
	pid = getpid();
	printf("pid: %d\n", pid);
	kill(pid, SIGUSR1);
	kill(pid, SIGUSR2);
	return (0);
}
