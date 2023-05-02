/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgannoun <bgannoun@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 14:37:15 by bgannoun          #+#    #+#             */
/*   Updated: 2023/05/02 18:33:55 by bgannoun         ###   ########.fr       */
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

void	routine(t_global *glo, int i, int fork_id_r, int fork_id_l)
{
	// while (1)
	// {
		printf("%ld %d is thinking\n", (time_cal() - glo->start), glo->phs[i].index);
		// pthread_mutex_lock(data->left_fork);
		sem_wait(&glo->mutex[i % glo->n_ph]);
		printf("%ld %d has taken a fork\n", (time_cal() - glo->start), glo->phs[i].index);
		// pthread_mutex_lock(data->right_fork);
		sem_wait(&glo->mutex[(i + 1) % glo->n_ph]);
		printf("%ld %d has taken a fork\n", (time_cal() - glo->start), glo->phs[i].index);
		printf("%ld %d is eating\n", (time_cal() - glo->start), glo->phs[i].index);
		usleep(glo->tte * 1000);
		sem_post(&glo->mutex[i % glo->n_ph]);
		sem_post(&glo->mutex[(i + 1) % glo->n_ph]);
		// pthread_mutex_unlock(data->right_fork);
		// pthread_mutex_unlock(data->left_fork);
		printf("%ld %d is sleeping\n", (time_cal() - glo->start), glo->phs[i].index);
		usleep(glo->tts * 1000);
	// }
}

void	create_proc(t_global *glo)
{
	int	i;
	pid_t	pid;

	i = 1;
	// printf("main proc %d\n", getpid());
	glo->start = time_cal();
	while (i <= glo->n_ph)
	{
		pid = fork();
		if (pid == 0)
		{
			glo->phs[i].index = i;
			routine(glo, i, i, i + 1);
			exit(0);
		}
		// else
		// 	printf("parent id is %d\n", getpid());
		i++;
	}
	// printf("%d died\n", getpid() +2 - waitpid(-1, NULL, 0));
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
