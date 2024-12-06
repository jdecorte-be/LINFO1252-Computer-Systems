PROJECTS = philosopher prodcons spinlock

all: $(PROJECTS)

$(PROJECTS):
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

.PHONY: all clean fclean re $(PROJECTS)