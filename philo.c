#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>

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

long long get_time(void)
{
    struct timeval tv;
    
    gettimeofday(&tv, NULL);
    return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void print_status(t_data *data, int id, char *status)
{
    pthread_mutex_lock(&data->print_mutex);
    if (!data->someone_died)
        printf("%lld %d %s\n", get_time() - data->start_time, id + 1, status);
    pthread_mutex_unlock(&data->print_mutex);
}

void philo_think(t_philo *philo)
{
    print_status(philo->data, philo->id, "is thinking");
}

void philo_sleep(t_philo *philo)
{
    t_data *data = philo->data;
    
    print_status(data, philo->id, "is sleeping");
    usleep(data->time_to_sleep * 1000);
}

void philo_eat(t_philo *philo)
{
    t_data *data = philo->data;
    
    pthread_mutex_lock(&data->forks[philo->left_fork].mutex);
    print_status(data, philo->id, "has taken a fork");
    pthread_mutex_lock(&data->forks[philo->right_fork].mutex);
    print_status(data, philo->id, "has taken a fork");
    
    print_status(data, philo->id, "is eating");
    philo->last_meal = get_time();
    philo->times_eaten++;
    usleep(data->time_to_eat * 1000);
    
    pthread_mutex_unlock(&data->forks[philo->left_fork].mutex);
    pthread_mutex_unlock(&data->forks[philo->right_fork].mutex);
}

void *philosopher(void *arg)
{
    t_philo *philo = (t_philo *)arg;
    t_data *data = philo->data;
    
    if (philo->id % 2)
        usleep(1000);
    
    while (!data->someone_died)
    {
        philo_eat(philo);
        if (data->must_eat_count != -1 && 
            philo->times_eaten >= data->must_eat_count)
            break;
        
        philo_sleep(philo);
        philo_think(philo);
    }
    return NULL;
}

void monitor_philos(t_data *data)
{
    int i;
    int all_ate;
    
    while (!data->someone_died)
    {
        i = -1;
        all_ate = 1;
        while (++i < data->num_philos)
        {
            if (get_time() - data->philos[i].last_meal > data->time_to_die)
            {
                print_status(data, i, "died");
                data->someone_died = 1;
                break;
            }
            if (data->must_eat_count != -1 && 
                data->philos[i].times_eaten < data->must_eat_count)
                all_ate = 0;
        }
        if (data->must_eat_count != -1 && all_ate)
            break;
        usleep(1000);
    }
}

int init_data(t_data *data, int argc, char **argv)
{
    memset(data, 0, sizeof(t_data));
    data->num_philos = atoi(argv[1]);
    data->time_to_die = atoi(argv[2]);
    data->time_to_eat = atoi(argv[3]);
    data->time_to_sleep = atoi(argv[4]);
    data->must_eat_count = (argc == 6) ? atoi(argv[5]) : -1;
    data->someone_died = 0;
    data->start_time = get_time();
    
    if (pthread_mutex_init(&data->print_mutex, NULL))
        return 1;
        
    return 0;
}

int init_forks(t_data *data)
{
    int i;
    
    data->forks = malloc(sizeof(t_fork) * data->num_philos);
    if (!data->forks)
        return 1;
        
    i = -1;
    while (++i < data->num_philos)
        if (pthread_mutex_init(&data->forks[i].mutex, NULL))
            return 1;
            
    return 0;
}

int init_philos(t_data *data)
{
    int i;
    
    data->philos = malloc(sizeof(t_philo) * data->num_philos);
    if (!data->philos)
        return 1;
        
    i = -1;
    while (++i < data->num_philos)
    {
        data->philos[i].id = i;
        data->philos[i].left_fork = i;
        data->philos[i].right_fork = (i + 1) % data->num_philos;
        data->philos[i].times_eaten = 0;
        data->philos[i].last_meal = get_time();
        data->philos[i].data = data;
    }
    return 0;
}

int start_simulation(t_data *data)
{
    int i;
    
    i = -1;
    while (++i < data->num_philos)
        if (pthread_create(&data->philos[i].thread, NULL, 
                          philosopher, &data->philos[i]))
            return 1;
            
    monitor_philos(data);
    
    i = -1;
    while (++i < data->num_philos)
        pthread_join(data->philos[i].thread, NULL);
        
    return 0;
}

void cleanup(t_data *data)
{
    int i;
    
    if (data->forks)
    {
        i = -1;
        while (++i < data->num_philos)
            pthread_mutex_destroy(&data->forks[i].mutex);
        free(data->forks);
    }
    
    if (data->philos)
        free(data->philos);
        
    pthread_mutex_destroy(&data->print_mutex);
}

int main(int argc, char **argv)
{
    t_data data;
    
    if (argc != 5 && argc != 6)
    {
        printf("Usage: %s number_of_philosophers time_to_die time_to_eat"
               " time_to_sleep [number_of_times_each_philosopher_must_eat]\n", argv[0]);
        return 1;
    }
    
    if (init_data(&data, argc, argv) || init_forks(&data) || 
        init_philos(&data) || start_simulation(&data))
    {
        cleanup(&data);
        return 1;
    }
    
    cleanup(&data);
    return 0;
}
