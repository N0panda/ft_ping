/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   diplay.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ythomas <ythomas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/21 12:46:40 by ythomas           #+#    #+#             */
/*   Updated: 2021/05/26 12:57:55 by ythomas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

static const char *type_error[] = {
    "Echo Reply", //0
    "None",
    "None",
    "Dest unreachable", // 3
    "Packet lost, slow down", // 4
    "Shorter route", // 5
    "None",
    "None",
    "Echo service", // 8
    "Router advertisement", // 9
    "Router solicitation", // 10
    "Time to live exceeded", // 11
    "IP header bad", // 12
    "timestamp request", // 13
    "timestamp reply", // 14
    "information request", // 15
    "information reply", // 16
    "address mask request", //17
    "address mask reply" // 18
};

static const char *unreach_code[] = {
    "(Bad net)",
    "(Bad host)",
    "(Bad protocol)",
    "(Bad port)",
    "(IP_DF cause drop)",
    "(Src routee failed)",
    "(Unknow net)",
    "(Unknow host)",
    "(Src host isolated)",
    "(Net denied)",
    "(Host denied)",
    "(Bad tos for net)",
    "(Bad tos for host)",
    "(Admin prohib)",
    "(Host prec vio.)",
    "(Prec cutoff)"
};

static const char *redirect_code[] = {
    "(For network)",
    "(For host)",
    "(For tos and net)",
    "(For tos and host)"
};

static const char *timexceed_code[] = {
    "(Ttl==0 in transit)",
    "(Ttl==0 in reass)"
};

static const char *paramprob_code[] = {
    "(Req. opt. absent)"
};

void        get_error_string(error_string_t *error, int type, int code, t_flags flags)
{
    if (type < 0 || type > 18)
    {
        printf("Type error, (type doesn't correspond to any type of ICMP)");
        exit(EXIT_FAILURE);
    }
    memcpy(error->error_type, type_error[type], strlen(type_error[type]));
    
    if (flags.v_flag == TRUE)
    {
        if (type == ICMP_UNREACH)
            memcpy(error->error_code, unreach_code[code], strlen(unreach_code[code]));
        else if (type == ICMP_REDIRECT)
            memcpy(error->error_code, redirect_code[code], strlen(redirect_code[code]));
        else if (type == ICMP_TIME_EXCEEDED)
            memcpy(error->error_code, timexceed_code[code], strlen(timexceed_code[code]));
        else if (type == ICMP_PARAMETERPROB)
            memcpy(error->error_code, paramprob_code[code], strlen(paramprob_code[code]));
    }
}

void        display_start_info(t_env *env)
{
    printf("PING %s (%s) %d(%d) bytes of data.\n", env->dest.given_input, env->dest.ip, ECHO_MAX_DATA_SIZE, PKT_SIZE);
}

void        print_each_ping(t_env *env, struct timeval start, struct timeval end, int bytes_read, rep_message_t *reply)
{
    char    tmp_ip[INET_ADDRSTRLEN];
    double  tmp_time;
    
    if (reply->reply_icmp->icmp_type != ICMP_ECHOREPLY)
    {
        error_string_t error;
        bzero(&error, sizeof(error));
        
        if (inet_ntop(AF_INET, &(reply->reply_ip->ip_src.s_addr), tmp_ip, INET_ADDRSTRLEN) == NULL)
        {
            perror("inet_ntop");
            exit(EXIT_FAILURE);
        }
        get_error_string(&error, reply->reply_icmp->icmp_type, reply->reply_icmp->icmp_code, env->flags);

        printf("From %s (%s) icmp_seq=%d %s %s\n",
        tmp_ip,
        tmp_ip,
        env->nb_sent,
        error.error_type,
        error.error_code);
    }
    else{
        tmp_time = ((double)(end.tv_sec - start.tv_sec) * 1000 + (double)(end.tv_usec - start.tv_usec) / 1000);
        env->time_sum += tmp_time;
        env->time_sum2 += (tmp_time * tmp_time);

        if (tmp_time > env->time_max)
            env->time_max = tmp_time;
        if (tmp_time < env->time_min || env->time_min == 0.0)
            env->time_min = tmp_time;

        env->nb_received++;
        printf("%ld bytes from %s (%s): icmp_seq=%d ttl=%d time=%.2f ms\n",
        bytes_read - sizeof(struct ip),
        env->dest.domain_name,
        env->dest.ip,
        env->nb_sent,
        reply->reply_ip->ip_ttl,
        tmp_time);
    }
}

void        display_stats(void)
{
    if (gettimeofday(&env.time_end, NULL) == -1)
        printf("ERROR GETTIMEOFDAY\n");
    printf("\n--- %s ping statistics ---\n", env.dest.given_input);
    int         percent = 100 - ((env.nb_received / env.nb_sent) * 100);
    int      total_time = ((double)(env.time_end.tv_sec - env.time_start.tv_sec) * 1000 + (double)(env.time_end.tv_usec - env.time_start.tv_usec) / 1000);
    if (env.nb_sent == env.nb_received)
    {
        double time_mean = env.time_sum / env.nb_received;
        double time_mean2 = env.time_sum2 / env.nb_received;
        double mdev =  sqrt(time_mean2 - (time_mean * time_mean));

        printf("%d packets transmitted, %d received, %d%% packet loss, time %dms\n", env.nb_sent, env.nb_received, percent, total_time);
        printf("rtt min/avg/max/mdev = %.3f/%.3f/%.3f/%.3f ms\n", env.time_min, time_mean, env.time_max, mdev);
    }
    else
    {
        int nb_err = env.nb_sent - env.nb_received;

        // GERER LE CAS OU ON A LES ERREURS
        printf("%d packets transmitted, %d received, +%d errors, %d%% packet loss, time %dms\n\n", env.nb_sent, env.nb_received, nb_err, percent, total_time);
    }
}