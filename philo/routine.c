/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgannoun <bgannoun@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 16:04:33 by bgannoun          #+#    #+#             */
/*   Updated: 2023/05/12 21:53:37 by bgannoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	routine_1(t_ph *data)
{
	pthread_mutex_lock(data->left_fork);
	pthread_mutex_lock(data->m_print);
	printf("%ld %d has taken a fork\n", (time_cal() - data->start), data->index);
	pthread_mutex_unlock(data->m_print);
	pthread_mutex_lock(data->right_fork);
	pthread_mutex_lock(data->m_print);
	printf("%ld %d has taken a fork\n", (time_cal() - data->start), data->index);
	pthread_mutex_unlock(data->m_print);
	pthread_mutex_lock(data->m_last_meal);
	data->last_meal = time_cal();
	pthread_mutex_unlock(data->m_last_meal);
	pthread_mutex_lock(data->m_print);
	printf("%ld %d is eating\n", (time_cal() - data->start), data->index);
	pthread_mutex_unlock(data->m_print);
	data->tmp = time_cal();
	pthread_mutex_lock(data->m_n_each_ph_me);
	data->n_each_ph_me--;
	pthread_mutex_unlock(data->m_n_each_ph_me);
	while (time_cal() - data->tmp < (unsigned long)data->tte)
		usleep(200);
	pthread_mutex_unlock(data->right_fork);
	pthread_mutex_unlock(data->left_fork);
}

void	routine_2(t_ph	*data)
{
	pthread_mutex_lock(data->m_print);
	printf("%ld %d is sleeping\n", (time_cal() - data->start), data->index);
	pthread_mutex_unlock(data->m_print);
	data->tmp2 = time_cal();
	while (time_cal() - data->tmp2 < (unsigned long)data->tts)
		usleep(200);
}

void	*routine(void *arg)
{
	t_ph	*data;

	data = (t_ph *)arg;
	pthread_mutex_lock(data->m_last_meal);
	data->last_meal = data->start;
	pthread_mutex_unlock(data->m_last_meal);
	if (data->index % 2 != 0)
		usleep(100);
	while (1)
	{
		pthread_mutex_lock(data->m_print);
		printf("%ld %d is thinking\n", (time_cal() - data->start), data->index);
		pthread_mutex_unlock(data->m_print);
		pthread_mutex_lock(data->mutex_shared);
		if (data->n_each_ph_me == 0 || *data->stop == 0)
		{
			*data->stop = 0;
			return (NULL);
		}
		pthread_mutex_unlock(data->mutex_shared);
		routine_1(data);
		routine_2(data);
	}
	return (NULL);
}
