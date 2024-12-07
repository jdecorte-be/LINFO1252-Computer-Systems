PROJECTS = spinlock philo prodcons writeread 

SCRIPTS_DIR = script

all: $(PROJECTS)

$(PROJECTS): perf plot
	@echo "Building $@..."
	@$(MAKE) -C $@

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

perf: $(PROJECTS)
	@echo "Running performance tests..."

	@for project in $(PROJECTS); do \
		echo "Running performance tests for $$project..."; \
		$(MAKE) -C $$project perf; \
	done

plot: $(PROJECTS)
	@echo "Plotting..."
	@for project in $(PROJECTS); do \
		echo "Plotting $$project..."; \
		$(MAKE) -C $$project plot; \
	done

.PHONY: all clean fclean re $(PROJECTS)