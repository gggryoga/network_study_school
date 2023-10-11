#ifndef DG_H
#define DG_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h> /* 繧ｽ繧ｱ繝�ヨ縺ｮ縺溘ａ縺ｮ蝓ｺ譛ｬ逧�↑繝倥ャ繝繝輔ぃ繧､繝ｫ      */
#include <netinet/in.h> /* 繧､繝ｳ繧ｿ繝阪ャ繝医ラ繝｡繧､繝ｳ縺ｮ縺溘ａ縺ｮ繝倥ャ繝繝輔ぃ繧､繝ｫ  */
#include <netdb.h>      /* gethostbyname()繧堤畑縺�ｋ縺溘ａ縺ｮ繝倥ャ繝繝輔ぃ繧､繝ｫ */
#include <errno.h>
#include <string.h>
#include <unistd.h>
#define  MAXHOSTNAME	64
#define  S_UDP_PORT	(u_short)5000  /* 譛ｬ繧ｵ繝ｼ繝舌′逕ｨ縺�ｋ繝昴�繝育分蜿ｷ */
#define  MAXKEYLEN	128
#define  MAXDATALEN	256

typedef struct s_data
{
    char **key_array;
}t_data;


int setup_dgserver(struct hostent*, u_short);
void db_search(int);
int setup_dgclient(struct hostent*, u_short, struct sockaddr_in*, int*);
void remote_dbsearch(int, struct sockaddr_in*, socklen_t);
void remote_dbadd(int socd, struct sockaddr_in *s_addressp, socklen_t s_addrlen);
void db_edit(int socd);
void remote_dbedit(int socd, struct sockaddr_in *s_addressp, socklen_t s_addrlen);



#endif