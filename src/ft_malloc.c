/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iel-amra <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/30 16:53:50 by iel-amra          #+#    #+#             */
/*   Updated: 2021/11/30 18:49:57 by iel-amra         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <stdlib.h>
#include "get_next_line_leaks.h"
int	ft_atoi(const char *str);

void *ft_malloc(size_t size)
{
	static	int	count = 0;
	int			fd;
	char		*line;
	int			max;

	fd = open("tempo/max.txt", O_RDONLY);
	line = get_next_line_leaks(fd);
	close(fd);
	if (!line)
		return ((void *) 0);
	max = atoi(line);
	free(line);
	if (count == max)
		return ((void *) 0);
	count++;
	return (malloc(size));
}
