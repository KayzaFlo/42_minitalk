/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgeslin <fgeslin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 12:57:33 by fgeslin           #+#    #+#             */
/*   Updated: 2023/02/10 16:40:34 by fgeslin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

pid_t	cur_client;

static void	handler_byte(int sig, siginfo_t *info, void *ucontext)
{
	static char	c = 0;
	static int	count = 0;
	static char	*str = "";

	(void)ucontext;
	c = c << 1;
	// ft_printf(sig == SIGUSR1 ? "1" : "0");
	if (sig == SIGUSR1)
		c++;
	count = (count + 1) % 8;
	if (!count)
	{
		str = ft_strjoin(str, &c);
		if (!c && *str)
		{
			ft_printf("%s\n", str);
			free (str);
			str = "";
	// usleep(400);
			kill(info->si_pid, SIGUSR1);
			// cur_client = 0;
			// return ;
		}
		c = 0;
	}
	// usleep(400);
	kill(info->si_pid, SIGUSR2);
}

static void	init_sigs(struct sigaction *sa, void f(int, siginfo_t*, void*))
{
	sigemptyset(&sa->sa_mask);
	sa->sa_flags = SA_SIGINFO;
	sa->sa_sigaction = f;
	if (sigaction(SIGUSR1, sa, NULL) == -1)
		exit (-1);
	if (sigaction(SIGUSR2, sa, NULL) == -1)
		exit (-1);
}

int	main(int argc, char const *argv[])
{
	struct sigaction	sa;

	if (argc != 1 || !argv)
		return (0);
	// cur_client = 0;
	printf(KYEL KBLD "pid:" KWHT " %d" KNRM "\n", getpid());
	init_sigs(&sa, handler_byte);
	while (1)
		pause();
	return (0);
}
