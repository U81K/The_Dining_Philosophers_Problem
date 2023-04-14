/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgannoun <bgannoun@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 17:37:58 by bgannoun          #+#    #+#             */
/*   Updated: 2023/04/14 21:14:02 by bgannoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int check_if_nb(char *str)
{
	int i;

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

int args_checker(int ac, char **av, t_global *ph)
{
	int i;

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

unsigned long time_cal(void)
{
	unsigned long i;
	struct timeval current_time;

	gettimeofday(&current_time, NULL);
	i = (current_time.tv_sec * 1000) + (current_time.tv_usec / 1000);
	return (i);
}

void *routine(void *arg)
{
	t_ph *data;

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
		pthread_mutex_unlock(data->mutex_shared);
		pthread_mutex_lock(data->left_fork);
		printf("%ld %d has taken a fork\n", (time_cal() - data->start), data->index);
		pthread_mutex_lock(data->right_fork);
		printf("%ld %d has taken a fork\n", (time_cal() - data->start), data->index);
		data->last_meal = time_cal();
		printf("%ld %d is eating\n", (time_cal() - data->start), data->index);
		// usleep(data->tte * 1000);
		data->tmp = time_cal();
		data->n_each_ph_me--;
		while (time_cal() - data->tmp < (unsigned long)data->tte)
			usleep(200);
		pthread_mutex_unlock(data->right_fork);
		pthread_mutex_unlock(data->left_fork);
		printf("%ld %d is sleeping\n", (time_cal() - data->start), data->index);
		// usleep(data->tts * 1000);
		data->tmp2 = time_cal();
		while (time_cal() - data->tmp2 < (unsigned long)data->tts)
			usleep(200);
	}
	return (NULL);
}

int stop_check(t_ph *phs)
{
	int i;

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

int check_if_dead(t_ph *phs)
{
	int i;
	int n_ph;

	n_ph = phs[0].n_ph;
	i = 0;
	usleep(100000);
	while (1)
	{
		if (i == n_ph)
			i = 0;
		if (stop_check(phs) == 1)
			return 0;
		usleep(10000);
		if ((time_cal() - phs[i].last_meal) >= (unsigned long)phs[i].ttd)
		{
			*phs[i].stop = 0;
			printf("%ld %d died\n", (time_cal() - phs[i].start), phs[i].index);
			break;
		}
		i++;
	}
	return (0);
}

int	create_threads(t_global *glo, int *stop)
{
	int i;

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

int mutex_init(t_global *ph)
{
	int i;

	ph->fork_locks = malloc(sizeof(pthread_mutex_t) * ph->n_ph);
	if (!ph->fork_locks)
		return (2);
	i = 0;
	while (i < ph->n_ph)
		pthread_mutex_init(&ph->fork_locks[i++], NULL);
	return (0);
}

int main(int ac, char **av)
{
	t_global glo;
	int *stop;

	if (ac < 5 || ac > 6 || args_checker(ac, av, &glo) == 2)
	{
		ft_putstr_fd("invalide args\n", 2);
		return (2);
	}
	stop = malloc(sizeof(int));
	*stop = 1;
	if (mutex_init(&glo) == 2)
		ft_putstr_fd("malloc return NULL\n", 2);
	if (create_threads(&glo, stop) == 2)
		ft_putstr_fd("pthread_create return error\n", 2);
	check_if_dead(glo.phs);
	// int			i;
	// i = 0;
	// while (i < glo.n_ph)
	// {
	// 	pthread_join(glo.th[i], NULL);
	// 	// printf("thread : %d has finished\n", i);
	// 	i++;
	// }
	return (0);
}
