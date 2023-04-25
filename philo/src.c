/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   src.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgannoun <bgannoun@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 16:31:27 by bgannoun          #+#    #+#             */
/*   Updated: 2023/04/25 12:06:21 by bgannoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
	return (0);
}

unsigned long	time_cal(void)
{
	unsigned long	i;
	struct timeval	current_time;

	gettimeofday(&current_time, NULL);
	i = (current_time.tv_sec * 1000) + (current_time.tv_usec / 1000);
	return (i);
}

int	stop_check(t_ph *phs)
{
	int	i;

	i = 0;
	while (i < phs[i].n_ph)
	{
		if (phs[i].n_each_ph_me == 0)
			i++;
		else
			return (0);
	}
	return (1);
}

int	check_if_dead(t_ph *phs)
{
	int	i;
	int	n_ph;

	n_ph = phs[0].n_ph;
	i = 0;
	usleep(1000);
	while (1)
	{
		if (i == n_ph)
			i = 0;
		if (stop_check(phs) == 1)
			return (0);
		if ((time_cal() - phs[i].last_meal) >= (unsigned long)phs[i].ttd)
		{
			*phs[i].stop = 0;
			printf("%ld %d died\n", (time_cal() - phs[i].start), phs[i].index);
			break ;
		}
		i++;
		usleep(1000);
	}
	return (0);
}
