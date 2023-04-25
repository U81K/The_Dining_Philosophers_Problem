/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgannoun <bgannoun@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 17:37:58 by bgannoun          #+#    #+#             */
/*   Updated: 2023/04/25 12:12:39 by bgannoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	routine_1(t_ph *data)
{
	pthread_mutex_unlock(data->mutex_shared);
	pthread_mutex_lock(data->left_fork);
	printf("%ld %d has taken a fork\n", (time_cal() - data->start), data->index);
	pthread_mutex_lock(data->right_fork);
	printf("%ld %d has taken a fork\n", (time_cal() - data->start), data->index);
	data->last_meal = time_cal();
	printf("%ld %d is eating\n", (time_cal() - data->start), data->index);
	data->tmp = time_cal();
	data->n_each_ph_me--;
	while (time_cal() - data->tmp < (unsigned long)data->tte)
		usleep(200);
	pthread_mutex_unlock(data->right_fork);
	pthread_mutex_unlock(data->left_fork);
}

void	*routine(void *arg)
{
	t_ph	*data;

	data = (t_ph *)arg;
	data->last_meal = data->start;
	if (data->index % 2 != 0)
		usleep(100);
	while (1)
	{
		printf("%ld %d is thinking\n", (time_cal() - data->start), data->index);
		pthread_mutex_lock(data->mutex_shared);
		if (data->n_each_ph_me == 0 || *data->stop == 0)
		{
			*data->stop = 0;
			return (NULL);
		}
		routine_1(data);
		printf("%ld %d is sleeping\n", (time_cal() - data->start), data->index);
		data->tmp2 = time_cal();
		while (time_cal() - data->tmp2 < (unsigned long)data->tts)
			usleep(200);
	}
	return (NULL);
}

int	create_threads(t_global *glo, int *stop)
{
	int	i;

	glo->mutex_shared = malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(glo->mutex_shared, NULL);
	glo->th = malloc(sizeof(pthread_t) * glo->n_ph);
	i = 0;
	glo->phs = malloc(sizeof(t_ph) * glo->n_ph);
	while (i < glo->n_ph)
	{
		glo->phs[i].index = i;
		glo->phs[i].left_fork = &glo->fork_locks[i];
		glo->phs[i].right_fork = &glo->fork_locks[(i + 1) % glo->n_ph];
		glo->phs[i].n_each_ph_me = glo->n_each_ph_me + 2;
		glo->phs[i].n_ph = glo->n_ph;
		glo->phs[i].ttd = glo->ttd;
		glo->phs[i].tte = glo->tte;
		glo->phs[i].tts = glo->tts;
		glo->phs[i].stop = stop;
		glo->phs[i].start = time_cal();
		glo->phs[i].mutex_shared = glo->mutex_shared;
		if (pthread_create(&glo->th[i], NULL, &routine, &glo->phs[i]) != 0)
			return (2);
		i++;
	}
	return (0);
}

int	mutex_init(t_global *ph)
{
	int	i;

	ph->fork_locks = malloc(sizeof(pthread_mutex_t) * ph->n_ph);
	if (!ph->fork_locks)
		return (2);
	i = 0;
	while (i < ph->n_ph)
		pthread_mutex_init(&ph->fork_locks[i++], NULL);
	return (0);
}

int	main(int ac, char **av)
{
	t_global	glo;
	int			*stop;

	if (ac < 5 || ac > 6 || args_checker(ac, av, &glo) == 2)
	{
		ft_putstr_fd("invalide args\n", 2);
		return (2);
	}
	stop = malloc(sizeof(int));
	*stop = 1;
	if (mutex_init(&glo) == 2 || create_threads(&glo, stop) == 2)
	{
		ft_putstr_fd("Error\n", 2);
		return (2);
	}
	check_if_dead(glo.phs);
	destroy_mutex(&glo);
	return (0);
}
