CC = gcc
CFLAGS = -Wall -Wextra -Werror
DEBUG_FLAGS = -fsanitize=thread -g3 -pthread

SRC_DIR_DEF = src
SRC_DIR_TAS = src/tas
SRC_DIR_TATAS = src/tatas

INCLUDE_DIR_TAS = ../spinlock/include/tas
INCLUDE_DIR_TATAS = ../spinlock/include/tatas

LIB_DIR = ../spinlock/lib
BUILD_DIR = build

NAME_DEF = prodcons
NAME_TAS = prodcons_tas
NAME_TATAS = prodcons_tatas

SRCS_DEF = $(wildcard $(SRC_DIR_DEF)/*.c)
SRCS_TAS = $(wildcard $(SRC_DIR_TAS)/*.c)
SRCS_TATAS = $(wildcard $(SRC_DIR_TATAS)/*.c)

OBJS_DEF = $(patsubst $(SRC_DIR_DEF)/%.c, $(BUILD_DIR)/%.o, $(SRCS_DEF))
OBJS_TAS = $(patsubst $(SRC_DIR_TAS)/%.c, $(BUILD_DIR)/tas/%.o, $(SRCS_TAS))
OBJS_TATAS = $(patsubst $(SRC_DIR_TATAS)/%.c, $(BUILD_DIR)/tatas/%.o, $(SRCS_TATAS))

all: $(NAME_TAS) $(NAME_TATAS) $(NAME_DEF)

# Normal 
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
	@rm -rf $(BUILD_DIR) debug csv graph
	@echo "Object files removed"

fclean: clean
	@rm -f $(NAME_TAS) $(NAME_TATAS) $(NAME_DEF)
	@echo "Executables removed"

re: fclean all

# Run performance tests
perf: $(NAME_TAS) $(NAME_TATAS) $(NAME_DEF)
	@echo "Running performance tests..."
	../script/performance.sh ../prodcons/$(NAME_TAS) ${NAME_TAS}
	../script/performance.sh ../prodcons/$(NAME_TATAS) ${NAME_TATAS}
	../script/performance.sh ../prodcons/$(NAME_DEF) ${NAME_DEF}

plot: $(NAME_TAS) $(NAME_TATAS) $(NAME_DEF)
	@echo "Plotting..."
	python3 ../script/graph.py ../prodcons/csv/$(NAME_DEF).csv
	python3 ../script/graph.py ../prodcons/csv/$(NAME_TAS).csv
	python3 ../script/graph.py ../prodcons/csv/$(NAME_TATAS).csv

	python3 ../script/combine.py ../philo/csv --combined-name philo --output-dir graph

# Valrgind
valgrind: $(NAME_TAS) $(NAME_TATAS) $(NAME_DEF)
	@echo "Running valgrind tests..."
	@mkdir -p debug
	# write valgrind output in debug/valgrind.log
	valgrind --tool=memcheck --log-file=debug/$(NAME_DEF)_valgrind.log ../prodcons/$(NAME_DEF) 2 2

# Helgrind
helgrind: $(NAME_TAS) $(NAME_TATAS) $(NAME_DEF)
	@echo "Running helgrind tests..."
	@mkdir -p debug
	# write helgrind output in debug/helgrind.log
	valgrind --tool=helgrind --log-file=debug/$(NAME_DEF)_helgrind.log ../prodcons/$(NAME_DEF) 2 2

.PHONY: all clean fclean re debug perf plot valgrind helgrind
