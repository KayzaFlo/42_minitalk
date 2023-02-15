/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgeslin <fgeslin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 12:57:33 by fgeslin           #+#    #+#             */
/*   Updated: 2023/02/15 17:31:55 by fgeslin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

char	*ft_strjoin_free(char *s1, char const *s2)
{
	char	*str;
	int		s1_size;
	int		s2_size;

	if (!s1 || !s2)
		return (NULL);
	s1_size = ft_strlen(s1);
	s2_size = ft_strlen(s2);
	str = malloc((s1_size + s2_size + 1) * sizeof(char));
	if (str == NULL)
		return (NULL);
	ft_strlcpy(str, s1, s1_size + 1);
	ft_strlcpy(str + s1_size, s2, s1_size + s2_size + 1);
	free(s1);
	return (str);
}

static void	fill_byte(char **str, char *byte, pid_t pid)
{
	*str = ft_strjoin_free(*str, byte);
	if (!*byte && **str)
	{
		kill(pid, SIGUSR1);
		ft_printf("%s\n", *str);
		free (*str);
		*str = "";
		exit (0);
	}
	*byte = 0;
}

static void	handler_byte(int sig, siginfo_t *info, void *ucontext)
{
	static char		byte = 0;
	static int		count = 0;
	static char		*str = "";
	static pid_t	client_pid = 0;

	(void)ucontext;
	if (info->si_pid)
		client_pid = info->si_pid;
	if (client_pid == getpid() || !client_pid)
	{
		printf(KRED "🔴 Error:" KWHT " Infinite PID call\n");
		exit (-1);
	}
	byte <<= 1;
	byte |= (sig == SIGUSR1);
	count = (count + 1) % 8;
	if (!count)
		fill_byte(&str, &byte, client_pid);
	kill(client_pid, SIGUSR2);
}

static void	init_sigs(struct sigaction *sa, void func(int, siginfo_t*, void*))
{
	sigemptyset(&sa->sa_mask);
	sigaddset(&sa->sa_mask, SIGUSR1);
	sigaddset(&sa->sa_mask, SIGUSR2);
	sa->sa_flags = SA_SIGINFO | SA_RESTART;
	sa->sa_sigaction = func;
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
	printf(KYEL KBLD "pid:" KWHT " %d" KNRM "\n", getpid());
	init_sigs(&sa, handler_byte);
	while (1)
		pause();
	return (0);
}
