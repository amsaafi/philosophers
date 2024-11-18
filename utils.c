/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samsaafi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 15:26:51 by samsaafi          #+#    #+#             */
/*   Updated: 2024/11/18 15:32:24 by samsaafi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*ft_memset(void *ptr, int value, size_t num)
{
	unsigned char	*byte_ptr;
	size_t			i;

	byte_ptr = (unsigned char *)ptr;
	i = 0;
	while (i < num)
	{
		byte_ptr[i] = (unsigned char)value;
		i++;
	}
	return (ptr);
}

long long get_time(void)
{
    struct timeval tv;
    
    gettimeofday(&tv, NULL);
    return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

int	ft_atoi(const char *nptr)
{
	size_t	i;
	int		sign;
	int		num;

	i = 0;
	sign = 1;
	while ((nptr[i] >= 9 && nptr[i] <= 13) || (nptr[i]) == 32)
		i++;
	if (nptr[i] == '-' || nptr[i] == '+')
	{
		if (nptr[i] == '-')
			sign *= -1;
		i++;
	}
	num = 0;
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		num = num * 10;
		num = num + nptr[i] - '0';
		i++;
	}
	return (num * sign);
}

