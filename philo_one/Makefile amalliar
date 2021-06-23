SHELL      := /bin/sh
CC         := clang
CFLAGS     := -Wall -Wextra -fdiagnostics-color -g -pipe \
              -march=native -O2 -flto
INCLUDE    := -I./include
LIBS       := -lpthread
NAME       := philo_one
SRCDIR     := src
OBJDIR     := .obj
DEPDIR     := .dep

SRCS       := philo.c
SRCS       := $(addprefix src/, $(SRCS))
OBJS       := $(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
DEPS       := $(SRCS:$(SRCDIR)/%.c=$(DEPDIR)/%.d)

# Run multiple threads.
MAKEFLAGS  = -j 4 --output-sync=recurse --no-print-directory

# Protect against make incorrectly setting 'last modified' attribute
# when running in parallel (-j flag).
POST_COMPILE = mv -f $(DEPDIR)/$*.tmp $(DEPDIR)/$*.d && touch $@

# Define some colors for echo -e:
LGREEN     := \033[1;32m
WHITE      := \033[1;37m
NOC        := \033[0m

all: $(NAME)
$(NAME): $(OBJS)
	@echo -e "$(LGREEN)Linking executable $(NAME)$(NOC)"
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $@
	@echo -e "Built target $(NAME)"
.PHONY: all

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(DEPDIR)/%.d | $(OBJDIR) $(DEPDIR)
	$(CC) $(CFLAGS) $(INCLUDE) -MMD -MF $(DEPDIR)/$*.tmp -c $< -o $@
	@$(POST_COMPILE)
$(OBJDIR):
	@mkdir -p $@
$(DEPDIR):
	@mkdir -p $@
$(DEPDIR)/%.d: ;
.PRECIOUS: $(DEPDIR)/%.d

clean:
	@echo -e "$(WHITE)Removing C object files...$(NOC)"
	@-rm -rf $(OBJDIR)
	@echo -e "$(WHITE)Removing make dependency files...$(NOC)"
	@-rm -rf $(DEPDIR)
.PHONY: clean

fclean: clean
	@echo -e "$(WHITE)Removing executable $(NAME)$(NOC)"
	@-rm -f $(NAME)
.PHONY: fclean

re:
	$(MAKE) fclean
	$(MAKE) all
.PHONY: re

help:
	@echo -e "The following are some of the valid targets for this Makefile:"
	@echo -e "... all (the default if no target is provided)"
	@echo -e "... clean"
	@echo -e "... fclean"
	@echo -e "... re"
.PHONY: help

# Do not include dependency files if the current goal is
# set to clean/fclean/re.
ifeq ($(findstring $(MAKECMDGOALS), clean fclean re),)
    -include $(DEPS)
endif
