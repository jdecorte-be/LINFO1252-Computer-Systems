CC = gcc
CFLAGS = -Wall -Wextra -Werror
DEBUG_FLAGS = -fsanitize=address -g -pthread

SRC_DIR_TAS = src/tas
SRC_DIR_TATAS = src/tatas
INCLUDE_DIR_TAS = include/tas
INCLUDE_DIR_TATAS = include/tatas
BUILD_DIR = build
LIB_DIR = lib

NAME_TAS = tas
NAME_TATAS = tatas
LIB_TAS = $(LIB_DIR)/libtas.a
LIB_TATAS = $(LIB_DIR)/libtatas.a

SRCS_TAS = $(wildcard $(SRC_DIR_TAS)/*.c)
SRCS_TATAS = $(wildcard $(SRC_DIR_TATAS)/*.c)

OBJS_TAS = $(patsubst $(SRC_DIR_TAS)/%.c, $(BUILD_DIR)/tas/%.o, $(SRCS_TAS))
OBJS_TATAS = $(patsubst $(SRC_DIR_TATAS)/%.c, $(BUILD_DIR)/tatas/%.o, $(SRCS_TATAS))

all: $(LIB_TAS) $(LIB_TATAS) $(NAME_TAS) $(NAME_TATAS)

$(BUILD_DIR)/tas/%.o: $(SRC_DIR_TAS)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR_TAS) -c $< -o $@

$(LIB_TAS): $(OBJS_TAS)
	@mkdir -p $(LIB_DIR)
	ar rcs $@ $^

$(BUILD_DIR)/tatas/%.o: $(SRC_DIR_TATAS)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR_TATAS) -c $< -o $@

$(LIB_TATAS): $(OBJS_TATAS)
	@mkdir -p $(LIB_DIR)
	ar rcs $@ $^

$(NAME_TAS): $(LIB_TAS)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR_TAS) -L$(LIB_DIR) -ltas -o $(NAME_TAS)

$(NAME_TATAS): $(LIB_TATAS)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR_TATAS) -L$(LIB_DIR) -ltatas -o $(NAME_TATAS)

debug: CFLAGS += $(DEBUG_FLAGS)
debug: re

clean:
	@rm -rf $(BUILD_DIR)
	@echo "Object files removed"

fclean: clean
	@rm -f $(LIB_TAS) $(LIB_TATAS) $(NAME_TAS) $(NAME_TATAS)
	@rm -rf $(LIB_DIR)
	@echo "Libraries and executables removed"

re: fclean all

.PHONY: all clean fclean re debug
