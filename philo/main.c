/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgannoun <bgannoun@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 17:37:58 by bgannoun          #+#    #+#             */
/*   Updated: 2023/04/04 02:35:46 by bgannoun         ###   ########.fr       */
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
	unsigned long	tmp;
	int				eat_times;
	int				i;
	
	data = (t_ph *)arg;
	start = time_cal();
	data->start = start;
	eat_times = data->n_each_ph_me;
	i = 0;
	tmp = 0;
	while (1)
	{
		// printf("%d ===>thinking===>%ld\n", data->index, data->thinking);
		printf("%ld %d is thinking\n", (time_cal() - start), data->index);
		if (eat_times == 0)
			break ;
		data->thinking = time_cal() - start;
		if (data->index % 2 != 0)
			usleep(100);
			pthread_mutex_lock(data->left_fork);
		printf("%ld %d has taken a fork\n", (time_cal() - start), data->index);
			pthread_mutex_lock(data->right_fork);
		printf("%ld %d has taken a fork\n", (time_cal() - start), data->index);
		// if (i == 0 && (time_cal() - start) >= ((unsigned long)data->ph1->ttd * 1000))
		// {
		// 	printf("%ld %d died\n", (time_cal() - start), data->index);
		// 	exit(0);
		// 	pthread_mutex_unlock(data->right_fork);
		// 	pthread_mutex_unlock(data->left_fork);
		// 	break ;
		// }
		// i++;
		// if (i != 0 && ((time_cal() - start) - tmp) >= ((unsigned long)data->ph1->ttd))
		// {
		// 	printf("%ld %d died\n", (time_cal() - start), data->index);
		// 	// exit(0);
		// 	pthread_mutex_unlock(data->right_fork);
		// 	pthread_mutex_unlock(data->left_fork);
		// 	break ;
		// }
		// if (i != 0)
		// 	tmp = (time_cal() - start) - tmp;
		data->last_meal = time_cal() - start;
		// printf("%d ===>last_meal ===>%ld\n", data->index, data->last_meal);
		printf("%ld %d is eating\n", (time_cal() - start), data->index);
		usleep(data->tte * 1000);
		eat_times--;
			pthread_mutex_unlock(data->right_fork);
			pthread_mutex_unlock(data->left_fork);

		printf("%ld %d is sleeping\n", (time_cal() - start), data->index);
		usleep(data->tts * 1000);
	}
	// free(arg);
	return (NULL);
}

int check_if_dead(t_ph *phs)
{
	long	start;
	long	tmp;
	int		i;
	
	// sleep(1);
	start = phs[0].start;
	tmp = 0;
	i = 0;
	while (1)
	{
		if (i == phs[i].n_ph)
			i = 0;
		// tmp = phs[i].last_meal;
		if ((phs[i].thinking  - phs[i].last_meal) >= (long)phs[i].ttd)
		{
			printf("%ld %d died\n", (time_cal() - start), phs[i].index);
			exit(0);
		}
		i++;
	}
	return (0);
}

void	create_threads(t_global *glo)
{
	int	i;

	glo->th = malloc(sizeof(pthread_t) * glo->n_ph);
	if (!glo->th)
		error_mes("malloc retun NULL");
	i = 0;
	glo->phs = malloc(sizeof(t_ph) * glo->n_ph);
	while (i < glo->n_ph)
	{
		glo->phs[i].index = i;
		glo->phs[i].left_fork = &glo->fork_locks[i];
		glo->phs[i].right_fork = &glo->fork_locks[(i + 1) % glo->n_ph];
		glo->phs[i].tte = glo->tte;
		glo->phs[i].tts = glo->tts;
		glo->phs[i].n_each_ph_me = glo->n_each_ph_me;
		glo->phs[i].n_ph = glo->n_ph;
		glo->phs[i].ttd = glo->ttd;
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

	if (ac < 5 || ac > 6)
		error_mes("invalide args\n");
	args_checker(ac, av, &glo);
	mutex_init(&glo);
	create_threads(&glo);
	int			i;
	i = 0;
	while (i < glo.n_ph)
	{
		pthread_join(glo.th[i], NULL);
		// printf("thread : %d has finished\n", i);
		i++;
	}
	check_if_dead(glo.phs);
	// printf("====>%d\n", check_philosopher_alive(1, &ph));
	return (0);
}
