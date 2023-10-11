#include "dg.h"

char *db[] = {"amano-taro","0426-91-9418","ishida-jiro","0426-91-9872",
                 "ueda-saburo","0426-91-9265","ema-shiro","0426-91-7254",
                 "ooishi-goro","0426-91-9618",NULL};

int main(void)
{
	int	socd;
	char	s_hostname[MAXHOSTNAME];
	struct hostent	*s_hostent;

	/* 繧ｵ繝ｼ繝舌�繝帙せ繝亥錐縺ｨ縺昴�Internet繧｢繝峨Ξ繧ｹ(繧偵Γ繝ｳ繝舌↓謖√▽hostent讒矩�菴�)繧呈ｱゅａ繧� */
	gethostname(s_hostname, sizeof(s_hostname));
    printf("server host name =: %s\n", s_hostname);
	s_hostent = gethostbyname(s_hostname);

	/* 繝��繧ｿ繧ｰ繝ｩ繝�繧ｵ繝ｼ繝舌�蛻晄悄險ｭ螳� */
	socd = setup_dgserver(s_hostent, S_UDP_PORT);

	/* 繧ｯ繝ｩ繧､繧｢繝ｳ繝医°繧峨�繝��繧ｿ讀懃ｴ｢隕∵ｱゅ�蜃ｦ逅� */
	db_search(socd);
}

int setup_dgserver(struct hostent *hostent, u_short port)
{
	int	socd;
	struct sockaddr_in	s_address;

	/* 繧､繝ｳ繧ｿ繝ｼ繝阪ャ繝医ラ繝｡繧､繝ｳ縺ｮSOCK_DGRAM(UDP)蝙九た繧ｱ繝�ヨ縺ｮ讒狗ｯ� */
	if((socd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) { perror("socket");exit(1); }

	/* 繧｢繝峨Ξ繧ｹ(Internet繧｢繝峨Ξ繧ｹ縺ｨ繝昴�繝育分蜿ｷ)縺ｮ菴懈� */
	bzero((char *)&s_address, sizeof(s_address));
	s_address.sin_family = AF_INET;
	s_address.sin_port = htons(port);
	bcopy((char *)hostent->h_addr, (char *)&s_address.sin_addr, hostent->h_length);

	/* 繧｢繝峨Ξ繧ｹ縺ｮ繧ｽ繧ｱ繝�ヨ縺ｸ縺ｮ蜑ｲ繧雁ｽ薙※ */
	if(bind(socd, (struct sockaddr *)&s_address, sizeof(s_address)) < 0) { perror("bind");exit(1); }

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

char *return_ans(int key_num, char *key, char **dbp)
{
	char *mario;
	char *ans;
	char *data;

	for (int i = 0; i < key_num; i++)
	{
		if (i != 0){
			strcat(ans, ",");
		}
		mario = strtok(key, ",");
		/* 繧ｭ繝ｼ繧堤畑縺�※繝��繧ｿ讀懃ｴ｢ */
		dbp = db;
		while(*dbp) {
			if(strcmp(mario, *dbp) == 0) {
				strcpy(data, *(++dbp));
				break;
			}
			dbp += 2;
		}
		strncat(ans, data, sizeof(ans) - strlen(ans) - 1);
	}
	return ans;
}

void db_search(int socd) /* 繧ｯ繝ｩ繧､繧｢繝ｳ繝医′繝��繧ｿ讀懃ｴ｢隕∵ｱゅｒ蜃ｦ逅�☆繧� */
{
	struct sockaddr_in	c_address;
    socklen_t c_addrlen;
	char	key[MAXKEYLEN+1], data[MAXDATALEN+1];
	int	keylen, datalen;
    char *ans; // バッファを適切に初期化
	char	**dbp;
    int key_num;
    char *mario;
    int index = 0;
    char *new_key;
	char *new_num;
	char *only_key;
	int flag = 0;


	while(1) {
		/* 繧ｭ繝ｼ繧偵た繧ｱ繝�ヨ縺九ｉ隱ｭ縺ｿ霎ｼ繧 */
		c_addrlen = sizeof(c_address);
		if((keylen = recvfrom(socd, key, MAXKEYLEN, 0, (struct sockaddr *)&c_address, &c_addrlen)) < 0) {
			perror("recvfrom");
			exit(1);
		}
		key[keylen] = '\0';
        key_num = check_key(key);
        if (strncmp(key, "GET:", 4) == 0)
		{
			new_key = malloc(sizeof(char) * keylen);
			
			new_key = key;
			while (new_key[index] != '\0')
			{
				key[index] = new_key[index+4];
				index++;
			}
			key[index] = '\0';
		}
		else if (strncmp(key, "PUT:", 4) == 0 && key_num == 2)
		{
			new_key = key;
			while (new_key[index] != '\0')
			{
				key[index] = new_key[index+4];
				index++;
			}
			printf("%s\n", key);
			key[index] = '\0';
			index = 0;
			while (key[index] != ',')
			{
				only_key[index] = key[index];
				printf("%c\n", only_key[index]);
				printf("debug\n");
				index++;
			}
			printf("%s\n", only_key);
			while (key[index] != '\0')
			{
				new_num[index] = key[index+1];
			}
			dbp = db;
			while (*dbp != NULL)
			{
				if (strcmp(only_key, *dbp) == 0)
				{
					flag = 1;
					break;
				}
				dbp += 2;
			}
			
			
			
		}
        else
		{
            printf("argumet error\n");
            exit(1);
        }
        printf("Received key> %s\n",key);
        ans = return_ans(key_num, key, dbp);

		if(*dbp == NULL) strcpy(ans, "No entry");
		/* 讀懃ｴ｢縺励◆繝��繧ｿ繧偵た繧ｱ繝�ヨ縺ｫ譖ｸ縺崎ｾｼ繧 */
        datalen = strlen(ans);
		if(sendto(socd, ans, strlen(ans), 0, (struct sockaddr *)&c_address, c_addrlen) != datalen) {
			fprintf(stderr, "datagram error\n"); 
			exit(1);
		}
		printf("Sent data> %s\n", ans);
		if (flag == 1)
		{
			int i = 0;
			while (only_key != NULL)
			{
				if (strcmp(only_key, *db) == 0)
				{
					i++;
					db[i] = new_num;
					break;
				}
				i ++;
			}
		}
	}
}
