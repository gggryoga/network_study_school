#include "dg.h"

// #include <stdio.h>
// #include <stdlib.h>
// #include <sys/types.h>
// #include <sys/socket.h> /* 繧ｽ繧ｱ繝�ヨ縺ｮ縺溘ａ縺ｮ蝓ｺ譛ｬ逧�↑繝倥ャ繝繝輔ぃ繧､繝ｫ      */
// #include <netinet/in.h> /* 繧､繝ｳ繧ｿ繝阪ャ繝医ラ繝｡繧､繝ｳ縺ｮ縺溘ａ縺ｮ繝倥ャ繝繝輔ぃ繧､繝ｫ  */
// #include <netdb.h>      /* gethostbyname()繧堤畑縺�ｋ縺溘ａ縺ｮ繝倥ャ繝繝輔ぃ繧､繝ｫ */
// #include <errno.h>
// #include <string.h>
// #define  MAXHOSTNAME    64
// #define	 S_UDP_PORT	(u_short)5000 
// #define  MAXKEYLEN	128
// #define	 MAXDATALEN	256  
// int setup_dgclient(struct hostent*, u_short, struct sockaddr_in*, int*);
// void remote_dbsearch(int, struct sockaddr_in*, int);


int main()
{
	int	socd;
	char	s_hostname[MAXHOSTNAME];
	struct hostent	*s_hostent;
	struct sockaddr_in s_address;
	int	s_addrlen;
    int select;


	printf("server host name?: "); scanf("%s",s_hostname);
	if((s_hostent = gethostbyname(s_hostname)) == NULL) {
		fprintf(stderr, "server host does not exists\n");
		exit(1);
	}

	socd = setup_dgclient(s_hostent, S_UDP_PORT, &s_address, &s_addrlen);
    
	printf("1) search?\n2) add key?: ");
    if (scanf("%d", &select) == 1)
    {
        if (select == 1)
        {
            remote_dbsearch(socd, &s_address, s_addrlen);
        }
        else if (select == 2)
        {
            remote_dbadd(socd, &s_address, s_addrlen);
        }
        else
        {
            printf("Invalid selection\n");
        }
        close(socd);
        exit(0);
    }
    else
    {
        printf("error\n");
        exit(1);
    }
}

