/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgannoun <bgannoun@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 14:37:58 by bgannoun          #+#    #+#             */
/*   Updated: 2023/05/07 15:22:51 by bgannoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <stdio.h>
# include <unistd.h>
# include <semaphore.h>
# include <stdlib.h>
# include <sys/time.h>
# include <signal.h>
# include <pthread.h>

typedef struct s_ph
{
	int				index;
	pid_t			pid;
	unsigned long	start;
	unsigned long	last_meal;
	int				thinking;
	int				ttd;
	int				tte;
	int				tts;
	int				n_each_ph_me;
	int				n_ph;
	sem_t			*sem;
	sem_t			*dead;
	sem_t			*print;
}				t_ph;

typedef struct s_global
{
	int				n_ph;
	int				ttd;
	int				tte;
	int				tts;
	int				n_each_ph_me;
	unsigned long	start;
	t_ph			*phs;
	pthread_t		*thr;
	sem_t			*sem;
	sem_t			*dead;
	sem_t			*print;
}				t_global;

int				args_checker(int ac, char **av, t_global *ph);
void			ft_putstr_fd(char *s, int fd);
int				ft_atoi(const char *str);
void			create_proc(t_global *glo);
void			routine(t_ph *data);
void			*thr_routine(void *args);
unsigned long	time_cal(void);

#endif