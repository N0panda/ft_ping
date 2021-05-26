/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ythomas <ythomas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/27 12:53:07 by ythomas           #+#    #+#             */
/*   Updated: 2021/05/26 12:31:29 by ythomas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PING_H
# define FT_PING_H

# include <unistd.h>
# include <signal.h>
# include <netinet/in.h>
# include <netinet/ip.h>
# include <netinet/ip_icmp.h>
# include <stdio.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>
# include <string.h>
# include <stdlib.h>
# include <arpa/inet.h>
# include <getopt.h>
# include <sys/time.h>
# include <math.h>

# define TRUE 1
# define FALSE 0

# define ICMP_UNREACH 3

# define ECHO_MAX_DATA_SIZE 56 //64 - sizeof icmp header (8)
# define ECHO_MESSAGE_SIZE  48 // - sizeof time_t
# define PKT_SIZE 84 // rest + ip header
# define MAX_ERROR_STRING 1000

typedef union       u_dest_sockaddr_in {
    struct sockaddr_in      ipv4;
    struct sockaddr_in6     ipv6;
}                   t_sockaddr_inx;

typedef struct      error_string_s {
    char    error_type[MAX_ERROR_STRING];
    char    error_code[MAX_ERROR_STRING];
}                   error_string_t;

enum echo_message_type {
    ECHO_REQUEST = 8,
    ECHO_REPLY   = 0
};

typedef struct s_flags{
    int     h_flag;
    int     v_flag;
    int     c_flag;
    int     wait_flag;
    int     ipv4;
    int     ipv6;
}               t_flags;

typedef struct s_dest{
    int                 ip_type; // Have to be AF_INET or AF_INET6
    char                *given_input; // hostname or ip given in input
    char                *domain_name; // domain_name returned after a reverse dns lookup NULL by default
    char                ip[INET6_ADDRSTRLEN]; // memcopy of the ip address from the sockaddr_in (struct sin_addr)
    t_sockaddr_inx      sa_in;
}               t_dest;

typedef struct  echo_message_s{
    // icmp header
    uint8_t         type;
    uint8_t         code;
    uint16_t        checksum;
    uint16_t        id;
    uint16_t        seq_num;
    time_t          timestamp;
    uint8_t         data[ECHO_MESSAGE_SIZE];
 
}               echo_message_t;

typedef struct  rep_message_s{
    struct iovec    iov;
    struct msghdr   msgh;
    char            buff[84];
    char            control[1000];
    struct ip       *reply_ip;
    struct icmp     *reply_icmp;  
}               rep_message_t;

typedef struct  s_env {
    int             sock;
    unsigned int    ttl;
    uint16_t        nb_sent;
    uint16_t        nb_received;
    t_flags         flags;
    t_dest          dest;

    int             pass;
    int             count;

    double          time_min;
    double          time_max;
    double          time_sum;
    double          time_sum2;

    struct timeval  time_start;
    struct timeval  time_end;
}               t_env;

t_env           env;



int             set_options(int ac, char **av, t_env *env);
void            print_usage(void);

void            get_dest_info(t_env *env);
int             setup_socket(t_env *env); // return socket file descripto
void            ping_loop(t_env *env);

void            generate_data(char **str, int length);

void            display_start_info(t_env *env);
void            print_each_ping(t_env *env, struct timeval start, struct timeval end, int bytes_read, rep_message_t *reply);
void            display_stats(void);

uint16_t    swap16(uint16_t nb);
uint32_t    swap32(uint32_t nb);
uint64_t    swap64(uint64_t nb);

#endif