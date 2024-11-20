/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samsaafi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 12:35:40 by samsaafi          #+#    #+#             */
/*   Updated: 2024/11/20 14:28:10 by samsaafi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	initialize_arguments(int argc, char **argv, t_simulation *sim)
{
	if (argc != 5 && argc != 6)
	{
		printf("error params numbers at least 5 params\n");
		return (1);
	}
	if (parse_args(argc, argv))
		return (1);
	if (argv[1] && string_to_int(argv[1]) == 0)
		return (1);
	sim->total_philosophers = string_to_int(argv[1]);
	sim->time_to_die = string_to_int(argv[2]);
	sim->time_to_eat = string_to_int(argv[3]);
	sim->time_to_sleep = string_to_int(argv[4]);
	sim->has_died = 0;
	sim->max_meals = -1;
	if (argc == 6)
		sim->max_meals = string_to_int(argv[5]);
	return (0);
}

void	assign_forks(int i, pthread_mutex_t *forks, t_diner *diners)
{
	diners[i].left_fork = &forks[i];
	if (i == 0)
	{
		diners[i].right_fork = &forks[diners[i].total_philosophers - 1];
	}
	else
	{
		diners[i].right_fork = &forks[i - 1];
	}
}

void	initialize_diners(t_diner *diners, pthread_mutex_t *forks,
		t_simulation *sim)
{
	int	i;

	i = 0;
	while (i < sim->total_philosophers)
	{
		diners[i].id = i + 1;
		diners[i].is_eating = 0;
		diners[i].meals_eaten = 0;
		diners[i].start_time = get_timestamp();
		diners[i].last_meal_time = get_timestamp();
		diners[i].output_mutex = &sim->output_mutex;
		diners[i].death_mutex = &sim->death_mutex;
		diners[i].eating_mutex = &sim->eating_mutex;
		diners[i].has_died = &sim->has_died;
		diners[i].total_philosophers = sim->total_philosophers;
		diners[i].time_to_eat = sim->time_to_eat;
		diners[i].time_to_die = sim->time_to_die;
		diners[i].time_to_sleep = sim->time_to_sleep;
		diners[i].max_meals = sim->max_meals;
		diners[i].sim_data = sim;
		assign_forks(i, forks, diners);
		i++;
	}
}

void	initialize_mutexes(pthread_mutex_t *forks, int philo_num,
		t_simulation *sim)
{
	int	i;

	i = 0;
	while (i < philo_num)
	{
		pthread_mutex_init(&forks[i], NULL);
		i++;
	}
	pthread_mutex_init(&sim->death_mutex, NULL);
	pthread_mutex_init(&sim->eating_mutex, NULL);
	pthread_mutex_init(&sim->output_mutex, NULL);
}
