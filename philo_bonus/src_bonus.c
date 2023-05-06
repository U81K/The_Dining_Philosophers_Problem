/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   src_bonus.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgannoun <bgannoun@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 14:37:37 by bgannoun          #+#    #+#             */
/*   Updated: 2023/05/06 11:10:44 by bgannoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	check_if_nb(char *str)
{
	int	i;

	i = 0;
	while (str[i] == ' ' || str[i] == '\t')
		i++;
	if (str[i] == '-')
		return (2);
	else if (str[i] == '+')
		i++;
	while (str[i] != '\0')
	{
		if (str[i] < '0' || str[i] > '9')
			return (2);
		i++;
	}
	return (0);
}

int	args_checker(int ac, char **av, t_global *ph)
{
	int	i;

	i = 1;
	while (i < ac)
	{
		if (check_if_nb(av[i++]) == 2)
			return (2);
	}
	ph->n_ph = ft_atoi(av[1]);
	ph->ttd = ft_atoi(av[2]);
	ph->tte = ft_atoi(av[3]);
	ph->tts = ft_atoi(av[4]);
	if (ac == 6)
		ph->n_each_ph_me = ft_atoi(av[5]);
	else if (ac == 5)
		ph->n_each_ph_me = 2147483646;
	if (ph->n_ph == 0 || ph->n_each_ph_me == 0)
		return (2);
	return (0);
}

int	ft_atoi(const char *str)
{
	long	sign;
	long	res;
	int		i;

	res = 0;
	sign = 1;
	i = 0;
	while (str[i] == ' ' || str[i] == '\t' || str[i] == '\v'
		|| str[i] == '\n' || str[i] == '\r' || str[i] == '\f')
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign *= -1;
		i++;
	}
	while (str[i] && (str[i] >= '0' && str[i] <= '9'))
		res = res * 10 + str[i++] - '0';
	return (res * sign);
}

void	ft_putstr_fd(char *s, int fd)
{
	int	i;

	i = 0;
	while (s[i])
	{
		write(fd, &s[i++], 1);
	}
}
