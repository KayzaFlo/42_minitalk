/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgeslin <fgeslin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 12:57:35 by fgeslin           #+#    #+#             */
/*   Updated: 2023/02/07 14:01:52 by fgeslin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

void	send_bit(unsigned char c, int size, int pid)
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
	usleep(100);
	if (size <= 1)
		return ;
	send_bit(c, size / 2, pid);
}

int	main(int argc, char const *argv[])
{
	pid_t		server_pid;
	// pid_t	pid;
	char		*to_send;
	int			len;
	int			size;

	// char happy[] = { 0xe2, 0x98, 0xba };
	// char happy[] = { -16, -97, -115, -114 };

	// write(1, happy, 4);
	// ft_printf("\n%u\t%x\n", happy[0], argv[2][0]);
	// ft_printf("\n%x\t%x\n", happy[1], argv[2][1]);
	// ft_printf("\n%x\t%x\n", happy[2], argv[2][2]);
	// ft_printf("\n%x\t%x\n", happy[3], argv[2][3]);
	// ft_printf("\n%x\t%x\n", happy[0], argv[2][4]);
	// ft_printf("\n%x\t%x\n", happy[0], argv[2][5]);
	// ft_printf("\n%x\t%x\n", happy[0], argv[2][6]);
	// ft_printf("\n%s\n", argv[2]);

	if (argc != 3 || !argv)
		return (0);
	server_pid = atoi(argv[1]);
	// pid = getpid();
	size = 128;
	len = ft_strlen(argv[2]);					//|
	send_bit(len, size, server_pid);			//|SEND LEN
	send_bit(0, size, server_pid);				// SEND SEP (0)
	to_send = (char *)argv[2];					//|-----
	while (*to_send)							//|
	{											//|
		send_bit(*to_send, size, server_pid);	//|SEND STR
		to_send++;								//|
	}											//|-----
	send_bit(*to_send, size, server_pid);		// SEND SEP (0)
	return (0);
}
