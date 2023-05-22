/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgannoun <bgannoun@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 17:37:58 by bgannoun          #+#    #+#             */
/*   Updated: 2023/05/17 00:33:06 by bgannoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	create_threads_1(t_global *glo, int *stop, int i)
{
	glo->phs[i].index = i + 1;
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
	glo->phs[i].m_print = glo->print;
}

int	create_threads(t_global *glo, int *stop,
	pthread_mutex_t *m_n_each_ph_me, pthread_mutex_t *m_last_meal)
{
	int	i;

	glo->mutex_shared = malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(glo->mutex_shared, NULL);
	glo->th = malloc(sizeof(pthread_t) * glo->n_ph);
	i = 0;
	glo->phs = malloc(sizeof(t_ph) * glo->n_ph);
	while (i < glo->n_ph)
	{
		create_threads_1(glo, stop, i);
		glo->phs[i].m_n_each_ph_me = m_n_each_ph_me;
		glo->phs[i].m_last_meal = m_last_meal;
		if (pthread_create(&glo->th[i], NULL, &routine, &glo->phs[i]) != 0)
			return (2);
		usleep (10);
		i++;
	}
	return (0);
}

void	mutex_i(t_global *glo, pthread_mutex_t *n, pthread_mutex_t *l)
{
	pthread_mutex_init(n, NULL);
	pthread_mutex_init(l, NULL);
	glo->print = malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(glo->print, NULL);
}

void	free_all(t_global *glo, int *stop)
{
	free(glo->print);
	free(glo->fork_locks);
	free(glo->mutex_shared);
	free(glo->th);
	free(glo->phs);
	free(stop);
}

int	main(int ac, char **av)
{
	t_global		glo;
	int				*stop;
	pthread_mutex_t	m_n_each_ph_me;
	pthread_mutex_t	m_last_meal;

	if (ac < 5 || ac > 6 || args_checker(ac, av, &glo) == 2)
	{
		ft_putstr_fd("invalide args\n", 2);
		return (2);
	}
	stop = malloc(sizeof(int));
	*stop = 1;
	mutex_i(&glo, &m_n_each_ph_me, &m_last_meal);
	if (mutex_init(&glo) == 2
		|| create_threads(&glo, stop, &m_n_each_ph_me, &m_last_meal) == 2)
	{
		ft_putstr_fd("Error\n", 2);
		return (2);
	}
	check_if_dead(glo.phs, &glo);
	destroy_mutex(&glo);
	detach(&glo);
	free_all(&glo, stop);
	return (0);
}
