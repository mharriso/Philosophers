#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
//#include "philo.h"

#define FREE 0
#define LOCK 1

typedef struct s_info
{
	int				num_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				must_eat;
	int				finished_philos;
	int				end;
	int				*fork_status;
	size_t			start;
	pthread_mutex_t	mtx_end;
	pthread_mutex_t	*forks;
}	t_info;

typedef struct s_philo
{
	t_info	*info;
	int		r_fork;
	int		l_fork;
	int		num;
	int		num_eat;
	size_t	last_eat;
}	t_philo;

size_t	get_useconds(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000000 + time.tv_usec);
}

void	ft_usleep(useconds_t time)
{
	size_t	t;

	t = get_useconds();
	while (get_useconds() - t < time)
		usleep(1);
}

int	ft_atoi(char *str)
{
	long		number;

	while (*str == ' ' || (*str >= '\t' && *str <= '\r'))
		str++;
	if (*str == '-')
		return (-1);
	else if (*str == '+')
		str++;
	number = 0;
	while (*str >= '0' && *str <= '9')
	{
		if (number > 2147483647)
			return (-1);
		number = number * 10 + *str - '0';
		str++;
	}
	return (number);
}

int	save_args(int argc, char **argv, t_info *info)
{
	if (argc != 5 && argc != 6)
	{
		write(2, "Invalid args\n", 14);
		return (0);
	}
	info->num_philos = ft_atoi(argv[1]);
	info->time_to_die = ft_atoi(argv[2]);
	info->time_to_eat = ft_atoi(argv[3]);
	info->time_to_sleep = ft_atoi(argv[4]);
	info->must_eat = -1;
	if (argc == 6)
		info->must_eat = ft_atoi(argv[5]);
	if (info->num_philos < 1 || info->time_to_die < 1 || info->time_to_eat < 1 \
		|| info->time_to_sleep < 1 || (argc == 6 && info->must_eat < 2))
	{
		write(2, "Invalid args\n", 14);
		return (0);
	}
	return (1);
}

void	print_philo_status(t_philo *philo, char *status)
{
	pthread_mutex_lock(&philo->info->mtx_end);
	pthread_mutex_unlock(&philo->info->mtx_end);
	printf("%zu %d %s\n", (get_useconds() - philo->info->start) / 1000, \
	philo->num, status);
}

void	check_philo_must_eat(t_philo *philo)
{
	if (philo->info->must_eat > 0 && ++philo->num_eat == philo->info->must_eat)
	{
		if (++philo->info->finished_philos == philo->info->num_philos)
		{
			philo->info->end = 1;
			pthread_mutex_lock(&philo->info->mtx_end);
		}
	}
}

void	philo_eat(t_philo *philo)
{
	while (1)
	{
		if (philo->info->fork_status[philo->r_fork] == FREE)
		{
			pthread_mutex_lock(&philo->info->forks[philo->r_fork]);
			philo->info->fork_status[philo->r_fork] = LOCK;
			print_philo_status(philo, "has taken a fork");
			pthread_mutex_lock(&philo->info->forks[philo->l_fork]);
			philo->info->fork_status[philo->l_fork] = LOCK;
			print_philo_status(philo, "has taken a fork");
			philo->last_eat = get_useconds();
			print_philo_status(philo, "is eating");
			ft_usleep((philo->info->time_to_eat * 1000));
			check_philo_must_eat(philo);
			philo->info->fork_status[philo->r_fork] = FREE;
			philo->info->fork_status[philo->l_fork] = FREE;
			pthread_mutex_unlock(&philo->info->forks[philo->r_fork]);
			pthread_mutex_unlock(&philo->info->forks[philo->l_fork]);
			break ;
		}
		else
			ft_usleep(500);
	}
}

void	philo_sleep(t_philo *philo)
{
	print_philo_status(philo, "is sleeping");
	ft_usleep((philo->info->time_to_sleep * 1000));
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
	return (NULL);
}

int	init_philo(t_info *info, t_philo *philo)
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
		philo[i].last_eat = info->start;
		philo[i].num_eat = 0;
		i++;
	}
	return (0);
}

int	init_mutex_forks(t_info *info)
{
	int	i;

	i = 0;
	while (i < info->num_philos)
	{
		if (pthread_mutex_init(&info->forks[i], NULL))
		{
			write(2, "Init mutex error\n", 18);
			return (1);
		}
		i++;
	}
	return (0);
}

int	init_info(t_info *info)
{
	info->forks = malloc(info->num_philos * sizeof(pthread_mutex_t));
	info->fork_status = malloc(info->num_philos * sizeof(int));
	if (!info->forks || ! info->fork_status)
	{
		write(2, "Malloc error\n", 14);
		return (1);
	}
	info->finished_philos = 0;
	info->end = 0;
	memset(info->fork_status, FREE, sizeof(int) * info->num_philos);
	if (init_mutex_forks(info) || pthread_mutex_init(&info->mtx_end, NULL))
	{
		write(2, "Init mutex error\n", 18);
		return (1);
	}
	info->start = get_useconds();
	return (0);
}

int	destroy_mutex_forks(t_info *info)
{
	int	i;

	i = 0;
	while (i < info->num_philos)
	{
		pthread_mutex_destroy(&info->forks[i]);
		i++;
	}
	return (0);
}

void	check_philo(t_philo *philo)
{
	if ((get_useconds() - philo->last_eat) / 1000 > \
	(size_t)philo->info->time_to_die)
	{
		philo->info->end = 1;
		print_philo_status(philo, "died");
		pthread_mutex_lock(&philo->info->mtx_end);
	}
}

void	check_philos_death(t_philo *philo, t_info *info)
{
	int	i;

	while (!info->end)
	{
		i = 0;
		ft_usleep(3000);
		while (!info->end && i < info->num_philos)
		{
			check_philo(&philo[i]);
			i++;
		}
	}
	return ;
}

void	kill_philos(pthread_t *threads, t_info *info, t_philo *philo)
{
	int	i;

	i = 0;
	while (i < info->num_philos)
	{
		pthread_detach(threads[i]);
		i++;
	}
	pthread_mutex_destroy(&info->mtx_end);
	destroy_mutex_forks(info);
	free(info->fork_status);
	free(info->forks);
	free(info);
	free(threads);
	free(philo);
}

int	start_philos(pthread_t *threads, t_info *info, t_philo *philo)
{
	int	i;

	if (!threads)
	{
		write(2, "Malloc error\n", 14);
		return (1);
	}
	i = 0;
	while (i < info->num_philos)
	{
		if (pthread_create(&threads[i], NULL, philo_life, &philo[i]))
		{
			write(2, "Pthread create error\n", 22);
			return (1);
		}
		ft_usleep(100);
		i++;
	}
	return (0);
}

int	main(int argc, char **argv)
{
	t_philo		*philo;
	t_info		*info;
	pthread_t	*threads;

	threads = NULL;
	info = malloc(sizeof(t_info));
	philo = malloc(info->num_philos * sizeof(t_philo));
	threads = malloc(info->num_philos * sizeof(pthread_t));
	if (!info || !philo || !save_args(argc, argv, info) || \
	init_info(info) || init_philo(info, philo))
		return (1);
	if (start_philos(threads, info, philo))
		return (1);
	check_philos_death(philo, info);
	kill_philos(threads, info, philo);
	return (0);
}
