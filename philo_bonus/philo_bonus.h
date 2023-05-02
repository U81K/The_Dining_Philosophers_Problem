/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgannoun <bgannoun@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 14:37:58 by bgannoun          #+#    #+#             */
/*   Updated: 2023/05/02 17:55:37 by bgannoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
#define PHILO_BONUS_H

#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <stdlib.h>
#include <sys/time.h>

typedef struct s_ph
{
	int				index;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	*stopp;
	pthread_mutex_t	*think;
	pthread_mutex_t	*mutex_shared;
	pthread_mutex_t	*m_n_each_ph_me;
	pthread_mutex_t	*m_last_meal;
	unsigned long	start;
	unsigned long	tmp;
	unsigned long	tmp2;
	unsigned long	last_meal;
	int				thinking;
	int				ttd;
	int				tte;
	int				tts;
	int				n_each_ph_me;
	int				n_ph;
	int				*stop;
}				t_ph;

typedef struct s_global
{
	int				n_ph;
	int				ttd;
	int				tte;
	int				tts;
	int				n_each_ph_me;
	sem_t			*mutex;
	t_ph			*phs;
	unsigned long	start;
}				t_global;

int		args_checker(int ac, char **av, t_global *ph);
void	ft_putstr_fd(char *s, int fd);
int		ft_atoi(const char *str);

#endif