CC = gcc
CFLAGS = -Wall -Wextra -Werror
DEBUG_FLAGS = -fsanitize=address -g -pthread

SRC_DIR_DEF = src
SRC_DIR_TAS = src/tas
SRC_DIR_TATAS = src/tatas

INCLUDE_DIR_TAS = ../spinlock/include/tas
INCLUDE_DIR_TATAS = ../spinlock/include/tatas

LIB_DIR = ../spinlock/lib
BUILD_DIR = build

NAME_DEF = philo
NAME_TAS = philo_tas
NAME_TATAS = philo_tatas

SRCS_DEF = $(wildcard $(SRC_DIR_DEF)/*.c)
SRCS_TAS = $(wildcard $(SRC_DIR_TAS)/*.c)
SRCS_TATAS = $(wildcard $(SRC_DIR_TATAS)/*.c)

OBJS_DEF = $(patsubst $(SRC_DIR_DEF)/%.c, $(BUILD_DIR)/%.o, $(SRCS_DEF))
OBJS_TAS = $(patsubst $(SRC_DIR_TAS)/%.c, $(BUILD_DIR)/tas/%.o, $(SRCS_TAS))
OBJS_TATAS = $(patsubst $(SRC_DIR_TATAS)/%.c, $(BUILD_DIR)/tatas/%.o, $(SRCS_TATAS))

all: $(NAME_TAS) $(NAME_TATAS) $(NAME_DEF)

# Normal Philo 
$(BUILD_DIR)/%.o: $(SRC_DIR_DEF)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME_DEF): $(OBJS_DEF)
	$(CC) $(CFLAGS) $(OBJS_DEF) -o $(NAME_DEF)

# TAS
$(BUILD_DIR)/tas/%.o: $(SRC_DIR_TAS)/%.c
	@mkdir -p $(dir $@)
	$(MAKE) -C ../spinlock
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR_TAS)  -c $< -o $@

$(NAME_TAS): $(OBJS_TAS)
	$(CC) $(CFLAGS) $(OBJS_TAS) $(LIB_DIR)/libtas.a  -o $(NAME_TAS)

# TATAS
$(BUILD_DIR)/tatas/%.o: $(SRC_DIR_TATAS)/%.c
	@mkdir -p $(dir $@)
	$(MAKE) -C ../spinlock
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR_TATAS) -c $< -o $@

$(NAME_TATAS): $(OBJS_TATAS)
	$(CC) $(CFLAGS) $(OBJS_TATAS) $(LIB_DIR)/libtatas.a -o $(NAME_TATAS)


debug: CFLAGS += $(DEBUG_FLAGS)
debug: re

clean:
	@rm -rf $(BUILD_DIR)
	@echo "Object files removed"

fclean: clean
	@rm -f $(NAME_TAS) $(NAME_TATAS) $(NAME_DEF)
	@echo "Executables removed"

re: fclean all

.PHONY: all clean fclean re debug
