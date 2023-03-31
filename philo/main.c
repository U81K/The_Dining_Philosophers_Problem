/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgannoun <bgannoun@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 17:37:58 by bgannoun          #+#    #+#             */
/*   Updated: 2023/03/31 01:09:28 by bgannoun         ###   ########.fr       */
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

typedef struct	s_PH_Data
{
	int index;
	pthread_mutex_t *left_fork;
	pthread_mutex_t *right_fork;
}				t_PH_Data;

void	*routine(void *arg)
{
	// int i;
	t_PH_Data *data;
	
	data = (t_PH_Data *)arg;
	// i =  *(int *)arg;
	// int left_fork = fork_indices[i];
  	// int right_fork = fork_indices[(i + 1) % NUM_FORKS];
	// pthread_mutex_lock(&mutex);
	while (1) 
	{
		// pick up left fork
		pthread_mutex_lock(data->left_fork);
		
		// pick up right fork
		pthread_mutex_lock(data->right_fork);
		
		// eat
		// ...
		printf("philosopher %d is eating\n", data->index);
		
		// put down right fork
		pthread_mutex_unlock(data->right_fork);
		
		// put down left fork
		pthread_mutex_unlock(data->left_fork);
		
		printf("philosopher %d is sleeping\n", data->index);
		sleep(2);
		// think
		// ...
	}
	// pthread_mutex_unlock(&mutex);
	free(arg);
	return (0);
}

void	create_threads(t_ph *ph)
{
	int	i;
	// int	*index;
	t_PH_Data	*data;
	int fork_indices[4] = {0, 1, 2, 3};
	
	ph->th = malloc(sizeof(pthread_t) * ph->n_ph);
	if (!ph->th)
		error_mes("malloc retun NULL");
	i = 0;
	while (i < ph->n_ph)
	{
		// index = malloc(sizeof(int));
		data = malloc(sizeof(t_PH_Data));
		data->index = i;
		data->left_fork = &ph->fork_locks[fork_indices[i]];
		data->right_fork = &ph->fork_locks[fork_indices[(i + 1) % ph->n_ph]];
		// *index = i;
		if (pthread_create(&ph->th[i], NULL, &routine, data) != 0)
			error_mes("pthread_create return error\n");
		// printf("thread : %d has created\n", i);
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
