CC = gcc
CFLAGS = -Wall -Wextra -Werror -pthread
CFLAGS += -I./include

NAME = philo
BUILD_DIR = build
SRC_DIR = src

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))

VALGRIND = valgrind
VALFLAGS = --leak-check=full \
           --show-leak-kinds=all \
           --track-origins=yes \
           --verbose \
           --error-exitcode=1

all: $(NAME)

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

clean:
	@rm -rf $(BUILD_DIR)
	@echo "Object files removed"

fclean: clean
	@rm -f $(NAME)
	@echo "Executable removed"

re: fclean all

# debug
valgrind: $(NAME)
	$(VALGRIND) $(VALFLAGS) ./$(NAME) 2

debug: CFLAGS += -g3 -fsanitize=thread
debug: re

.PHONY: all clean fclean re valgrind debug run