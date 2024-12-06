NAME = prodcons

CFLAGS = -Wall -Wextra -Werror

RM = rm -rf

SRCS = 	src/prodcons.c

$(NAME) :
	gcc $(CFLAGS) $(SRCS) -o $(NAME)

all : $(NAME)

fclean : clean
	$(RM) $(NAME)

clean :
	$(RM) $(NAME)

re : fclean all