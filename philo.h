/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samsaafi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 15:28:24 by samsaafi          #+#    #+#             */
/*   Updated: 2024/11/18 18:36:12 by samsaafi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
#define PHILO_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>

typedef struct s_fork
{
    pthread_mutex_t mutex;
} t_fork;

typedef struct s_philo
{
    int             id;
    int             left_fork;
    int             right_fork;
    int             times_eaten;
    long long       last_meal;
    pthread_t       thread;
    struct s_data   *data;
} t_philo;

typedef struct s_data
{
    int             num_philos;
    int             time_to_die;
    int             time_to_eat;
    int             time_to_sleep;
    int             must_eat_count;
    int             someone_died;
    long long       start_time;
    t_fork          *forks;
    t_philo         *philos;
    pthread_mutex_t print_mutex;
} t_data;

int parse_args(int argc, char **argv);
long long get_time(void);
void	*ft_memset(void *ptr, int value, size_t num);
int	ft_atoi(const char *nptr);


#endif