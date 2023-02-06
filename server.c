/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgeslin <fgeslin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 12:57:33 by fgeslin           #+#    #+#             */
/*   Updated: 2023/02/06 17:48:44 by fgeslin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void	*g_input;

void	oi(int signum)
{
	static int	nb = 0;
	static int	count = 0;
	static int	i = 0;

	if (((char *)g_input)[i] <= 0)
		i = 0;
	nb = nb << 1;
	if (signum == SIGUSR1)
		nb++;
	count = (count + 1) % 8;
	if (!count)
	{
		nb += '0';
		write(1, &nb, 1);
		((char *)g_input)[i] = nb;
		nb = 0;
		i++;
	}
}

static void	init_sigs(struct sigaction *sa)
{
	sa->sa_handler = oi;
	sigemptyset(&sa->sa_mask);
	sa->sa_flags = SA_RESTART; /* Restart functions if interrupted by handler */
	if (sigaction(SIGUSR1, sa, NULL) == -1)
		exit (-1);
	if (sigaction(SIGUSR2, sa, NULL) == -1)
		exit (-1);
}

int	main(int argc, char const *argv[])
{
	struct sigaction	sa;
	pid_t				pid;
	int					i;
	int					len;

	if (argc != 1 || !argv)
		return (0);
	init_sigs(&sa);
	pid = getpid();
	printf("pid: %d\n", pid);
	g_input = malloc(2);
	((char *)g_input)[0] = -1;
	((char *)g_input)[1] = -1;
	while (((char *)g_input)[1])
		pause();
	len = (int)((char *)g_input)[1];
	free (g_input);
	g_input = malloc(len + 1);
	i = -1;
	while (++i < len + 1)
		((char *)g_input)[i] = -1;
	while (((char *)g_input)[len])
		pause();
	return (0);
}
