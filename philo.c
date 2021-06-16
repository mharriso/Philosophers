#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include "philo.h"


#define FREE 0
#define LOCK 1

pthread_mutex_t	mutex;

typedef struct s_info
{
	int				num_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				must_eat;
	int				end;
	int				finished_philos;
	int				*fork_status;
	size_t			start;
	pthread_mutex_t	*forks;
}	t_info;

typedef	struct s_philo
{
	t_info	*info;
	int		r_fork;
	int		l_fork;
	int		num;
	int		num_eat;
}	t_philo;

size_t	get_useconds(void)
{
	struct	timeval	time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000000 + time.tv_usec);
}

int	ft_atoi(char *str)
{
	long		number;
	int			sign;

	sign = 1;
	while (*str == ' ' || (*str >= '\t' && *str <= '\r'))
		str++;
	if (*str == '-')
	{
		sign = -1;
		str++;
	}
	else if (*str == '+')
		str++;
	number = 0;
	while (*str >= '0' && *str <= '9')
	{
		if (number > 2147483647 && sign == 1)
			return (-1);
		if (number > (long)2147483647 + 1 && sign == -1)
			return (-1);
		number = number * 10 + *str - '0';
		str++;
	}
	return (sign * number);
}

int	save_args(int argc, char **argv, t_info *info)
{
	if (argc != 5 && argc != 6)
		return (0);
	info->num_philos = ft_atoi(argv[1]);
	info->time_to_die = ft_atoi(argv[2]);
	info->time_to_eat = ft_atoi(argv[3]);
	info->time_to_sleep = ft_atoi(argv[4]);
	info->must_eat = -1;
	if (info->num_philos < 1 || info->time_to_die < 1 || \
		info->time_to_eat < 1 || info->time_to_sleep < 1)
		return (0);
	if (argc == 6)
	{
		info->must_eat = ft_atoi(argv[5]);
		if (info->must_eat < 2)
			return (0);
	}
	return (1);
}

void	print_philo_status(t_philo *philo, char *status)
{
	printf("%ld %d %s\n", (get_useconds() - philo->info->start) / 1000, philo->num, status);
}


void	philo_eat(t_philo *philo)
{
	while (1)
	{
		if (philo->info->fork_status[philo->r_fork] == FREE )
		{
			pthread_mutex_lock(&philo->info->forks[philo->r_fork]);
			pthread_mutex_lock(&philo->info->forks[philo->l_fork]);
			philo->info->fork_status[philo->r_fork] = LOCK;
			philo->info->fork_status[philo->l_fork] = LOCK;
			print_philo_status(philo, "has taken a fork");
			print_philo_status(philo, "has taken a fork");
			print_philo_status(philo, "is eating");
			if (++philo->num_eat == philo->info->must_eat)
				if(++philo->info->finished_philos == philo->info->num_philos)
					philo->info->end = 1;
			usleep((philo->info->time_to_eat * 1000));
			philo->info->fork_status[philo->r_fork] = FREE;
			philo->info->fork_status[philo->l_fork] = FREE;
			pthread_mutex_unlock(&philo->info->forks[philo->r_fork]);
			pthread_mutex_unlock(&philo->info->forks[philo->l_fork]);
			break ;
		}
		else
			usleep(1);
	}
}

void	philo_sleep(t_philo *philo)
{
	print_philo_status(philo, "is sleeping");
	usleep((philo->info->time_to_sleep * 1000));
}

void	philo_think(t_philo *philo)
{
	print_philo_status(philo, "is thinking");
}

void	*philo_life(void *arg)
{
	while (1)
	{
		philo_eat((t_philo *)arg);
		philo_sleep((t_philo *)arg);
		philo_think((t_philo *)arg);
	}
	return NULL;
}

void	init_philo(t_info *info, t_philo *philo)
{
	int		i;

	i = 0;
	while (i < info->num_philos)
	{
		philo[i].num = i + 1;
		philo[i].info = info;
		philo[i].l_fork = i;
		if (i == 0)
			philo[i].r_fork = info->num_philos - 1;
		else
			philo[i].r_fork = i - 1;
		philo[i].num_eat = 0;
		i++;
	}
}
int	init_mutex_forks(t_info *info)
{
	int	i;

	i = 0;
	while (i < info->num_philos)
	{
		pthread_mutex_init(&info->forks[i], NULL);
		i++;
	}
	return (0);
}
int	main (int argc, char **argv)
{
	t_philo		*philo;
	t_info		info;
	pthread_t	*threads;

	if(!save_args(argc, argv, &info))
	{
		write(1, "Invalid args\n", 14);
		return (1);
	}
	philo = malloc(info.num_philos * sizeof(t_philo));
	threads = malloc(info.num_philos * sizeof(pthread_t));
	info.forks = malloc(info.num_philos * sizeof(pthread_mutex_t));
	info.fork_status = malloc(info.num_philos * sizeof(int));
	info.finished_philos = 0;
	info.end = 0;
	memset(info.fork_status, FREE, sizeof(int) * info.num_philos);
	init_mutex_forks(&info);
	info.start = get_useconds();
	printf("eat = %d\nsleep = %d\nmust = %d\n", info.time_to_eat, info.time_to_sleep, info.must_eat);
	init_philo(&info, philo);
	for (size_t i = 0; i < info.num_philos; i++)
	{
		pthread_create(&threads[i], NULL, philo_life, &philo[i]);
		usleep(1);
	}
	for (size_t i = 0; i < info.num_philos; i++)
	{
		pthread_join(threads[i], NULL);
	}
	return (0);
}
