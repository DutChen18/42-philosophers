SRC=main.c philo.c util.c
INC=philo.h
OBJ=$(SRC:.c=.o)
CFLAGS=-Wall -Wextra -Werror -O3 -flto
NAME=philo

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c $(INC)
	$(CC) $(CFLAGS) -o $@ $< -c

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all
