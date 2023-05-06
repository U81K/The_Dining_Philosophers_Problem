/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgannoun <bgannoun@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 14:37:15 by bgannoun          #+#    #+#             */
/*   Updated: 2023/05/06 11:15:24 by bgannoun         ###   ########.fr       */
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
			sem_wait(data->print);
		printf("%ld %d is thinking\n", (time_cal() - data->start), data->index);
			sem_post(data->print);
		sem_wait(data->sem);
			sem_wait(data->print);
		printf("%ld %d has taken a fork\n", (time_cal() - data->start), data->index);
			sem_post(data->print);
		sem_wait(data->sem);
			sem_wait(data->print);
		printf("%ld %d has taken a fork\n", (time_cal() - data->start), data->index);
			sem_post(data->print);
		
		// sem_wait(data->dead);
		data->last_meal = time_cal();
		// sem_post(data->dead);
		
		data->tmp2 = time_cal();
			sem_wait(data->print);
		printf("%ld %d is eating\n", (time_cal() - data->start), data->index);
			sem_post(data->print);
		while (time_cal() - data->tmp2 <= (unsigned long)data->tte)
			usleep(200);
		if (data->n_each_ph_me-- == 0)
			exit(0);
		sem_post(data->sem);
		sem_post(data->sem);
		data->tmp = time_cal();
			sem_wait(data->print);
		printf("%ld %d is sleeping\n", (time_cal() - data->start), data->index);
			sem_post(data->print);
		while (time_cal() - data->tmp <= (unsigned long)data->tts)
			usleep(200);
	}
}

void	*thr_routine(void *args)
{
	t_ph *data;

	data = (t_ph *)args;
	int i;
	i  = 0;
	usleep(1000 * data->ttd);
	while(1)
	{
		if (i == data->n_ph - 1)
			i = 0;
		sem_wait(data->print);
		if ((time_cal() - data->last_meal) >= (unsigned long)data->ttd)
		{
			sem_wait(data->dead);
			printf("%ld %d died\n", (time_cal() - data->start), data->index);
			exit(0);
		}
		sem_post(data->print);
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
	sem_t	*dead;
	sem_t	*print;
	
	i = 0;
	glo->thr = malloc(sizeof(pthread_t) * glo->n_ph);
	sem_unlink("/sema");
	sem_unlink("/dead");
	sem_unlink("/print");
	sem = sem_open("/sema", O_CREAT , 0644, glo->n_ph);
	dead = sem_open("/dead", O_CREAT , 0644, 1);
	print = sem_open("/print", O_CREAT, 0644, 1);
	glo->start = time_cal();
	while (i < glo->n_ph)
	{
		pid = fork();
		if (pid == 0)
		{
			glo->phs[i].index = i + 1;
			glo->phs[i].tte = glo->tte;
			glo->phs[i].tts = glo->tts;
			glo->phs[i].ttd = glo->ttd;
			glo->phs[i].start = glo->start;
			glo->phs[i].sem = sem;
			glo->phs[i].n_each_ph_me = glo->n_each_ph_me + 2;
			glo->phs[i].dead = dead;
			glo->phs[i].print = print;
			pthread_create(&glo->thr[i], NULL, &thr_routine, &glo->phs[i]);
			routine(&(glo->phs[i]));
			exit(0);
		}
		else
			glo->phs[i].pid = pid;
		i++;
	}
	waitpid(-1, NULL, 0);
	int j;
	j = 0;
	while (j < glo->n_ph)
		kill(glo->phs[j++].pid, SIGKILL);
	sem_close(sem);
	sem_unlink("/sema");
	sem_unlink("/dead");
	sem_unlink("/print");
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
	return (0);
}
