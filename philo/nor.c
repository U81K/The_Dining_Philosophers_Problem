/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nor.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgannoun <bgannoun@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/01 12:49:12 by bgannoun          #+#    #+#             */
/*   Updated: 2023/05/01 13:06:43 by bgannoun         ###   ########.fr       */
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

int	check_if_dead_1(int *i, int n_ph, t_ph *phs)
{
	if (*i == n_ph)
		*i = 0;
	pthread_mutex_lock(phs[*i].m_n_each_ph_me);
	if (stop_check(phs) == 1)
		return (0);
	pthread_mutex_unlock(phs[*i].m_n_each_ph_me);
	return (1);
}
