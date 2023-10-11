#include "dg.h"

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

	printf("GET or PUT ");

    remote_dbsearch(socd, &s_address, s_addrlen);
    close(socd);
    exit(0);
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

void remote_dbsearch(int socd, struct sockaddr_in *s_addressp, socklen_t s_addrlen) /* 繧ｵ繝ｼ繝舌↓繧ｭ繝ｼ繧帝√ｊ讀懃ｴ｢邨先棡(繝��繧ｿ)繧貞女縺大叙繧� */
{
	char	key[MAXKEYLEN+1], data[MAXDATALEN+1];
	int	keylen, datalen;

	/* 繧ｭ繝ｼ繧呈ｨ呎ｺ門�蜉帙°繧牙�蜉� */
    while (1)
    {
        printf("key?: ");
        /* 繧ｭ繝ｼ繧偵た繧ｱ繝�ヨ縺ｫ譖ｸ縺崎ｾｼ繧 */
        if (scanf("%s",key) == EOF) break;
        keylen = strlen(key);
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
