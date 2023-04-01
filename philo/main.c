/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgannoun <bgannoun@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 17:37:58 by bgannoun          #+#    #+#             */
/*   Updated: 2023/04/01 02:41:31 by bgannoun         ###   ########.fr       */
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

int	args_checker(int ac, char **av, t_ph *ph)
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
	t_PH_Data		*data;
	unsigned long	start;
	
	data = (t_PH_Data *)arg;
	start = time_cal();
	while (1) 
	{
		if (data->index % 2 != 0)
			usleep(100);
		printf("%ld %d is thinking\n", (time_cal() - start), data->index);

		pthread_mutex_lock(data->left_fork);
		printf("%ld %d has taken a fork\n", (time_cal() - start), data->index);

		pthread_mutex_lock(data->right_fork);
		printf("%ld %d has taken a fork\n", (time_cal() - start), data->index);

		printf("%ld %d is eating\n", (time_cal() - start), data->index);
		usleep(data->ph1->tte * 1000);

		pthread_mutex_unlock(data->right_fork);
		pthread_mutex_unlock(data->left_fork);

		printf("%ld %d is sleeping\n", (time_cal() - start), data->index);
		usleep(data->ph1->tts * 1000);
	}
	free(arg);
	return (0);
}

void	create_threads(t_ph *ph)
{
	int	i;
	t_PH_Data	*data;
	
	ph->th = malloc(sizeof(pthread_t) * ph->n_ph);
	if (!ph->th)
		error_mes("malloc retun NULL");
	i = 0;
	while (i < ph->n_ph)
	{
		data = malloc(sizeof(t_PH_Data));
		data->index = i;
		data->left_fork = &ph->fork_locks[i];
		data->right_fork = &ph->fork_locks[(i + 1) % ph->n_ph];
		data->ph1 = ph;
		if (pthread_create(&ph->th[i], NULL, &routine, data) != 0)
			error_mes("pthread_create return error\n");
		i++;
	}
}

void	mutex_init(t_ph *ph)
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
	t_ph		ph;

	if (ac < 5 || ac > 6)
		error_mes("invalide args\n");
	args_checker(ac, av, &ph);
	mutex_init(&ph);
	create_threads(&ph);
	int			i;
	i = 0;
	while (i < ph.n_ph)
	{
		pthread_join(ph.th[i], NULL);
		printf("thread : %d has finished\n", i);
		i++;
	}
	return (0);
}
