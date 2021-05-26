/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dest_info.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ythomas <ythomas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/28 17:24:59 by ythomas           #+#    #+#             */
/*   Updated: 2021/05/26 12:57:45 by ythomas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "ft_ping.h"

void        get_dest_hostname(t_env *env)
{
    char        hbuff[NI_MAXHOST]; // NI_MAXHOST = 1025 defined in netdb.h even if not known here
    char        sbuff[NI_MAXSERV]; // NI_MAXSERV = 32 defined in netdb.h even if not known here
    int         err = 1;

    if (env->dest.ip_type == AF_INET)
        err = getnameinfo((struct sockaddr*)&(env->dest.sa_in.ipv4), sizeof(env->dest.sa_in.ipv4), hbuff, sizeof(hbuff), sbuff, sizeof(sbuff), NI_NAMEREQD);
    else
        err = getnameinfo((struct sockaddr*)&(env->dest.sa_in.ipv6), sizeof(env->dest.sa_in.ipv6), hbuff, sizeof(hbuff), sbuff, sizeof(sbuff), NI_NAMEREQD);
    if (err == 0)
        env->dest.domain_name = strdup(hbuff); // No need to protect the strdup bacause if it's NULL i'll not use it
    else
        env->dest.domain_name = NULL;
}


void        get_dest_ip(t_env *env)
{
    struct addrinfo     hints;
    struct addrinfo     *res;
    int                 err;
    const char          *ntop_err = NULL;
    struct addrinfo     *tmp;
    
    memset(&hints, 0, sizeof(hints));

    if (env->flags.ipv4 == TRUE && env->flags.ipv6 == FALSE)
        hints.ai_family = AF_INET;
    else if (env->flags.ipv6 == TRUE && env->flags.ipv4 == FALSE)
        hints.ai_family = AF_INET6;
    else
        hints.ai_family = AF_UNSPEC; // Deal with ipv4 and ipv6, no need to specify the socketype
    hints.ai_socktype = SOCK_RAW; // To avoid to do to much iteration in the while loop
    
    if ((err = getaddrinfo(env->dest.given_input, NULL, &hints, &res)) != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(err)); // gestion d'erreur de getaddrinfo
        exit(EXIT_FAILURE);
    }

    tmp = res;
    while (tmp != NULL)
    {
        if (tmp->ai_family == AF_INET)
        {
            
            memcpy(&(env->dest.sa_in.ipv4), (void *)tmp->ai_addr, sizeof(struct sockaddr_in)); // copy sockaddr struct in case for later (maybe ca be removes)
            ntop_err = inet_ntop(AF_INET, &(env->dest.sa_in.ipv4.sin_addr), env->dest.ip, INET_ADDRSTRLEN);
            env->dest.ip_type = AF_INET;
            break;
        }
        else
        {
            memcpy(&(env->dest.sa_in.ipv6), (void *)tmp->ai_addr, sizeof(struct sockaddr_in6)); // copy sockaddr struct in case for later (maybe ca be removes)
            ntop_err = inet_ntop(AF_INET6, &(env->dest.sa_in.ipv6.sin6_addr), env->dest.ip, INET6_ADDRSTRLEN);
            env->dest.ip_type = AF_INET6;
            break;
        }
        tmp = tmp->ai_next;
    }
    if (!ntop_err)
    {
        fprintf(stderr, "Can't find address associated with [%s]\n", env->dest.given_input);
        exit(EXIT_FAILURE);
    }
    freeaddrinfo(res);
}



void        get_dest_info(t_env *env)
{
    get_dest_ip(env); // First get the ip address in env->dest.ip
    get_dest_hostname(env); // Get the domain name
}