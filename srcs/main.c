/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ythomas <ythomas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/22 16:35:25 by ythomas           #+#    #+#             */
/*   Updated: 2021/05/26 12:58:32 by ythomas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "ft_ping.h"

void            signal_handler_stop(int signum)
{
    (void)signum;
    display_stats();
    exit(EXIT_SUCCESS);
}

void            signal_handler_wait(int signum)
{
    (void)signum;
    env.pass = TRUE;
}

void            print_usage(void)
{
    fprintf(stderr, "Usage : ft_ping [-hv46twc] URL/IP");
    exit(EXIT_FAILURE);
}


int main(int ac, char **av)
{
    int         arg_pos;

    if (ac < 2) {
        print_usage();
    }

    env.ttl = 64;
    arg_pos = set_options(ac, av, &env);
    env.dest.given_input = av[arg_pos];
    env.nb_sent = 0;
    env.nb_received = 0;
    env.time_min = 0.0;
    env.time_max = 0.0;
    env.time_sum2 = 0.0;
    env.time_sum = 0.0;
    env.pass = TRUE;
    get_dest_info(&env);
    env.sock = setup_socket(&env);

    signal(SIGINT, signal_handler_stop);
    signal(SIGALRM, signal_handler_wait);

    ping_loop(&env);

    return(0);
}