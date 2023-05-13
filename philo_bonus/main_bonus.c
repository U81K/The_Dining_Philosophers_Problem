/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgannoun <bgannoun@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 14:37:15 by bgannoun          #+#    #+#             */
/*   Updated: 2023/05/13 21:57:34 by bgannoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	routine_1(t_ph *data)
{
	printf("%ld %d is eating\n", (time_cal() - data->start), data->index);
	sem_post(data->print);
	usleep(data->tte * 1000);
	if (data->n_each_ph_me-- == 0)
		exit(0);
	sem_post(data->sem);
	sem_post(data->sem);
	sem_wait(data->print);
	printf("%ld %d is sleeping\n", (time_cal() - data->start), data->index);
	sem_post(data->print);
	usleep(data->tts * 1000);
}

void	routine(t_ph *data)
{
	while (1)
	{
		sem_wait(data->print);
		printf("%ld %d is thinking\n", (time_cal() - data->start), data->index);
		sem_post(data->print);
		sem_wait(data->sem);
		sem_wait(data->print);
		printf("%ld %d has taken a fork\n", (time_cal() - data->start),
			data->index);
		sem_post(data->print);
		sem_wait(data->sem);
		sem_wait(data->print);
		printf("%ld %d has taken a fork\n", (time_cal() - data->start),
			data->index);
		sem_post(data->print);
		data->last_meal = time_cal();
		sem_wait(data->print);
		routine_1(data);
	}
}

void	*thr_routine(void *args)
{
	t_ph	*data;
	int		i;

	data = (t_ph *)args;
	i = 0;
	usleep(1000 * data->ttd);
	while (1)
	{
		if (i == data->n_ph)
			i = 0;
		sem_wait(data->print);
		if ((time_cal() - data->last_meal) >= (unsigned long)data->ttd)
		{
			sem_wait(data->dead);
			printf("%ld %d died\n", (time_cal() - data->start), data->index);
			exit(0);
		}
		sem_post(data->print);
		i++;
	}
	return (NULL);
}

int	main(int ac, char **av)
{
	t_global	glo;

	if (ac < 5 || ac > 6 || args_checker(ac, av, &glo) == 2)
	{
		ft_putstr_fd("invalide args\n", 2);
		return (2);
	}
	glo.phs = malloc(sizeof(t_ph) * glo.n_ph);
	create_proc(&glo);
	free(glo.thr);
	free(glo.phs);
	return (0);
}
