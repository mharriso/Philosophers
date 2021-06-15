#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>

pthread_mutex_t	mutex;

typedef struct s_info
{
	size_t	start;
	int		num_philos;
	int		time_to_die;
	int		time_to_eat;
	int		time_to_sleep;
	int		num_dinners;
}	t_info;

typedef	struct s_philo
{
	t_info	*info;
	int		right_fork;
	int		left_fork;
	int		num;
	int		dinners;
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
	info->num_dinners = -1;
	if (info->num_philos < 1 || info->time_to_die < 1 || \
		info->time_to_eat < 1 || info->time_to_sleep < 1)
		return (0);
	if (argc == 6)
	{
		info->num_dinners = ft_atoi(argv[5]);
		if (info->num_dinners < 2)
			return (0);
	}
	return (1);
}

void	print_philo_status(t_philo *philo, char *status)
{
	printf("%ld %d %s\n", (get_useconds() - philo->info->start) / 1000, philo->num, status);
}

void	*philo_life(void *arg)
{
	pthread_mutex_lock(&mutex);
	print_philo_status((t_philo *)arg, "has taken a fork");
	usleep(1000);
	pthread_mutex_unlock(&mutex);
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
		philo[i].left_fork = i;
		if (i == 0)
			philo[i].right_fork = info->num_philos - 1;
		else
			philo[i].right_fork = i - 1;
		philo[i].dinners = 0;
		i++;
	}
}

int	main (int argc, char **argv)
{
	t_philo		*philo;
	t_info		info;
	pthread_t	*threads;

	pthread_mutex_init(&mutex, NULL);
	if(!save_args(argc, argv, &info))
	{
		write(1, "Invalid args\n", 14);
		return (1);
	}
	philo = malloc(info.num_philos * sizeof(t_philo));
	threads = malloc(info.num_philos * sizeof(pthread_t));
	info.start = get_useconds();
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
	pthread_mutex_destroy(&mutex);
	return 0;
}
