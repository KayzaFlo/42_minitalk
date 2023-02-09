/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgeslin <fgeslin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 12:57:33 by fgeslin           #+#    #+#             */
/*   Updated: 2023/02/09 12:42:25 by fgeslin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

void	*g_input;

static void	handler_uint(int sig, siginfo_t *info, void *ucontext)
{
	static uint32_t	c = 0;
	static int		count = 0;
	static int		i = 0;

	(void)ucontext;
	(void)info;
	c = c << 1;
	if (sig == SIGUSR1)
		c++;
	count = (count + 1) % 32;
	if (!count)
	{
		((uint32_t *)g_input)[i] = c;
		if (!c)
			i = 0;
		else
			i++;
		c = 0;
	}
}

static void	handler_char(int sig, siginfo_t *info, void *ucontext)
{
	static char	c = 0;
	static int	count = 0;
	static int	i = 0;

	(void)ucontext;
	(void)info;
	c = c << 1;
	if (sig == SIGUSR1)
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

static void	init_sigs(struct sigaction *sa, void f(int, siginfo_t, void*))
{
	sigemptyset(&sa->sa_mask);
	sa->sa_flags = SA_SIGINFO;
	sa->sa_sigaction = f;
	if (sigaction(SIGUSR1, sa, NULL) == -1)
		exit (-1);
	if (sigaction(SIGUSR2, sa, NULL) == -1)
		exit (-1);
}

void	waiting(struct sigaction *sa)
{
	pid_t	client_pid;
	int		len;

	init_sigs(sa, handler_uint);
	g_input = calloc(3, sizeof(uint32_t));
	ft_memset(g_input, -1, 3 * sizeof(uint32_t));
	while (((uint32_t *)g_input)[2])
		pause();
	client_pid = ((uint32_t *)g_input)[0];
	len = ((uint32_t *)g_input)[1];
	init_sigs(sa, handler_char);
	g_input = realloc(g_input, (len + 1) * sizeof(char));
	ft_memset(g_input, -1, (len + 1) * sizeof(char));
	while (((char *)g_input)[len])
		pause();
	ft_printf("%s\n", (char *)g_input);
	kill(client_pid, SIGUSR1);
}

int	main(int argc, char const *argv[])
{
	struct sigaction	sa;
	pid_t				pid;

	if (argc != 1 || !argv)
		return (0);
	pid = getpid();
	printf(KYEL KBLD "pid:" KWHT " %d" KNRM "\n", pid);
	while (pid)
	{
		waiting(&sa);
		if (!ft_strncmp((char *)g_input, "quit", 4))
			break ;
		free (g_input);
	}
	free (g_input);
	return (0);
}
