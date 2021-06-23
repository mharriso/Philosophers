/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mharriso <mharriso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/23 22:42:21 by mharriso          #+#    #+#             */
/*   Updated: 2021/06/23 23:46:42 by mharriso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <unistd.h>
# include <string.h>
# include <sys/time.h>

# define FREE 0
# define LOCK 1

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

size_t	get_useconds(void);
void	ft_usleep(useconds_t time);
int		ft_atoi(char *str);
void	print_philo_status(t_philo *philo, char *status);
void	*philo_life(void *arg);
void	philo_think(t_philo *philo);
void	philo_think(t_philo *philo);
void	philo_eat(t_philo *philo);
int		save_args(int argc, char **argv, t_info *info);
t_philo	*init_philo(t_info *info);
int		init_info(t_info *info);
void	check_philos_death(t_philo *philo, t_info *info);
void	kill_philos(pthread_t *threads, t_info *info, t_philo *philo);

#endif
