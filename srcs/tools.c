/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ythomas <ythomas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/05 13:54:22 by ythomas           #+#    #+#             */
/*   Updated: 2021/05/26 12:57:13 by ythomas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "ft_ping.h"

void        generate_data(char **str, int length)
{
    char    data[26] = "abcdefghijklmnopqrstuvwxyz";

    int i = 0;
    int j = 0;

    while (i < length)
    {
        if (j > 25)
            j = 0;
        (*str)[i] = data[j];
        i++;
        j++;
    }
}

uint16_t    swap16(uint16_t nb)
{
    return ((nb << 8) | (nb >> 8));
}

uint32_t    swap32(uint32_t nb)
{
    nb = ((nb << 8) & 0xFF00FF00) | ((nb >> 8) & 0xFF00FF);
    return ((nb << 16) | (nb >> 16));
}

uint64_t    swap64(uint64_t nb)
{
    nb = ((nb <<8) & 0xFF00FF00FF00FF00ULL) | ((nb >> 8) & 0x00FF00FF00FF00FFULL);
    nb = ((nb <<16) & 0xFFFF0000FFFF0000ULL) | ((nb >> 16) & 0x0000FFFF0000FFFFULL);
    return (nb << 32) | (nb >> 32);
}
