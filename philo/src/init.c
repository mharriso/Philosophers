/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mharriso <mharriso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/23 22:46:35 by mharriso          #+#    #+#             */
/*   Updated: 2021/06/30 01:00:44 by mharriso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static	int	init_mutex_forks(t_info *info)
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
	memset(info->fork_status, FREE, sizeof(int) * info->num_philos);
	if (!info->forks || ! info->fork_status)
	{
		write(2, "Malloc error\n", 14);
		return (1);
	}
	info->finished_philos = 0;
	info->end = 0;
	if (init_mutex_forks(info) || pthread_mutex_init(&info->mtx_end, NULL))
	{
		write(2, "Init mutex error\n", 18);
		return (1);
	}
	info->start = get_useconds();
	return (0);
}

t_philo	*init_philo(t_info *info)
{
	t_philo		*philo;
	int			i;

	philo = malloc(info->num_philos * sizeof(t_philo));
	if (!philo)
	{
		write(2, "Malloc error\n", 14);
		return (NULL);
	}
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
	return (philo);
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
		|| info->time_to_sleep < 1 || (argc == 6 && info->must_eat < 1))
	{
		write(2, "Invalid args\n", 14);
		return (0);
	}
	return (1);
}
