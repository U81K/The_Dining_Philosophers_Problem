/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgannoun <bgannoun@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 17:38:20 by bgannoun          #+#    #+#             */
/*   Updated: 2023/05/16 19:38:48 by bgannoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# define ERROR "error"

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>

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
	pthread_mutex_t	*m_print;
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
	pthread_t		*th;
	pthread_mutex_t	*fork_locks;
	t_ph			*phs;
	pthread_mutex_t	*mutex_shared;
	pthread_mutex_t	*print;
}				t_global;

int				ft_atoi(const char *str);
void			ft_putstr_fd(char *s, int fd);
int				error_mes(char *str);
int				check_if_nb(char *str);
int				args_checker(int ac, char **av, t_global *ph);
unsigned long	time_cal(void);
int				stop_check(t_ph *phs);
int				check_if_dead(t_ph *phs, t_global *glo);
void			destroy_mutex(t_global *glo);
void			detach(t_global *glo);
int				mutex_init(t_global *ph);
int				check_if_dead_1(int *i, t_ph *phs);
void			*routine(void *arg);
void			con(t_ph *phs, t_global *glo, int i);
int				max(t_global *ph);

#endif
