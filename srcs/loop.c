/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ythomas <ythomas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/29 21:27:43 by ythomas           #+#    #+#             */
/*   Updated: 2021/05/26 12:58:28 by ythomas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "ft_ping.h"

uint16_t	checksum(void *addr, int size) {
	uint16_t	*buff;
	uint32_t	sum;

	buff = (uint16_t *)addr;
	for (sum = 0; size > 1; size -= 2)
		sum += *buff++;
	if (size == 1)
		sum += *(uint8_t*)buff;
	sum = (sum >> 16) + (sum & 0xFFFF);
	sum += (sum >> 16);
	return (~sum);
}

void        init_message(t_env *env, echo_message_t *ec_mes, time_t timestamp)
{
    int     data_size;
    char    *data_string;

    env->nb_sent++;
    ec_mes->type = ECHO_REQUEST;
    ec_mes->code = 0;
    ec_mes->checksum = 0;
    ec_mes->id = swap16((uint16_t)getpid());
    ec_mes->seq_num = swap16(0);
    ec_mes->timestamp = (time_t)swap64((uint64_t)timestamp);

    data_size = ECHO_MAX_DATA_SIZE - sizeof(timestamp);

    if (!(data_string = malloc(data_size)))
    {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }

    generate_data(&data_string, data_size);
    memcpy(ec_mes->data, data_string, data_size);
    free(data_string);
    ec_mes->checksum = checksum(ec_mes, sizeof(*ec_mes));

}

void        prepare_reply(rep_message_t *rep_mes)
{
    rep_mes->msgh.msg_name = NULL;
    rep_mes->msgh.msg_namelen = 0;
    rep_mes->msgh.msg_iov = &(rep_mes->iov);
    rep_mes->iov.iov_base = rep_mes->buff;
    rep_mes->iov.iov_len = sizeof(rep_mes->buff);
    rep_mes->msgh.msg_iovlen = 1;
    rep_mes->msgh.msg_control = &(rep_mes->control);
    rep_mes->msgh.msg_controllen = sizeof(rep_mes->control);
    
}

void        rebuild_reply_iphdr(rep_message_t *reply)
{
    int         ip_size;

    reply->reply_ip = (struct ip*)reply->buff;
    ip_size = (int)(reply->reply_ip->ip_hl * 32) / 8;
    reply->reply_icmp = (struct icmp*)(reply->buff + ip_size);
    if (reply->reply_ip->ip_p != IPPROTO_ICMP || sizeof(*(reply->reply_ip)) < sizeof(struct ip))
    {
        fprintf(stderr, "Data structure has been modified, ip header is corrupted\n");
        exit(EXIT_FAILURE);
    }
}

void        ping_loop(t_env *env)
{
    echo_message_t      ec_mes;
    rep_message_t       rep_mes;
    struct timeval      time_val_begin;
    struct timeval      time_val_end;
    int                 rcv_status;
    int                 send_status;
    
    if (gettimeofday(&env->time_start, NULL) == -1)
    {
        fprintf(stderr, "gettimeofday return error\n");
        exit(EXIT_FAILURE);
    }
    display_start_info(env);
    while(1)
    {
        alarm(env->flags.wait_flag);
        env->pass = FALSE;

        if (gettimeofday(&time_val_begin, NULL) == -1)
        {
            fprintf(stderr, "gettimeofday return error\n");
            exit(EXIT_FAILURE);
        }
        init_message(env, &ec_mes, time_val_begin.tv_sec);

        if ((send_status = sendto(env->sock, &ec_mes, sizeof(ec_mes), 0, (struct sockaddr*)&env->dest.sa_in.ipv4, sizeof(env->dest.sa_in.ipv4))) < 0)
        {
            fprintf(stderr, "Send to return an error\n");
            exit(2);
        }


        prepare_reply(&rep_mes);
        rcv_status = recvmsg(env->sock, &(rep_mes.msgh), 0);

        if (rcv_status == 0)
        {   
            perror("recv");
            exit(2);
        }
        else if (rcv_status < 0)
        {
            perror("recv");
            exit(2);
        }
        if (gettimeofday(&time_val_end, NULL) == -1)
        {
            fprintf(stderr, "gettimeofday return error\n");
            exit(EXIT_FAILURE);
        }
        rebuild_reply_iphdr(&rep_mes);
        print_each_ping(env, time_val_begin, time_val_end, rcv_status, &rep_mes);

        if (env->flags.c_flag == TRUE)
        {
            env->count -= 1;
            if (env->count <= 0)
            {
                display_stats();
                exit(EXIT_SUCCESS);
            }
        }
        while (env->pass != TRUE);
    }
}