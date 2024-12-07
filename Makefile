PROJECTS = spinlock philo prodcons writeread

SCRIPTS_DIR = script

all: $(PROJECTS)

$(PROJECTS): 
	@echo "Building $@..."
	$(MAKE) -C $@ > /dev/null

clean:
	@for project in $(PROJECTS); do \
		echo "Cleaning $$project..."; \
		$(MAKE) -C $$project clean; \
	done

fclean: 
	@for project in $(PROJECTS); do \
		echo "Fully cleaning $$project..."; \
		$(MAKE) -C $$project fclean; \
	done

re: fclean all

# Run performance tests
perf: $(PROJECTS)
	@echo "\n\n\nRunning performance tests..."
	@for project in $(PROJECTS); do \
		echo "Running performance tests for $$project..."; \
		$(MAKE) -C $$project perf; \
	done

# Plot
plot: $(PROJECTS)
	@echo "\n\n\nPlotting..."
	for project in $(PROJECTS); do \
		echo "Plotting $$project..."; \
		$(MAKE) -C $$project plot; \
	done

test: perf plot

# Valgrind
valgrind: $(PROJECTS)
	@echo "\n\n\nRunning valgrind tests..."
	for project in $(PROJECTS); do \
		echo "Running valgrind tests for $$project..."; \
		$(MAKE) -C $$project valgrind; \
	done

# Helgrind
helgrind: $(PROJECTS)
	@echo "\n\n\nRunning helgrind tests..."
	for project in $(PROJECTS); do \
		echo "Running helgrind tests for $$project..."; \
		$(MAKE) -C $$project helgrind; \
	done

.PHONY: all clean fclean re $(PROJECTS) perf plot