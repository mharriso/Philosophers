/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   end.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mharriso <mharriso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/23 23:00:17 by mharriso          #+#    #+#             */
/*   Updated: 2021/06/23 23:02:13 by mharriso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	destroy_mutex_forks(t_info *info)
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

static void	check_philo(t_philo *philo)
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
		ft_usleep(2000);
		i = 0;
		while (!info->end && i < info->num_philos)
		{
			check_philo(&philo[i]);
			i++;
		}
	}
	return ;
}
