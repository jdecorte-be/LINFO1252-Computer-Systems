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
	@rm -rf $(BUILD_DIR) debug csv graph
	@echo "Object files removed"

fclean: clean
	@rm -f $(LIB_TAS) $(LIB_TATAS) $(NAME_TAS) $(NAME_TATAS)
	@rm -rf $(LIB_DIR)
	@echo "Libraries and executables removed"

re: fclean all

# Run performance tests
perf: $(NAME_TAS) $(NAME_TATAS)
	@echo "Running performance tests..."
	../script/performance.sh ../spinlock/$(NAME_TAS) ${NAME_TAS}
	../script/performance.sh ../spinlock/$(NAME_TATAS) ${NAME_TATAS}

plot: $(NAME_TAS) $(NAME_TATAS)
	@echo "Plotting..."
	python3 ../script/graph.py ../spinlock/csv/$(NAME_TAS).csv
	python3 ../script/graph.py ../spinlock/csv/$(NAME_TATAS).csv

# Valgrind
valgrind: $(NAME_TAS) $(NAME_TATAS)
	@echo "Running valgrind tests..."
	@mkdir -p debug
	# write valgrind output in debug/valgrind.log
	valgrind --tool=memcheck --log-file=debug/$(NAME_TAS)_valgrind.log ../spinlock/$(NAME_TAS) 2 2
	valgrind --tool=memcheck --log-file=debug/$(NAME_TATAS)_valgrind.log ../spinlock/$(NAME_TATAS) 2 2

# Helgrind
helgrind: $(NAME_TAS) $(NAME_TATAS)
	@echo "Running helgrind tests..."
	@mkdir -p debug
	# write helgrind output in debug/helgrind.log
	valgrind --tool=helgrind --log-file=debug/$(NAME_TAS)_helgrind.log ../spinlock/$(NAME_TAS) 2 2
	valgrind --tool=helgrind --log-file=debug/$(NAME_TATAS)_helgrind.log ../spinlock/$(NAME_TATAS) 2 2


.PHONY: all clean fclean re debug
