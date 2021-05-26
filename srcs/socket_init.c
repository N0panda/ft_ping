/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ythomas <ythomas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/29 12:32:04 by ythomas           #+#    #+#             */
/*   Updated: 2021/05/26 12:59:27 by ythomas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "ft_ping.h"

void        set_socket_options(t_env *env, int sfd)
{
    if (env->dest.ip_type == AF_INET)
    {
        if(setsockopt(sfd, SOL_IP, IP_TTL, &(env->ttl), sizeof(env->ttl)) == -1)
        {
            printf("%d\n",env->ttl);
            fprintf(stderr, "PROBLEME SOCKET OPTIONS\n");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        int     activate = 1;
        if(setsockopt(sfd, IPPROTO_IPV6, IPV6_RECVHOPLIMIT, &activate, sizeof(activate)) == -1)
        {
            fprintf(stderr, "PROBLEME SOCKET OPTIONS\n");
            exit(EXIT_FAILURE);
        }
    }
}

int         setup_socket(t_env *env)
{
    /*
        Need sudo to creat a raw socket SOCK_RAW
        IPPROTO_ICMP define the icmp protocol
        IPPROTO_IP define the level of the protocol (netinet/in.h)
    */
    int     sfd;

    if (env->dest.ip_type == AF_INET)
    {
        if ((sfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) == -1)
        {
            fprintf(stderr, "Socket creation failed for ipv4 !\n");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        if((sfd = socket(AF_INET6, SOCK_RAW, IPPROTO_ICMPV6)) == -1)
        {
            fprintf(stderr, "Socket creation failed for ipv6 !\n");
            exit(EXIT_FAILURE);
        }
    }
    set_socket_options(env, sfd);
    return sfd;
}