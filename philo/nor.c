/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nor.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgannoun <bgannoun@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/01 12:49:12 by bgannoun          #+#    #+#             */
/*   Updated: 2023/05/17 00:30:43 by bgannoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

int	check_if_dead_1(int *i, t_ph *phs)
{
	pthread_mutex_lock(phs[*i].m_n_each_ph_me);
	if (stop_check(phs) == 1)
		return (0);
	pthread_mutex_unlock(phs[*i].m_n_each_ph_me);
	return (1);
}

void	con(t_ph *phs, t_global *glo, int i)
{
	pthread_mutex_lock(phs[i].mutex_shared);
	*phs[i].stop = 0;
	pthread_mutex_unlock(phs[i].mutex_shared);
	pthread_mutex_lock(glo->print);
	printf("%ld %d died\n", (time_cal() - phs[i].start), phs[i].index);
}

int	max(t_global *ph)
{
	if (ph->n_ph >= 2147483647 || ph->n_each_ph_me >= 2147483647
		|| ph->ttd >= 2147483647 || ph->tte >= 2147483647
		|| ph->tts >= 2147483647)
		return (2);
	return (0);
}
