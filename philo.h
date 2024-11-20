/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samsaafi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 10:25:46 by samsaafi          #+#    #+#             */
/*   Updated: 2024/11/20 14:16:24 by samsaafi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

// Structure Definitions
typedef struct s_simulation
{
	int				has_died;
	pthread_mutex_t	death_mutex;
	pthread_mutex_t	eating_mutex;
	pthread_mutex_t	output_mutex;
	size_t			time_to_die;
	size_t			time_to_eat;
	size_t			time_to_sleep;
	int				max_meals;
	int				total_philosophers;
}					t_simulation;

typedef struct s_diner
{
	pthread_t		thread;
	int				id;
	int				is_eating;
	int				meals_eaten;
	size_t			last_meal_time;
	size_t			time_to_die;
	size_t			time_to_eat;
	size_t			time_to_sleep;
	size_t			start_time;
	int				total_philosophers;
	int				max_meals;
	int				*has_died;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*output_mutex;
	pthread_mutex_t	*death_mutex;
	pthread_mutex_t	*eating_mutex;
	t_simulation	*sim_data;
}					t_diner;

// Function Prototypes
size_t				get_timestamp(void);
int					string_to_int(const char *str);
int					validate_inputs(char **argv);
int					initialize_arguments(int argc, char **argv,
						t_simulation *sim);
void				assign_forks(int i, pthread_mutex_t *forks,
						t_diner *diners);
void				initialize_diners(t_diner *diners, pthread_mutex_t *forks,
						t_simulation *sim);
void				initialize_mutexes(pthread_mutex_t *forks, int philo_num,
						t_simulation *sim);
int					check_death(t_diner diner);
void				log_status(char *msg, t_diner *diner, int id);
void				handle_single_diner(t_diner *diner);
void				acquire_forks(t_diner *diner);
void				release_forks(t_diner *diner);
void				perform_eating(t_diner *diner);
void				perform_sleeping_and_thinking(t_diner *diner);
int					precise_sleep(size_t milliseconds);
int					check_all_dead(t_diner *diners, t_simulation *sim);
int					check_all_eaten(t_diner *diners, t_simulation *sim);
void				*monitor_routine(void *diners_ptr);
void				*diner_routine(void *diner_ptr);
void				create_threads(t_diner *diners, int total_philosophers);
int					parse_args(int argc, char **argv);
