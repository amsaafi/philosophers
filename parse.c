/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samsaafi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 15:19:46 by samsaafi          #+#    #+#             */
/*   Updated: 2024/11/18 19:58:38 by samsaafi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int is_number(char *str)
{
    int i;

    i = 0;
    while (str[i])
    {
        if (str[i] < '0' || str[i] > '9')
            return (0);
        i++;
    }
    return (1);
}

int is_valid_int(char *str)
{
    long result;
    int i;
    
    result = 0;
    i = 0;
    while (str[i])
    {
        result = result * 10 + (str[i] - '0');
        if (result > 2147483647)
            return (0);
        i++;
    }
    return (1);
}

int parse_args(int argc, char **argv)
{
    int i;

    i = 1;
    while (i < argc)
    {
        if (!is_number(argv[i]))
        {
            printf("Error: arguments must be positive numbers only\n");
            return (1);
        }
        if (!is_valid_int(argv[i]))
        {
            printf("Error: number exceeds INT_MAX\n");
            return (1);
        }
        if (atoi(argv[i]) <= 0)
        {
            printf("Error: arguments must be positive\n");
            return (1);
        }
        i++;
    }
    return (0);
}
