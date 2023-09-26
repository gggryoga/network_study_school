NAME = server
NAME2 = client
CC = gcc

RM = rm -rf

$(NAME) : dg_server.c
	$(CC) -o $(NAME) dg_server.c

$(NAME2) : dg_client.c
	$(CC) -o $(NAME2) dg_client.c

all : $(NAME) $(NAME2)

clean :
	$(RM) $(NAME) $(NAME2)

fclean : clean

