/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   options.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ythomas <ythomas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/27 18:13:39 by ythomas           #+#    #+#             */
/*   Updated: 2021/05/26 12:58:39 by ythomas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "ft_ping.h"

void            init_all_flags(t_env *env)
{
    env->flags.v_flag = FALSE;
    env->flags.h_flag = FALSE;
    env->flags.ipv4 = FALSE;
    env->flags.ipv6 = FALSE;
    env->flags.c_flag = FALSE;
    env->flags.wait_flag = 1;
}

void            display_help(void)
{
    printf("Usage : ft_ping [OPTIONS...] URL/IP ...\n"
    "Send ICMP ECHO_REQUEST packets to network hosts.\n\n"
    "   -v          verbose\n"
    "   -h          help\n"
    "   -4          Use ipv4 only\n"
    "   -6          Use ipv6 only\n"
    "   -t          Time to live\n"
    "   -c          Echo Request Count\n"
    "   -w          Wait between echo requests\n");

    exit(0);
}

int             set_options(int ac, char **av, t_env *env)
{
    int             f;
    extern char     *optarg;

    init_all_flags(env);
    while ((f = getopt(ac, av, "hv46t:c:w:")) != -1)
    {
        if (f == 'h')
            display_help();
        else if (f == 'v')
            env->flags.v_flag = TRUE;
        else if (f == '4')
            env->flags.ipv4 = TRUE;
        else if (f == '6')
            env->flags.ipv6 = TRUE;
        else if (f == 't')
            env->ttl = atoi(optarg);
        else if (f == 'c')
        {
            env->count = atoi(optarg);
            env->flags.c_flag = TRUE;
        }
        else if (f == 'w')
            env->flags.wait_flag = atoi(optarg);

        else
            print_usage();
            
        if (env->ttl == 0)
        {
            fprintf(stderr, "ping: can't set unicast time-to-live: Argument invalide\n");
            exit(EXIT_FAILURE);
        }
        else if (env->ttl > 255)
        {
            fprintf(stderr, "ping: ttl %u out of range\n", env->ttl);
            exit(EXIT_FAILURE);
        }
        if (env->flags.c_flag == TRUE && env->count < 1)
        {
            fprintf(stderr, "ping: count have to be at least 1 or more\n");
            exit(EXIT_FAILURE);
        }
        if (env->flags.wait_flag < 1)
        {
            fprintf(stderr, "ping: Waiting time ahve to be minimum 1 second\n");
            exit(EXIT_FAILURE);
        }
        if (env->flags.ipv6 == TRUE)
        {
            fprintf(stderr, "ipv6 opt not suported [TODO]\n");
            exit(EXIT_FAILURE);
        }
    }
    return (optind);
}