/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgannoun <bgannoun@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 14:37:15 by bgannoun          #+#    #+#             */
/*   Updated: 2023/05/03 18:25:08 by bgannoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

unsigned long	time_cal(void)
{
	unsigned long	i;
	struct timeval	current_time;

	gettimeofday(&current_time, NULL);
	i = (current_time.tv_sec * 1000) + (current_time.tv_usec / 1000);
	return (i);
}

void	routine(t_ph *data)
{
	while (1)
	{
		printf("%ld %d is thinking\n", (time_cal() - data->start), data->index);
		sem_wait(data->sem);
		printf("%ld %d has taken a fork\n", (time_cal() - data->start), data->index);
		sem_wait(data->sem);
		printf("%ld %d has taken a fork\n", (time_cal() - data->start), data->index);
		data->last_meal = time_cal();
		printf("%ld %d is eating\n", (time_cal() - data->start), data->index);
		usleep(data->tte * 1000);
		if (data->n_each_ph_me-- == 0)
			exit(0);
		sem_post(data->sem);
		sem_post(data->sem);
		printf("%ld %d is sleeping\n", (time_cal() - data->start), data->index);
		usleep(data->tts * 1000);
	}
}

void	*thr_routine(void *args)
{
	t_ph *data;

	data = (t_ph *)args;
	int i;
	i  = 0;
	usleep(1000);
	while(1)
	{
		if (i == glo->n_ph)
			i = 0;
		if ((time_cal() - glo->phs[i].last_meal) >= (unsigned long)glo->ttd)
		{
			printf("%ld %d died\n", (time_cal() - glo->start), glo->phs[i].index);
			exit(0);
		}
		i++;
		usleep(1000);
	}
	return (NULL);
}

void	create_proc(t_global *glo)
{
	int	i;
	pid_t	pid;
	sem_t *sem;
	i = 0;
	glo->thr = malloc(sizeof(pthread_t) * glo->n_ph);
	glo->start = time_cal();
	sem_unlink("/sema");
	sem = sem_open("/sema", O_CREAT , 0644, glo->n_ph);
	while (i < glo->n_ph)
	{
		pid = fork();
		if (pid == 0)
		{
			glo->phs[i].index = i;
			glo->phs[i].tte = glo->tte;
			glo->phs[i].tts = glo->tts;
			glo->phs[i].ttd = glo->ttd;
			glo->phs[i].start = glo->start;
			glo->phs[i].sem = sem;
			glo->phs[i].n_each_ph_me = glo->n_each_ph_me + 2;
			pthread_create(&glo->thr[i], NULL, &thr_routine, &glo->phs[i]);
			routine(&(glo->phs[i]));
			exit(0);
		}
		else
			glo->phs[i].pid = pid;
		i++;
	}
	// check_if_dead(glo->phs);
	waitpid(-1, NULL, 0);
	int j;
	j = 0;
	while (j < glo->n_ph)
		kill(glo->phs[j++].pid, 0);
	sem_close(sem);
	sem_unlink("/sema");
}

int main(int ac, char **av)
{
	t_global	glo;
	
	if (ac < 5 || ac > 6 || args_checker(ac, av, &glo) == 2)
	{
		ft_putstr_fd("invalide args\n", 2);
		return (2);
	}
	glo.mutex = malloc(sizeof(sem_t) * glo.n_ph);
	glo.phs = malloc(sizeof(t_ph) * glo.n_ph);
	create_proc(&glo);
	// printf("%d\n", glo.mutex);
	// sem_wait(&mutex);
	// printf("%d\n", mutex);
	// sem_post(&mutex);
	// printf("%d\n", mutex);
	return (0);
}
