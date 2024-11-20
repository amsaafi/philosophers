/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samsaafi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 10:25:38 by samsaafi          #+#    #+#             */
/*   Updated: 2024/11/20 14:27:15 by samsaafi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*monitor_routine(void *diners_ptr)
{
	t_diner			*diners;
	t_simulation	*sim;

	diners = *(t_diner **)diners_ptr;
	sim = diners[0].sim_data;
	while (1)
	{
		if (check_all_dead(diners, sim))
			break ;
		if (check_all_eaten(diners, sim))
			break ;
		usleep(1000);
	}
	return (NULL);
}

void	*diner_routine(void *diner_ptr)
{
	t_diner	*diner;

	diner = (t_diner *)diner_ptr;
	if (diner->id % 2 == 0)
		precise_sleep(1);
	while (!check_death(*diner))
	{
		perform_eating(diner);
		perform_sleeping_and_thinking(diner);
	}
	return (NULL);
}

void	create_threads(t_diner *diners, int total_philosophers)
{
	pthread_t	monitor_thread;
	int			i;

	i = 0;
	if (pthread_create(&monitor_thread, NULL, &monitor_routine, &diners))
	{
		printf("Monitor thread creation failed\n");
		return ;
	}
	while (i < total_philosophers)
	{
		if (pthread_create(&diners[i].thread, NULL, &diner_routine, &diners[i]))
		{
			printf("Diner thread creation failed\n");
			return ;
		}
		i++;
	}
	pthread_join(monitor_thread, NULL);
	i = 0;
	while (i < total_philosophers)
	{
		pthread_join(diners[i].thread, NULL);
		i++;
	}
}

int	main(int argc, char **argv)
{
	t_simulation	sim;
	t_diner			*diners;
	pthread_mutex_t	*forks;

	if (initialize_arguments(argc, argv, &sim))
		return (1);
	diners = malloc(sizeof(t_diner) * sim.total_philosophers);
	if (!diners)
	{
		printf("Error: Memory allocation for diners failed.\n");
		return (1);
	}
	forks = malloc(sizeof(pthread_mutex_t) * sim.total_philosophers);
	if (!forks)
	{
		free(diners);
		printf("Error: Memory allocation for forks failed.\n");
		return (1);
	}
	initialize_diners(diners, forks, &sim);
	initialize_mutexes(forks, sim.total_philosophers, &sim);
	create_threads(diners, sim.total_philosophers);
	free(diners);
	free(forks);
	return (0);
}
