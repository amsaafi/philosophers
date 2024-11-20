/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samsaafi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 12:43:00 by samsaafi          #+#    #+#             */
/*   Updated: 2024/11/20 12:43:24 by samsaafi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	handle_single_diner(t_diner *diner)
{
	pthread_mutex_lock(diner->right_fork);
	log_status("has taken a fork", diner, diner->id);
	pthread_mutex_unlock(diner->right_fork);
	log_status("died", diner, diner->id);
	pthread_mutex_lock(diner->death_mutex);
	*(diner->has_died) = 1;
	pthread_mutex_unlock(diner->death_mutex);
}

// Acquire forks
void	acquire_forks(t_diner *diner)
{
	pthread_mutex_lock(diner->right_fork);
	log_status("has taken a fork", diner, diner->id);
	pthread_mutex_lock(diner->left_fork);
	log_status("has taken a fork", diner, diner->id);
}

// Release forks
void	release_forks(t_diner *diner)
{
	pthread_mutex_unlock(diner->left_fork);
	pthread_mutex_unlock(diner->right_fork);
}

// Perform eating action
void	perform_eating(t_diner *diner)
{
	if (diner->meals_eaten < diner->max_meals || diner->max_meals == -1)
	{
		if (diner->total_philosophers == 1)
		{
			handle_single_diner(diner);
			return ;
		}
		acquire_forks(diner);
		log_status("is eating", diner, diner->id);
		pthread_mutex_lock(diner->eating_mutex);
		diner->is_eating = 1;
		diner->last_meal_time = get_timestamp();
		diner->meals_eaten++;
		pthread_mutex_unlock(diner->eating_mutex);
		precise_sleep(diner->time_to_eat);
		diner->is_eating = 0;
		release_forks(diner);
	}
}

// Perform sleeping and thinking actions
void	perform_sleeping_and_thinking(t_diner *diner)
{
	log_status("is sleeping", diner, diner->id);
	precise_sleep(diner->time_to_sleep);
	log_status("is thinking", diner, diner->id);
}
