/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgeslin <fgeslin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 12:57:33 by fgeslin           #+#    #+#             */
/*   Updated: 2023/02/07 14:02:02 by fgeslin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

void	*g_input;

static void	handler(int signum)
{
	static char	c = 0;
	static int	count = 0;
	static int	i = 0;

	c = c << 1;
	if (signum == SIGUSR1)
		c++;
	count = (count + 1) % 8;
	if (!count)
	{
		((char *)g_input)[i] = c;
		if (!c)
			i = 0;
		else
			i++;
		c = 0;
	}
}

static void	init_sigs(struct sigaction *sa)
{
	sa->sa_handler = handler;
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
	int					len;

	if (argc != 1 || !argv)
		return (0);
	init_sigs(&sa);
	pid = getpid();
	printf("pid: %d\n", pid);
	while (pid)
	{
		// Retrieve Len
		g_input = calloc(2, sizeof(char));
		ft_memset(g_input, -1, 2 * sizeof(char));
		while (((char *)g_input)[1])
			pause();
		len = ((char *)g_input)[0];
	// ft_printf("\nlen:%d\n", len);
		free (g_input);
		// Retrieve Str
		g_input = calloc(len + 1, sizeof(char));
		ft_memset(g_input, -1, (len + 1) * sizeof(char));
		while (((char *)g_input)[len])
			pause();
		// End
		ft_printf("%s\n", (char *)g_input);
		if (!ft_strncmp((char *)g_input, "quit", 4))
		{
			free (g_input);
			break;
		}
		free (g_input);
	}
	return (0);
}
