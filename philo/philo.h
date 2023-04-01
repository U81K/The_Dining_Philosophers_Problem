/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgannoun <bgannoun@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 17:38:20 by bgannoun          #+#    #+#             */
/*   Updated: 2023/04/01 02:40:56 by bgannoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# define ERROR "error"

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <pthread.h>
#include <sys/time.h>

typedef struct	s_ph
{
	int	n_ph;
	int	ttd;
	int	tte;
	int	tts;
	int	n_each_ph_me;
	pthread_t	*th;
	pthread_mutex_t *fork_locks;
}				t_ph;

typedef struct	s_PH_Data
{
	int index;
	pthread_mutex_t *left_fork;
	pthread_mutex_t *right_fork;
	t_ph			*ph1;
	unsigned long	start;
}				t_PH_Data;

int		ft_atoi(const char *str);
void	ft_putstr_fd(char *s, int fd);
void	error_mes(char *str);

#endif
