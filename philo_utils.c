/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samsaafi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 12:37:37 by samsaafi          #+#    #+#             */
/*   Updated: 2024/11/20 12:42:31 by samsaafi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_death(t_diner diner)
{
	pthread_mutex_lock(diner.death_mutex);
	if (*(diner.has_died))
	{
		pthread_mutex_unlock(diner.death_mutex);
		return (1);
	}
	pthread_mutex_unlock(diner.death_mutex);
	return (0);
}

// Log status messages
void	log_status(char *msg, t_diner *diner, int id)
{
	pthread_mutex_lock(diner->output_mutex);
	if (!check_death(*diner))
		printf("%zu %d %s\n", (get_timestamp() - diner->start_time), id, msg);
	pthread_mutex_unlock(diner->output_mutex);
}

int	precise_sleep(size_t milliseconds)
{
	size_t	start;
	size_t	current;

	start = get_timestamp();
	while (1)
	{
		current = get_timestamp();
		if ((current - start) >= milliseconds)
			break ;
		usleep(100);
	}
	return (0);
}

int	check_all_dead(t_diner *diners, t_simulation *sim)
{
	int	i;

	i = 0;
	while (i < sim->total_philosophers)
	{
		pthread_mutex_lock(diners[i].eating_mutex);
		if ((get_timestamp()
				- diners[i].last_meal_time) >= diners[i].time_to_die
			&& !diners[i].is_eating)
		{
			pthread_mutex_unlock(diners[i].eating_mutex);
			log_status("died", &diners[i], diners[i].id);
			pthread_mutex_lock(diners[i].death_mutex);
			*(diners[i].has_died) = 1;
			pthread_mutex_unlock(diners[i].death_mutex);
			return (1);
		}
		pthread_mutex_unlock(diners[i].eating_mutex);
		i++;
	}
	return (0);
}

// Check if all diners have eaten required meals
int	check_all_eaten(t_diner *diners, t_simulation *sim)
{
	int	i;
	int	counter;

	i = 0;
	counter = 0;
	while (i < sim->total_philosophers)
	{
		pthread_mutex_lock(diners[i].eating_mutex);
		if (diners[i].meals_eaten >= diners[i].max_meals)
			counter++;
		pthread_mutex_unlock(diners[i].eating_mutex);
		i++;
	}
	if (counter == sim->total_philosophers && sim->max_meals != -1)
	{
		pthread_mutex_lock(&sim->death_mutex);
		*(diners[0].has_died) = 1;
		pthread_mutex_unlock(&sim->death_mutex);
		return (1);
	}
	return (0);
}
