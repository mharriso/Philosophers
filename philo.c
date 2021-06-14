#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>

#define NUM_THREADS 20

pthread_mutex_t	mutex;

typedef struct s_info
{
	long	start;
	int		num_philos;
	int		time_to_die;
	int		time_to_eat;
	int		time_to_sleep;
	int		num_eat;
}	t_info;

typedef	struct s_philo
{
	t_info	*info;
	int		num;
}	t_philo;

long	get_useconds(void)
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

void	*print_num(void *arg)
{
	pthread_mutex_lock(&mutex);
	printf("%ld %d has taken a fork\n", (get_useconds() - ((t_philo *)arg)->info->start) / 1000, ((t_philo *)arg)->num);
	usleep(1000);
	pthread_mutex_unlock(&mutex);
	return NULL;
}

int	save_args(int argc, char **argv, t_info *info)
{
	if (argc != 5 && argc != 6)
		return (0);
	info->num_philos = ft_atoi(argv[1]);
	info->time_to_die = ft_atoi(argv[2]);
	info->time_to_eat = ft_atoi(argv[3]);
	info->time_to_sleep = ft_atoi(argv[4]);
	info->num_eat = -1;
	if (info->num_philos < 1 || info->time_to_die < 1 || \
		info->time_to_eat < 1 || info->time_to_sleep < 1)
		return (0);
	if (argc == 6)
	{
		info->num_eat = ft_atoi(argv[5]);
		if (info->num_eat < 2)
			return (0);
	}
	return (1);
}

int	main (int argc, char **argv)
{
	t_philo		philo[NUM_THREADS];
	t_info		info;
 	pthread_t	threads[NUM_THREADS];
	long		start;

	pthread_mutex_init(&mutex, NULL);
	if(!save_args(argc, argv, &info))
		{
			write(1, "Invalid args\n", 14);
			return (1);
		}
	info.start = get_useconds();
	for (size_t i = 0; i < NUM_THREADS; i++)
	{
		philo[i].num = i + 1;
		philo[i].info = &info;
		pthread_create(&threads[i], NULL, print_num, &philo[i]);
		usleep(1);
	}
	for (size_t i = 0; i < NUM_THREADS; i++)
	{
		pthread_join(threads[i], NULL);
	}
	pthread_mutex_destroy(&mutex);
	return 0;
}
