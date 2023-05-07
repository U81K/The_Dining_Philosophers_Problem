/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_pro.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgannoun <bgannoun@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/07 14:20:30 by bgannoun          #+#    #+#             */
/*   Updated: 2023/05/07 15:19:01 by bgannoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	kill_proc(t_global *glo)
{
	int	j;

	j = 0;
	while (j < glo->n_ph)
		kill(glo->phs[j++].pid, SIGKILL);
	sem_close(glo->sem);
	sem_close(glo->dead);
	sem_close(glo->print);
	sem_unlink("/sema");
	sem_unlink("/dead");
	sem_unlink("/print");
}

void	init_v(t_global *glo, int i)
{
	glo->phs[i].index = i + 1;
	glo->phs[i].tte = glo->tte;
	glo->phs[i].tts = glo->tts;
	glo->phs[i].ttd = glo->ttd;
	glo->phs[i].start = glo->start;
	glo->phs[i].n_each_ph_me = glo->n_each_ph_me + 2;
	glo->phs[i].sem = glo->sem;
	glo->phs[i].dead = glo->dead;
	glo->phs[i].print = glo->print;
}

void	init_sem(t_global *glo)
{
	sem_unlink("/sema");
	sem_unlink("/dead");
	sem_unlink("/print");
	glo->sem = sem_open("/sema", O_CREAT, 0644, glo->n_ph);
	glo->dead = sem_open("/dead", O_CREAT, 0644, 1);
	glo->print = sem_open("/print", O_CREAT, 0644, 1);
}

void	create_proc(t_global *glo)
{
	int		i;
	pid_t	pid;

	i = 0;
	glo->thr = malloc(sizeof(pthread_t) * glo->n_ph);
	init_sem(glo);
	glo->start = time_cal();
	while (i < glo->n_ph)
	{
		pid = fork();
		if (pid == 0)
		{
			init_v(glo, i);
			pthread_create(&glo->thr[i], NULL, &thr_routine, &glo->phs[i]);
			routine(&(glo->phs[i]));
			exit(0);
		}
		else
			glo->phs[i].pid = pid;
		i++;
	}
	waitpid(-1, NULL, 0);
	kill_proc(glo);
}