int setup_dgclient(struct hostent *hostent, u_short port, struct sockaddr_in *s_addressp, int *s_addrlenp)
{
        int     socd;
        struct sockaddr_in      c_address;
 
	/* 繧､繝ｳ繧ｿ繝ｼ繝阪ャ繝医ラ繝｡繧､繝ｳ縺ｮSOCK_DGRAM(UDP)蝙九た繧ｱ繝�ヨ縺ｮ讒狗ｯ� */
        if((socd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) { perror("socket");exit(1); }
 
	/* 繧ｵ繝ｼ繝舌�繧｢繝峨Ξ繧ｹ(Internet繧｢繝峨Ξ繧ｹ縺ｨ繝昴�繝育分蜿ｷ)縺ｮ菴懈� */
        bzero((char *)s_addressp, sizeof(*s_addressp));
        s_addressp->sin_family = AF_INET;
        s_addressp->sin_port = htons(port);
        bcopy((char *)hostent->h_addr, (char *)&s_addressp->sin_addr, hostent->h_length);
	*s_addrlenp = sizeof(*s_addressp);

	/* 繧ｯ繝ｩ繧､繧｢繝ｳ繝医�繧｢繝峨Ξ繧ｹ(Internet繧｢繝峨Ξ繧ｹ縺ｨ繝昴�繝育分蜿ｷ)縺ｮ菴懈� */
        bzero((char *)&c_address, sizeof(c_address));
        c_address.sin_family = AF_INET;
        c_address.sin_port = htons(0);		       /* 繝昴�繝育分蜿ｷ縺ｮ閾ｪ蜍募牡繧雁ｽ薙※ */
	c_address.sin_addr.s_addr = htonl(INADDR_ANY); /* Internet繧｢繝峨Ξ繧ｹ縺ｮ閾ｪ蜍募牡繧雁ｽ薙※ */

	/* 繧ｯ繝ｩ繧､繧｢繝ｳ繝医い繝峨Ξ繧ｹ縺ｮ繧ｽ繧ｱ繝�ヨ縺ｸ縺ｮ蜑ｲ繧雁ｽ薙※ */
	if(bind(socd, (struct sockaddr *)&c_address, sizeof(c_address)) < 0) { perror("bind");exit(1); }

	return socd;
}

// char separate_key(t_data *key_data, char *key, int key_num)
// {
//     int i = 0;
//     int j = 0;
    

//     // キーの数だけメモリを割り当て
//     for (i = 0; i < key_num; i++) {
//         key_data->key_array[i] = (char *)malloc(MAXKEYLEN + 1);
//     }

//     i = 0;
//     while (key[i] != '\0')
//     {
//         if (key[i] == ',')
//         {
//             key_data->key_array[j][i] = '\0';
//             j++;
//             i = 0;
//         }
//         else
//         {
//             key_data->key_array[j][i] = key[i];
//             i++;
//         }
//     }
//     key_data->key_array[j][i] = '\0';

//     return 0;
// }

int check_key(char *key)
{
    int	i;
    int ans = 1;

    while (key[i] != '\0')
    {
        if (key[i] == ',') ans++;
        i++;
    }
    return ans;
}

void remote_dbadd(int socd, struct sockaddr_in *s_addressp, socklen_t s_addrlen)
{
    char new_key[MAXKEYLEN + 1];
    char new_data[MAXDATALEN + 1];

    printf("Enter new key: ");
    scanf("%s", new_key);

    printf("Enter new data: ");
    scanf("%s", new_data);

    // サーバーに新しい名前とデータを送信
    if (sendto(socd, new_key, strlen(new_key), 0, (struct sockaddr *)s_addressp, s_addrlen) != strlen(new_key))
    {
        fprintf(stderr, "datagram error\n");
        exit(1);
    }

    if (sendto(socd, new_data, strlen(new_data), 0, (struct sockaddr *)s_addressp, s_addrlen) != strlen(new_data))
    {
        fprintf(stderr, "datagram error\n");
        exit(1);
    }

    printf("New key and data sent to the server.\n");
}

void remote_dbsearch(int socd, struct sockaddr_in *s_addressp, socklen_t s_addrlen)
{
	char	key[MAXKEYLEN+1], data[MAXDATALEN+1];
	int	keylen, datalen;
    int key_num;
    // char **key_array;
    int i =0;
    // t_data key_data;
    char *some_key;

	/* 繧ｭ繝ｼ繧呈ｨ呎ｺ門�蜉帙°繧牙�蜉� */
    while (1)
    {
        printf("key?: ");
        /* 繧ｭ繝ｼ繧偵た繧ｱ繝�ヨ縺ｫ譖ｸ縺崎ｾｼ繧 */
        if (scanf("%s",key) == EOF) break;
        // printf("hi");
        keylen = strlen(key);
        key_num = check_key(key);
        if (key_num != 1)
        {
            // separate_key(&key_data, key, key_num);
            while (i < key_num)
            {
                // printf("%d",key_num);
                some_key = strtok(key, ",");
                keylen = strlen(some_key);
                if(sendto(socd, some_key, keylen, 0, (struct sockaddr *)s_addressp, s_addrlen) != keylen) 
                {
                    fprintf(stderr, "datagram error\n");
                    exit(1);
                }
                /* 讀懃ｴ｢繝��繧ｿ繧偵た繧ｱ繝�ヨ縺九ｉ隱ｭ縺ｿ霎ｼ繧 */
                if((datalen = recvfrom(socd, data, MAXDATALEN, 0, NULL, &s_addrlen)) < 0) { 
                    perror("recvfrom");
                    exit(1);
                }
                /* 繝��繧ｿ繧呈ｨ呎ｺ門�蜉帙↓蜃ｺ蜉� */
                data[datalen] = '\0';
                fputs("data: ",stdout);puts(data);
                memmove(key, key + keylen + 1, strlen(key) - keylen + 1);
                i++;
            }
        }
        else
        {
            printf("hi2");
            if(sendto(socd, key, keylen, 0, (struct sockaddr *)s_addressp, s_addrlen) != keylen) {
                fprintf(stderr, "datagram error\n");
                exit(1);
            }
            /* 讀懃ｴ｢繝��繧ｿ繧偵た繧ｱ繝�ヨ縺九ｉ隱ｭ縺ｿ霎ｼ繧 */
            if((datalen = recvfrom(socd, data, MAXDATALEN, 0, NULL, &s_addrlen)) < 0) { 
                perror("recvfrom");
                exit(1);
            }
            /* 繝��繧ｿ繧呈ｨ呎ｺ門�蜉帙↓蜃ｺ蜉� */
            data[datalen] = '\0';
            fputs("data: ",stdout);puts(data);
        }

    }
}