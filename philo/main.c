/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgannoun <bgannoun@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 17:37:58 by bgannoun          #+#    #+#             */
/*   Updated: 2023/04/06 05:49:46 by bgannoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	check_if_nb(char *str)
{
	int	i;

	i = 0;
	while (str[i] == ' ' || str[i] == '\t')
		i++;
	if (str[i] == '-')
		error_mes("invalide args\n");
	else if (str[i] == '+')
		i++;
	while (str[i] != '\0')
	{
		if (str[i] < '0' || str[i] > '9')
			error_mes("invalide args\n");
		i++;
	}
}

int	args_checker(int ac, char **av, t_global *ph)
{
	int	i;

	i = 1;
	while (i < ac)
		check_if_nb(av[i++]);
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

void	*routine(void *arg)
{
	t_ph			*data;
	unsigned long	start;
	int				eat_times;
	
	// pthread_mutex_t thinking;
	// pthread_mutex_t last_meal;
	// pthread_mutex_t eat;
	// pthread_mutex_t stop;
	
	// pthread_mutex_init(&stop, NULL);
	// pthread_mutex_init(&eat, NULL);
	// pthread_mutex_init(&eat_count, NULL);
	
	// pthread_mutex_lock(&eat);
		data = (t_ph *)arg;
		start = time_cal();
		data->start = start;
		eat_times = data->n_each_ph_me;
	// pthread_mutex_unlock(&eat);
	while (1)
	{
		// pthread_mutex_lock(&thinking);
		// data->thinking = time_cal() - start;
		// pthread_mutex_unlock(&thinking);
		// pthread_mutex_lock(&stop);
		if (*data->stop == 0)
			break ;
		// printf("===>%d\n", *data->stop);
		// pthread_mutex_unlock(&stop);
		printf("%ld %d is thinking\n", (time_cal() - start), data->index);
		if (data->n_each_ph_me == 1)
		{
			// pthread_mutex_unlock(data->right_fork);
			// pthread_mutex_unlock(data->left_fork);
			// data->stop = 0;
			break ;
		}
		if (data->index % 2 != 0)
			usleep(100);
			pthread_mutex_lock(data->left_fork);
		printf("%ld %d has taken a fork\n", (time_cal() - start), data->index);
			pthread_mutex_lock(data->right_fork);
		printf("%ld %d has taken a fork\n", (time_cal() - start), data->index);
		// pthread_mutex_lock(&last_meal);
		if (data->i == 0)
			data->last_meal = start;
		else
			data->last_meal = time_cal();
		data->i++;
		// pthread_mutex_unlock(&last_meal);
			// data->first_meal = time_cal() - start;
		// printf("%d ===>time of eating ===>%d\n", data->index, data->first_meal);
		// else
		// {
		// 	data->last_meal = data->first_meal;
		// }
		// printf("%d ===>last_meal ===>%d\n", data->index, data->last_meal);
		printf("%ld %d is eating\n", (time_cal() - start), data->index);
		usleep(data->tte * 1000);
		data->n_each_ph_me--;
			// eat_times--;
			pthread_mutex_unlock(data->right_fork);
			pthread_mutex_unlock(data->left_fork);
		printf("%ld %d is sleeping\n", (time_cal() - start), data->index);
		usleep(data->tts * 1000);
		// printf("%d ===>is thinking ==> %d\n", data->index, data->last_meal);
		// }
		// 	data->last_meal = time_cal() - start;
		// printf("%d ===>last_meal ===>%d\n", data->index, data->last_meal);
	}
	// free(arg);
	return (NULL);
}

int check_if_dead(t_ph *phs)
{
	int		i;
	int		n_ph;
	
	n_ph = phs[0].n_ph;
	i = 0;
	usleep(1000);
	while (1)
	{
		if (i == n_ph)
			i = 0;
			// break ;
		// printf("%d %lu\n", phs[i].index, (time_cal() - phs[i].last_meal));
		// printf("%ld %d died\n", (time_cal() - phs[i].start), phs[i].index);
		if ((time_cal() - phs[i].last_meal) >= (unsigned long)phs[i].ttd)
		{
			printf("%ld %d died\n", (time_cal() - phs[i].start), phs[i].index);
			*phs[i].stop = 0;
			break ;
		}
		i++;
	}
	return (0);
}

void	create_threads(t_global *glo, int *stop)
{
	int	i;

	glo->th = malloc(sizeof(pthread_t) * glo->n_ph);
	if (!glo->th)
		error_mes("malloc retun NULL");
	i = 0;
	glo->phs = malloc(sizeof(t_ph) * glo->n_ph);
	if (glo->n_ph == 1)
	{
		printf("0 0 died\n");
		return ;
	}
	while (i < glo->n_ph)
	{
		glo->phs[i].index = i;
		glo->phs[i].left_fork = &glo->fork_locks[i];
		glo->phs[i].right_fork = &glo->fork_locks[(i + 1) % glo->n_ph];
		glo->phs[i].tte = glo->tte;
		glo->phs[i].tts = glo->tts;
		if (i % 2 == 0)
			glo->phs[i].n_each_ph_me = glo->n_each_ph_me + 1;
		else
			glo->phs[i].n_each_ph_me = glo->n_each_ph_me;
		glo->phs[i].n_ph = glo->n_ph;
		glo->phs[i].ttd = glo->ttd;
		glo->phs[i].is_dead = 1;
		glo->phs[i].stop = stop;
		if (pthread_create(&glo->th[i], NULL, &routine, &glo->phs[i]) != 0)
			error_mes("pthread_create return error\n");
		i++;
	}
	
}

void	mutex_init(t_global *ph)
{
	int				i;

	ph->fork_locks = malloc(sizeof(pthread_mutex_t) * ph->n_ph);
	if (!ph->fork_locks)
		error_mes("malloc return NULL");
	i = 0;
	while (i < ph->n_ph)
		pthread_mutex_init(&ph->fork_locks[i++], NULL);
}


int	main(int ac, char **av)
{
	t_global		glo;
	int				*stop;

	if (ac < 5 || ac > 6)
		error_mes("invalide args\n");
	args_checker(ac, av, &glo);
	stop = malloc(sizeof(int));
	*stop = 1;
	mutex_init(&glo);
	create_threads(&glo, stop);
	int			i;
	i = 0;
	check_if_dead(glo.phs);
	// while (i < glo.n_ph)
	// {
	// 	pthread_join(glo.th[i], NULL);
	// 	// printf("thread : %d has finished\n", i);
	// 	i++;
	// }
	return (0);
}
