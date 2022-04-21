SRC=main.c philo.c util.c
OBJ=$(SRC:.c=.o)
CFLAGS=-Wall -Wextra -Werror -Og -g -ansi -pedantic
LDFLAGS=-Wall -Wextra -Werror -Og -g -pedantic
NAME=philo

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c $(INC)
	$(CC) $(CFLAGS) -o $@ $< -c

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean
	make all
