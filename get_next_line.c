/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   get_next_line.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: jlensing <jlensing@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/01/08 18:45:55 by jlensing      #+#    #+#                 */
/*   Updated: 2020/06/11 20:22:29 by jlensing      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <limits.h>
#include <unistd.h>

// Finds the index of the first occurence of a char within a string and returns its index

static int	find_char(const char *s)
{
	int i;

	i = 0;
	while (s[i] != '\0')
	{
		if (s[i] == '\n')
			return (i);
		i++;
	}
	return (i);
}

/*
 * See axeLib
 * https://github.com/Axenth/axeLib
 * Temp has been added and whill be freed if flag is set to 1
 */

static char	*ft_strdup(const char *s1, char *temp, int flag)
{
	int		size;
	int		counter;
	char	*dup;

	if (flag == 1)
		free(temp);
	size = 0;
	counter = 0;
	size = ft_strlen(s1);
	dup = malloc(sizeof(char) * size + 1);
	if (dup == NULL)
		return (NULL);
	while (counter < size)
	{
		dup[counter] = s1[counter];
		counter++;
	}
	dup[counter] = '\0';
	return (dup);
}

/*
 * See axeLib
 * https://github.com/Axenth/axeLib
 */

static char	*ft_strrchr(const char *s, int c, char *remainder)
{
	size_t			i;

	i = 0;
	free(remainder);
	while (s[i] != 0)
	{
		if (s[i] == c)
		{
			return (ft_strdup(s + i + 1, NULL, 0));
		}
		i++;
	}
	return (NULL);
}

// Initiates loop

static int	get_next_line_loop(int fd, char **line, char *temp, int ret)
{
	static char		buff[BUFFER_SIZE + 1];				 // Buffer for reading
	static char		*remainder;			  				 // Store remainder if no /n is found or store whatever comes after /n for the next line

	if (remainder != NULL)
		temp = ft_strdup(remainder, temp, 1);			 // If there is something in the remainder copy it to temp
	if (temp == NULL)
		return (-1);									 // If strdup fails return -1
	while (ret > 0)										 // While EOF has not been returned from read
	{
		ret = read(fd, buff, BUFFER_SIZE);				 // Read file with BUFFER_SIZE into buff
		if (ret == -1)
			return (-1);								 // If read fails return -1
		buff[ret] = '\0';								 // terminate the end of the buffer for reading from other functions
		temp = ft_strjoin(temp, buff);					 // Append temp with what is in buff
		if (temp == NULL)
			return (-1);								 // If strjoin fails return -1
		remainder = ft_strrchr(temp, '\n', remainder);   // see if there is an \n if so store whatever is after \n in remainder
		if (remainder != NULL)							 
		{
			*line = ft_substr(temp, 0, find_char(temp)); // if a \n is found get the string from temp up until \n and store this in line
			return ((*line == NULL) ? -1 : 1);			 // return 1 if line has been created succesfully otherwise -1 if substr failed
		}
	}
	*line = ft_substr(temp, 0, ft_strlen(temp));		 // EOF has been reached return whatever is left in temp
	return ((*line == NULL) ? -1 : ret);				 // return 0 if succesfull otherwise return -1 if substr failed
}

/*
 * Main function and error checking
 * Returns -1 if error
 * Returns 0 if EOF
 * Returns 1 if line has been read.
 */

int			get_next_line(int fd, char **line)
{
	int			ret;
	char		*temp;

	temp = ft_strdup("", NULL, 0);
	if (fd < 0 || fd > OPEN_MAX || !line || BUFFER_SIZE < 1 || temp == NULL)
	{
		if (temp != NULL)
			free(temp);
		return (-1);
	}
	ret = get_next_line_loop(fd, line, temp, 1);
	if (ret == -1)
		if (temp != NULL)
			free(temp);
	return (ret);
}
