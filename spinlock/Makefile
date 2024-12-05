NAME_TAS = tas
NAME_TATAS = tatas
NAME = $(NAME_TAS) $(NAME_TATAS)

CFLAGS = -Wall -Wextra -Werror
DEBUG_FLAGS = -fsanitize=address -g

RM = rm -rf

DIR_TAS = src/tas
DIR_TATAS = src/tatas

SRCS_TAS = $(DIR_TAS)/main_test_and_set.c $(DIR_TAS)/test_and_set.c
SRCS_TATAS = $(DIR_TATAS)/main_test_and_test_and_set.c $(DIR_TATAS)/test_and_test_and_set.c

INCLUDE_TAS = -Iinclude/tas
INCLUDE_TATAS = -Iinclude/tatas

$(NAME) :
	gcc $(CFLAGS) $(SRCS_TAS) $(INCLUDE_TAS) -o $(NAME_TAS)
	gcc $(CFLAGS) $(SRCS_TATAS) $(INCLUDE_TATAS) -o $(NAME_TATAS)

debug :
	gcc $(DEBUG_FLAGS) $(SRCS_TAS) $(INCLUDE_TAS) -o $(NAME_TAS)

all : $(NAME)

fclean : clean
	$(RM) $(NAME)

clean :
	$(RM) $(NAME)

re : fclean all