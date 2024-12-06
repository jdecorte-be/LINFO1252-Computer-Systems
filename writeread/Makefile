NAME = writeread

CFLAGS = -Wall -Wextra -Werror

RM = rm -rf

SRCS = 	src/writeread.c

$(NAME) :
	gcc $(CFLAGS) $(SRCS) -o $(NAME)

all : $(NAME)

fclean : clean
	$(RM) $(NAME)

clean :
	$(RM) $(NAME)

re : fclean all