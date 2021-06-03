/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsuardi <lsuardi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/05 21:59:27 by lsuardi           #+#    #+#             */
/*   Updated: 2021/04/06 21:14:55 by lsuardi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <get_next_line.h>

size_t	_ft_strlen(const char *s)
{
	size_t	len;

	len = 0;
	while (s[len])
		len++;
	return (len);
}

char	*_ft_strdup(const char *s)
{
	char	*new_s;
	size_t	len;
	int		i;

	len = _ft_strlen(s);
	new_s = (char *)malloc(sizeof(char) * (len + 1));
	if (!new_s)
		return (NULL);
	new_s[len] = 0;
	i = -1;
	while (s[++i])
		new_s[i] = s[i];
	return (new_s);
}

/*
**	Replaces the first occurence of the character `before` inside the string `s`
**	by `after`, then it will return either true if a character has been
**	replaced, or false if `before` has not been found in the string.
*/
bool	_ft_str_replace_first(char *s, int before, int after)
{
	char	*it;

	it = s;
	while (*it && (*it != before))
		it++;
	if (*it == before)
	{
		*it = after;
		return (true);
	}
	return (false);
}

char	*_ft_strjoin(const char *prefix, const char *suffix)
{
	size_t	new_len;
	char	*new_s;
	char	*it;

	if (!prefix)
	{
		if (suffix)
			return (_ft_strdup(suffix));
		return (NULL);
	}
	if (!suffix)
		return (_ft_strdup(prefix));
	new_len = _ft_strlen(prefix) + _ft_strlen(suffix);
	new_s = (char *)malloc(sizeof(char) * (new_len + 1));
	if (!new_s)
		return (NULL);
	it = new_s;
	while (*prefix)
		*(it++) = *(prefix++);
	while (*suffix)
		*(it++) = *(suffix++);
	*it = 0;
	return (new_s);
}
